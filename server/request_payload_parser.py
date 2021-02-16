from socket import socket

from bytes_buffer import SocketBytesBuffer
from constants import *
from socket_helper import SockHelper


class RequestPayloadParser:

    def __init__(self, request_code, sock: socket, payload_size: int):
        super().__init__()
        self.__request_code = request_code
        self.__sock = sock
        self.__payload_size = payload_size
        self.__helper = SockHelper(self.__sock)

    def parse(self):
        if self.__payload_size > MAX_PAYLOAD_BYTES:
            raise Exception("Message payload size limit reached. got={}".format(self.__payload_size))

        if self.__request_code == REQUEST_CODE_REGISTER:
            return self.__parse_register_request_params()
        elif self.__request_code == REQUEST_CODE_GET_CLIENTS_LIST:
            return self.__parse_get_clients_req_params()
        elif self.__request_code == REQUEST_CODE_GET_CLIENT_PUBLIC_KEY:
            return self.__parse_get_client_public_key_req_params()
        elif self.__request_code == REQUEST_CODE_SEND_MESSAGE:
            return self.__parse_send_message_req_params()
        elif self.__request_code == REQUEST_CODE_GET_MESSAGES:
            return self.__parse_get_messages_req_params()
        else:
            raise Exception("Got unknown request code. got={}".format(self.__request_code))

    def __parse_register_request_params(self):
        if not self.__payload_size == 255 + 32:
            raise Exception("Incorrect payload size. got={}".format(self.__payload_size))

        client_name = self.__helper.read_str(255)
        client_pk = self.__helper.read_bytes(32)

        return {
            REQUEST_PAYLOAD_CLIENT_NAME: client_name,
            REQUEST_PAYLOAD_PUBLIC_KEY: client_pk
        }

    def __parse_get_clients_req_params(self):
        if not self.__payload_size == 0:
            raise Exception("Incorrect payload size. got={}".format(self.__payload_size))

        return {}

    def __parse_get_client_public_key_req_params(self):
        if not self.__payload_size == 16:
            raise Exception("Incorrect payload size. got={}".format(self.__payload_size))

        client_id = self.__helper.read_bytes(16)
        return {
            REQUEST_PAYLOAD_CLIENT_ID: client_id
        }

    def __parse_send_message_req_params(self):
        if self.__payload_size < 16 + 1 + 4:
            raise Exception("Incorrect payload size. got={}".format(self.__payload_size))

        client_id = self.__helper.read_bytes(16)
        message_type = self.__helper.read_byte()
        message_size = self.__helper.read_int()

        if message_size > MAX_MSG_BODY_BYTES:
            raise Exception("Max message body size reached. got={}".format(message_size))

        if not self.__payload_size == message_size + 16 + 1 + 4:
            raise Exception("Incorrect payload size. does not match message size. got={}".format(message_size))

        if message_type == MESSAGE_TYPE_ENC_KEY_REQUEST:
            if not message_size == 0:
                raise Exception("Incorrect message size. should be 0. instead got {}".format(message_size))
            else:
                message_body = ""
        elif message_type == MESSAGE_TYPE_ENC_KEY_RESP or message_type == MESSAGE_TYPE_TEXT_MESSAGE:
            message_body = self.__helper.read_bytes(message_size)
        elif message_type == MESSAGE_TYPE_FILE_MESSAGE:
            message_body = SocketBytesBuffer(self.__sock, message_size)
        else:
            raise Exception("Unknown message type. got={}".format(message_type))

        return {
            REQUEST_PAYLOAD_CLIENT_ID: client_id,
            REQUEST_PAYLOAD_MESSAGE_TYPE: message_type,
            REQUEST_PAYLOAD_MESSAGE_BODY: message_body
        }

    def __parse_get_messages_req_params(self):
        if not self.__payload_size == 0:
            raise Exception("Incorrect payload size. got={}".format(self.__payload_size))

        return {}
