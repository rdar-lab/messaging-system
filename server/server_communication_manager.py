import logging
import selectors
import socket
import threading
import types

from constants import *
from model import Response
from request_handler import RequestHandler
from request_reader import RequestReader
from response_writer import ResponseWriter
from storage_manager import StorageManager

_logger = logging.getLogger(__name__)


class ServerCommunicationManager:
    def __init__(self, storage_manager: StorageManager, port: int):
        self.__storage_manager = storage_manager
        self.__port = port
        self.__sel = selectors.DefaultSelector()

    def start_server(self):
        _logger.info("Starting server at port {}".format(self.__port))
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(("0.0.0.0", self.__port))
        sock.listen(CONNECTIONS_BACKLOG)
        sock.setblocking(False)
        self.__sel.register(sock, selectors.EVENT_READ, data=None)

        while True:
            events = self.__sel.select(timeout=None)
            for key, mask in events:
                if key.data is None:
                    self.__accept_connection(key, mask)
                else:
                    client_thread = threading.Thread(target=self.__handle_data, args=(key, mask))
                    client_thread.start()

    def __accept_connection(self, key, mask):
        sock = key.fileobj
        conn, addr = sock.accept()
        _logger.info("Got connection from {}".format(addr))
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr)
        events = selectors.EVENT_READ
        self.__sel.register(conn, events, data=data)

    def __handle_data(self, key, mask):
        sock = key.fileobj
        self.__sel.unregister(sock)
        data = key.data
        addr = data.addr

        try:
            request_reader = RequestReader(sock)
            req = request_reader.read_request()
            _logger.info("{0} - Request = {1}".format(addr, req))
            request_handler = RequestHandler(self.__storage_manager)
            resp = request_handler.handle_request(req)
        except Exception:
            _logger.exception("{0} - Error while handling request".format(addr))
            resp = Response(SERVER_VERSION, RESPONSE_CODE_GENERAL_ERROR, {})

        _logger.info("{0} - Response = {1}".format(addr, resp))

        try:
            response_writer = ResponseWriter(sock)
            response_writer.write_response(resp)
        except Exception:
            _logger.exception("{0} - Was unable to send the response".format(addr))

        _logger.info("Closing connection to {}".format(addr))

        try:
            sock.close()
        except Exception:
            pass

