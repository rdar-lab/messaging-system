import socket

from model import Response
from response_payload_writer import ResponsePayloadWriter
from socket_helper import SockHelper


class ResponseWriter:
    def __init__(self, sock: socket.socket):
        self.__sock = sock
        self.__helper = SockHelper(self.__sock)

    def write_response(self, resp: Response):
        server_version = resp.get_version()
        self.__helper.write_byte(server_version)
        response_code = resp.get_response_code()
        self.__helper.write_short(response_code)
        ResponsePayloadWriter(resp, self.__sock).write_payload()
