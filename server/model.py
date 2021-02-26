from utils import auto_str


@auto_str
class Client:
    """
    A client in the messaging system
    """

    def __init__(self, client_id, client_name, client_public_key):
        self.__client_id = client_id
        self.__client_name = client_name
        self.__client_public_key = client_public_key
        self.__last_seen = None

    def get_client_id(self):
        return self.__client_id

    def get_client_name(self):
        return self.__client_name

    def get_client_public_key(self):
        return self.__client_public_key

    def get_last_seen(self):
        return self.__last_seen

    def set_last_seen(self, last_seen):
        self.__last_seen = last_seen


@auto_str
class Message:
    """
    A message in the messaging system
    """

    def __init__(self, message_id, to_client_id, from_client_id, message_type, message_content):
        self.__message_id = message_id
        self.__to_client_id = to_client_id
        self.__from_client_id = from_client_id
        self.__message_type = message_type
        self.__message_content = message_content

    def get_message_id(self):
        return self.__message_id

    def get_to_client_id(self):
        return self.__to_client_id

    def get_from_client_id(self):
        return self.__from_client_id

    def get_message_type(self):
        return self.__message_type

    def get_message_content(self):
        return self.__message_content


@auto_str
class Request:
    """
    A request in the messaging system
    """

    def __init__(self, version, client_id, request_code, payload_map):
        self.__version = version
        self.__client_id = client_id
        self.__request_code = request_code
        self.__payload_map = payload_map

    def get_version(self) -> int:
        return self.__version

    def get_client_id(self):
        return self.__client_id

    def get_request_code(self) -> int:
        return self.__request_code

    def get_payload_map(self) -> dict:
        return self.__payload_map


@auto_str
class Response:
    """
    A response in the messaging system
    """

    def __init__(self, version, response_code, payload_map):
        self.__version = version
        self.__response_code = response_code
        self.__payload_map = payload_map

    def get_version(self) -> int:
        return self.__version

    def get_response_code(self) -> int:
        return self.__response_code

    def get_payload_map(self) -> dict:
        return self.__payload_map
