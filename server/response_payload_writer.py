from socket import socket

from bytes_buffer import BytesBuffer
from constants import *
from model import Response
from socket_helper import SockHelper


class ResponsePayloadWriter:
    """
    The response payload writer is responsible for writing the response payload to the socket
    """

    def __init__(self, resp: Response, sock: socket):
        super().__init__()
        self.__resp = resp
        self.__sock = sock
        self.__helper = SockHelper(self.__sock)

    def write_payload(self):
        payload = self.__resp.get_payload_map()
        payload_size = self.__calc_payload_size(payload)
        self.__helper.write_int(payload_size)
        self.__write_payload(payload)

    def __write_payload(self, payload):
        if RESPONSE_PAYLOAD_CLIENT_ID in payload:
            self.__helper.write_bytes(payload[RESPONSE_PAYLOAD_CLIENT_ID])
        if RESPONSE_PAYLOAD_CLIENTS in payload:
            for client in payload[RESPONSE_PAYLOAD_CLIENTS].values():
                client_id = client.get_client_id()
                self.__helper.write_bytes(client_id)
                client_name = client.get_client_name()
                self.__helper.write_str(client_name, STR_SIZE)
        if RESPONSE_PAYLOAD_PUBLIC_KEY in payload:
            self.__helper.write_bytes(payload[RESPONSE_PAYLOAD_PUBLIC_KEY])
        if RESPONSE_PAYLOAD_MESSAGE_ID in payload:
            self.__helper.write_int(payload[RESPONSE_PAYLOAD_MESSAGE_ID])
        if RESPONSE_PAYLOAD_MESSAGES in payload:
            for message in payload[RESPONSE_PAYLOAD_MESSAGES]:
                from_client_id = message.get_from_client_id()
                self.__helper.write_bytes(from_client_id)
                msg_id = message.get_message_id()
                self.__helper.write_int(msg_id)
                msg_type = message.get_message_type()
                self.__helper.write_byte(msg_type)
                msg_body = message.get_message_content()
                msg_body_len = len(msg_body)
                self.__helper.write_int(msg_body_len)
                if isinstance(msg_body, BytesBuffer):
                    msg_body.write_to_socket(self.__sock)
                else:
                    self.__helper.write_bytes(msg_body)

    @staticmethod
    def __calc_payload_size(payload):
        size = 0
        if RESPONSE_PAYLOAD_CLIENT_ID in payload:
            size = size + CLIENT_ID_SIZE

        if RESPONSE_PAYLOAD_CLIENTS in payload:
            size = size + len(payload[RESPONSE_PAYLOAD_CLIENTS].values()) * (CLIENT_ID_SIZE + STR_SIZE)

        if RESPONSE_PAYLOAD_PUBLIC_KEY in payload:
            size = size + PUBLIC_KEY_SIZE

        if RESPONSE_PAYLOAD_MESSAGE_ID in payload:
            size = size + MESSAGE_ID_SIZE

        if RESPONSE_PAYLOAD_MESSAGES in payload:
            for message in payload[RESPONSE_PAYLOAD_MESSAGES]:
                size = size + CLIENT_ID_SIZE + MESSAGE_ID_SIZE + MESSAGE_TYPE_SIZE + MESSAGE_LENGTH_SIZE + len(
                    message.get_message_content())

        return size
