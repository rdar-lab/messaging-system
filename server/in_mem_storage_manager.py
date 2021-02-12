import logging
import threading
from datetime import datetime

from bytes_buffer import BytesBuffer
from model import Message, Client
from storage_manager import StorageManager
from utils import generate_client_id

_logger = logging.getLogger(__name__)


class InMemStorageManager(StorageManager):
    def __init__(self):
        self.__clients = {}
        self.__messages = {}
        self.__next_msg_id = 1
        self.__lock = threading.Lock()

    def get_all_clients(self) -> dict:
        return self.__clients

    def get_client(self, client_id) -> Client:
        return self.__clients.get(client_id)

    def add_client(self, client_name, client_public_key) -> Client:
        for client in self.__clients.values():
            if client.get_client_name() == client_name:
                raise Exception("Client already registered")

        client_id = generate_client_id()
        client = Client(client_id, client_name, client_public_key)

        _logger.info("Adding client={}".format(client))

        self.__clients[client_id] = client
        self.__messages[client_id] = {}
        return client

    def update_last_seen(self, client_id):
        if client_id not in self.__clients:
            raise Exception("Client is unknown. id={}".format(client_id))

        _logger.debug("Updating last seen for client id={}".format(client_id))

        client = self.__clients[client_id]
        client.set_last_seen(datetime.now())

    def remove_client(self, client_id):
        if client_id not in self.__clients:
            raise Exception("Client is unknown. id={}".format(client_id))

        _logger.info("Removing client {}".format(client_id))

        del self.__clients[client_id]
        del self.__messages[client_id]

    def add_message(self, to_client_id, from_client_id, message_type, message_content) -> Message:
        # translate message_content to the bytes representation
        # TODO: replace with file implementation to save server memory with dealing with very large files
        if isinstance(message_content, BytesBuffer):
            length = len(message_content)
            message_content = message_content.read(length)

        msg = Message(self.__get_next_msg_id(), to_client_id, from_client_id, message_type, message_content)

        _logger.info("Adding message: {}".format(msg))

        self.__messages[to_client_id][msg.get_message_id()] = msg
        return msg

    def get_messages(self, client_id, limit=0) -> list:
        messages = self.__messages[client_id].values()
        if 0 < limit < len(messages):
            messages = messages[:limit]

        return messages

    def remove_messages(self, messages_to_remove=None):
        _logger.info("Removing messages: {}".format(messages_to_remove))

        for message in messages_to_remove:
            client_id = message.get_to_client_id()
            msg_id = message.get_message_id()
            del self.__messages[client_id][msg_id]

    def __get_next_msg_id(self):
        self.__lock.acquire(True)
        ret_msg_id = self.__next_msg_id
        self.__next_msg_id = self.__next_msg_id + 1
        self.__lock.release()

        _logger.debug("Next message ID={}".format(ret_msg_id))

        return ret_msg_id
