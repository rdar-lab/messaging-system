import logging

from constants import *
from model import Request, Response
from storage_manager import StorageManager

_logger = logging.getLogger(__name__)


class RequestHandler:
    def __init__(self, storage_manager: StorageManager):
        self.__storage_manager = storage_manager

    def handle_request(self, req: Request):
        _logger.info("Handing request={}".format(req))
        if req.get_request_code() == REQUEST_CODE_REGISTER:
            response_code, response_payload_map = self.__handle_registration_request(req)
        else:
            if req.get_client_id() is None or len(req.get_client_id()) == 0:
                raise Exception("Invalid client id. got={}".format(req.get_client_id()))

            client = self.__storage_manager.get_client(req.get_client_id())
            if client is None:
                raise Exception("Client not found")

            self.__storage_manager.update_last_seen(req.get_client_id())

            if req.get_request_code() == REQUEST_CODE_GET_CLIENTS_LIST:
                response_code, response_payload_map = self.__handle_get_clients_list_request(req)
            elif req.get_request_code() == REQUEST_CODE_GET_CLIENT_PUBLIC_KEY:
                response_code, response_payload_map = self.__handle_get_client_public_key(req)
            elif req.get_request_code() == REQUEST_CODE_SEND_MESSAGE:
                response_code, response_payload_map = self.__handle_send_message_request(req)
            elif req.get_request_code() == REQUEST_CODE_GET_MESSAGES:
                response_code, response_payload_map = self.__handle_get_messages_request(req)
            else:
                raise Exception("Unknown request code: {}".format(req.get_request_code()))

        resp = Response(SERVER_VERSION, response_code, response_payload_map)
        _logger.info("Request processed. Response={}".format(resp))
        return resp

    def __handle_registration_request(self, req):
        client_name = req.get_payload_map().get(REQUEST_PAYLOAD_CLIENT_NAME)
        if client_name is None or len(client_name) < 2:
            raise Exception("Client name is invalid. got={}".format(client_name))

        public_key = req.get_payload_map().get(REQUEST_PAYLOAD_PUBLIC_KEY)
        if public_key is None or len(public_key) == 0:
            raise Exception("public key is invalid. got={}".format(public_key))

        client = self.__storage_manager.add_client(client_name, public_key)
        response_code = RESPONSE_CODE_REG_SUCCESS
        response_payload_map = {RESPONSE_PAYLOAD_CLIENT_ID: client.get_client_id()}

        return response_code, response_payload_map

    def __handle_get_clients_list_request(self, req):
        all_clients = self.__storage_manager.get_all_clients()

        if req.get_client_id() in all_clients:
            del all_clients[req.get_client_id()]

        response_code = RESPONSE_CODE_CLIENTS_LIST
        response_payload_map = {RESPONSE_PAYLOAD_CLIENTS: all_clients}

        return response_code, response_payload_map

    def __handle_get_client_public_key(self, req):
        client_id = req.get_payload_map().get(REQUEST_PAYLOAD_CLIENT_ID)

        if client_id is None or len(client_id) == 0:
            raise Exception("Invalid client id. got={}".format(client_id))

        client = self.__storage_manager.get_client(client_id)
        if client is None:
            raise Exception("Client not found by ID={}".format(client_id))

        client_pk = client.get_client_public_key()

        response_code = RESPONSE_CODE_CLIENT_PUBLIC_KEY
        response_payload_map = {
            RESPONSE_PAYLOAD_CLIENT_ID: client_id,
            RESPONSE_PAYLOAD_PUBLIC_KEY: client_pk
        }

        return response_code, response_payload_map

    def __handle_send_message_request(self, req):
        from_client_id = req.get_client_id()
        to_client_id = req.get_payload_map().get(REQUEST_PAYLOAD_CLIENT_ID)

        if to_client_id is None or len(to_client_id) == 0:
            raise Exception("Destination client ID is invalid. got={}".format(to_client_id))

        message_type = req.get_payload_map().get(REQUEST_PAYLOAD_MESSAGE_TYPE)
        if message_type is None:
            raise Exception("Message type is invalid. got={}".format(message_type))

        message_body = req.get_payload_map().get(REQUEST_PAYLOAD_MESSAGE_BODY)
        if message_body is None:
            raise Exception("Message text is invalid. got={}".format(message_body))

        msg = self.__storage_manager.add_message(to_client_id, from_client_id, message_type, message_body)
        msg_id = msg.get_message_id()

        response_code = RESPONSE_CODE_MESSAGE_SENT
        response_payload_map = {
            RESPONSE_PAYLOAD_CLIENT_ID: to_client_id,
            RESPONSE_PAYLOAD_MESSAGE_ID: msg_id
        }

        return response_code, response_payload_map

    def __handle_get_messages_request(self, req):
        client_id = req.get_client_id()
        messages = self.__storage_manager.get_messages(client_id)
        self.__storage_manager.remove_messages(messages)

        response_code = RESPONSE_CODE_LIST_OF_MESSAGES
        response_payload_map = {
            RESPONSE_PAYLOAD_MESSAGES: messages
        }

        return response_code, response_payload_map
