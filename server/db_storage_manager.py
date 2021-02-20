import logging
import os
import sqlite3
import threading
from datetime import datetime

from bytes_buffer import BytesBuffer
from model import Message, Client
from storage_manager import StorageManager
from utils import generate_client_id

_logger = logging.getLogger(__name__)

_DB_FILE_NAME = "server.db"
_DB_CONNECTION_TIMEOUT = 5

_COUNT_CLIENTS_SQL = "SELECT COUNT(*) AS rows_count FROM clients"
_COUNT_MESSAGES_SQL = "SELECT COUNT(*) AS rows_count FROM messages"
_CREATE_TABLE_CLIENTS_SQL = \
    "CREATE TABLE clients(" \
    "   client_id TEXT PRIMARY KEY, " \
    "   client_name TEXT unique," \
    "   client_pk BLOB," \
    "   last_update TIMESTAMP) "

_CREATE_TABLE_MESSAGES_SQL = \
    "CREATE TABLE messages(" \
    "   message_id INTEGER PRIMARY KEY, " \
    "   from_client_id TEXT, " \
    "   to_client_id TEXT, " \
    "   message_type TINYINT, " \
    "   message_content BLOB)"
_SELECT_ALL_CLIENTS_SQL = "SELECT * FROM clients"
_SELECT_CLIENT_BY_CLIENT_ID_SQL = "SELECT * FROM clients WHERE client_id=?"
_INSERT_CLIENT_SQL = "INSERT INTO clients(client_id, client_name, client_pk) VALUES(?,?,?)"
_UPDATE_CLIENT_DATE_SQL = "UPDATE clients SET last_update=? where client_id=?"
_DELETE_CLIENT_BY_ID_SQL = "DELETE FROM clients WHERE client_id=?"
_INSERT_MSG_SQL = \
    "INSERT INTO messages(to_client_id, from_client_id, message_type, message_content) " \
    "VALUES(?,?,?,?)"
_SELECT_MESSAGES_FOR_CLIENT_SQL = "SELECT * FROM messages WHERE to_client_id=?"
_DELETE_MESSAGE_BY_ID_SQL = "DELETE FROM messages WHERE message_id=?"

# SQLite can only be accessed by one thread
# Since SQLite3 it can have multiple threads but only one writes to it
# So we will use a global local
_db_conn_global_lock = threading.Lock()


class _DbConnection:
    def __init__(self):
        self.__conn = None

    def __enter__(self):
        _db_conn_global_lock.acquire()
        _logger.debug("Opening connection")
        self.__conn = sqlite3.connect(_DB_FILE_NAME, timeout=_DB_CONNECTION_TIMEOUT)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.__conn:
            _logger.debug("Closing connection")
            self.__conn.commit()
            self.__conn.close()
            self.__conn = None
        _db_conn_global_lock.release()

    def __execute_and_get_cursor(self, sql, params=None):
        _logger.info("Executing SQL={0} With params={1}".format(sql, params))
        cursor = self.__conn.cursor()
        if params is not None:
            cursor.execute(sql, params)
        else:
            cursor.execute(sql)
        return cursor

    def execute_and_get_result(self, sql, params=None):
        cursor = self.__execute_and_get_cursor(sql, params)
        rows = []
        column_names = cursor.description
        for row in cursor:
            row_result = {}
            for index in range(len(column_names)):
                key = column_names[index][0]
                row_result[key] = row[index]
            rows.append(row_result)

        _logger.info("Result={}".format(rows))
        return rows

    def execute(self, sql, params=None):
        self.__execute_and_get_cursor(sql, params)
        _logger.info("Execution done")

    def execute_and_get_id(self, sql, params=None):
        cursor = self.__execute_and_get_cursor(sql, params)
        row_id = cursor.lastrowid
        _logger.info("Execution done, ID={}".format(row_id))
        return row_id

    def commit(self):
        self.__conn.commit()

    def rollback(self):
        self.__conn.rollback()


class DbStorageManager(StorageManager):
    def __init__(self):
        is_ok = False
        if os.path.exists(_DB_FILE_NAME):
            _logger.info("Db file name exists")
            is_ok = self.validate_db()
            if not is_ok:
                _logger.warning("Db is corrupted and will be removed")
                os.remove(_DB_FILE_NAME)
        if not is_ok:
            self.create_db()

    @staticmethod
    def __open_connection():
        return _DbConnection()

    @staticmethod
    def __read_client_from_row(row) -> Client:
        client_id = row["client_id"]
        client_name = row["client_name"]
        client_pk = row["client_pk"]
        last_update = row["last_update"]
        client = Client(client_id, client_name, client_pk)
        client.set_last_seen(last_update)
        return client

    def get_all_clients(self) -> dict:
        _logger.info("Selecting all clients")
        with self.__open_connection() as conn:
            rows = conn.execute_and_get_result(_SELECT_ALL_CLIENTS_SQL)
            result = {}
            for row in rows:
                client = self.__read_client_from_row(row)
                result[client.get_client_id()] = client
            _logger.info("Clients={}".format(result))
            return result

    def get_client(self, client_id) -> Client:
        _logger.info("Searching for client by ID={}".format(client_id))
        with self.__open_connection() as conn:
            rows = conn.execute_and_get_result(_SELECT_CLIENT_BY_CLIENT_ID_SQL, params=(client_id,))
            if len(rows) == 0:
                client = None
            else:
                client = self.__read_client_from_row(rows[0])
        _logger.info("client={}".format(client))
        return client

    def add_client(self, client_name, client_public_key) -> Client:
        client_id = generate_client_id()
        _logger.info("Adding client. ID={0}, Name={1}, PK={2}".format(client_id, client_name, client_public_key))
        with self.__open_connection() as conn:
            conn.execute(_INSERT_CLIENT_SQL, params=(client_id, client_name, client_public_key))
        client = self.get_client(client_id)
        if client is None:
            raise Exception("Client creation failure. client not found after creation")
        return client

    def update_last_seen(self, client_id):
        _logger.info("Updating client last seen, client_id={}".format(client_id))
        with self.__open_connection() as conn:
            conn.execute_and_get_result(_UPDATE_CLIENT_DATE_SQL, params=(client_id, datetime.now()))

    def remove_client(self, client_id):
        _logger.info("Removing client by ID={}".format(client_id))
        with self.__open_connection() as conn:
            conn.execute(_DELETE_CLIENT_BY_ID_SQL, params=(client_id,))

    def add_message(self, to_client_id, from_client_id, message_type, message_content) -> Message:
        _logger.info("Adding new message, to_client_id={0}, from_client_id={1}, message_type={2}".format(
            to_client_id, from_client_id, message_type))

        # translate message_content to the bytes representation
        # TODO: replace with file implementation to save server memory with dealing with very large files
        if isinstance(message_content, BytesBuffer):
            length = len(message_content)
            message_content = message_content.read(length)

        with self.__open_connection() as conn:
            message_id = conn.execute_and_get_id(_INSERT_MSG_SQL,
                                                 params=(to_client_id, from_client_id, message_type, message_content))
            msg = Message(message_id, to_client_id, from_client_id, message_type, message_content)
            _logger.info("New message: {}".format(msg))
            return msg

    def get_messages(self, client_id, limit=0) -> list:
        _logger.info("Reading messages from client: {}".format(client_id))
        with self.__open_connection() as conn:
            sql = _SELECT_MESSAGES_FOR_CLIENT_SQL
            if limit > 0:
                # Note: this parameter should never come from the user, so no SQL injection
                sql = sql + ' LIMIT ' + str(limit)
            rows = conn.execute_and_get_result(sql, params=(client_id,))
            messages = []
            for row in rows:
                msg = Message(
                    row["message_id"],
                    row["to_client_id"],
                    row["from_client_id"],
                    row["message_type"],
                    row["message_content"]
                )
                messages.append(msg)
            _logger.info("Messages={}".format(messages))
            return messages

    def remove_messages(self, messages_to_remove=None):
        with self.__open_connection() as conn:
            for msg in messages_to_remove:
                msg_id = msg.get_message_id()
                _logger.info("Removing message: {}".format(msg_id))
                conn.execute(_DELETE_MESSAGE_BY_ID_SQL, params=(msg_id,))

    # noinspection PyBroadException
    def validate_db(self):
        _logger.info("Validating DB integrity")
        try:
            with self.__open_connection() as conn:
                rows = conn.execute_and_get_result(_COUNT_CLIENTS_SQL)
                _logger.info("Db contains {} clients".format(rows[0]["rows_count"]))
                rows = conn.execute_and_get_result(_COUNT_MESSAGES_SQL)
                _logger.info("Db contains {} messages".format(rows[0]["rows_count"]))
                return True
        except Exception as ex:
            _logger.error("Error in validation of DB. error={}".format(ex))
            return False

    def create_db(self):
        _logger.info("Creating DB")
        with self.__open_connection() as conn:
            conn.execute_and_get_result(_CREATE_TABLE_CLIENTS_SQL)
            conn.execute_and_get_result(_CREATE_TABLE_MESSAGES_SQL)
        _logger.info("DB Created")
