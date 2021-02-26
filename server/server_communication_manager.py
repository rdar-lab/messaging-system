import logging
import socket
import threading

from constants import *
from model import Response
from request_handler import RequestHandler
from request_reader import RequestReader
from response_writer import ResponseWriter
from storage_manager import StorageManager

_logger = logging.getLogger(__name__)


class ServerCommunicationManager:
    """
    This manager is responsible for starting the TCP server
    and manage the incoming connections
    """

    def __init__(self, storage_manager: StorageManager, port: int):
        self.__storage_manager = storage_manager
        self.__port = port

    def start_server(self):
        _logger.info("Starting server at port {}".format(self.__port))
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(("0.0.0.0", self.__port))
        sock.listen(CONNECTIONS_BACKLOG)

        try:

            while True:
                client_sock, addr = sock.accept()
                client_thread = threading.Thread(target=self.__handle_data, args=(client_sock, addr))
                client_thread.start()
        finally:
            sock.close()

    # noinspection PyBroadException
    def __handle_data(self, sock, addr):
        request_handler = RequestHandler(self.__storage_manager)
        req = None
        resp = None

        try:
            _logger.info("{0} - Handing new connection".format(addr))
            sock.settimeout(SOCKET_TIMEOUT)
            request_reader = RequestReader(sock)
            req = request_reader.read_request()
            _logger.info("{0} - Request = {1}".format(addr, req))
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

        try:
            request_handler.handle_post_request(req, resp)
        except Exception:
            _logger.exception("{0} - Error while handling post request".format(addr))
