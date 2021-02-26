import socket

from constants import *
from model import Request
from request_payload_parser import RequestPayloadParser
from socket_helper import SockHelper


class RequestReader:
    """
    Reads the request from the socket and returns it
    """

    def __init__(self, sock: socket.socket):
        self.__sock = sock
        self.__helper = SockHelper(self.__sock)

    def read_request(self):
        client_id = self.__helper.read_bytes(CLIENT_ID_SIZE)
        version = self.__helper.read_byte()
        request_code = self.__helper.read_byte()
        payload_size = self.__helper.read_int()
        payload_parser = RequestPayloadParser(request_code, self.__sock, payload_size)
        payload_map = payload_parser.parse()
        return Request(version, client_id, request_code, payload_map)

