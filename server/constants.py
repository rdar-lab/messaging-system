SERVER_VERSION = 1

MAX_PAYLOAD_BYTES = 1024 * 1024 * 1024
MAX_MSG_BODY_BYTES = 1024 * 1024 * 1024

BUFFER_SIZE = 1024
CONNECTIONS_BACKLOG = 100

REQUEST_CODE_REGISTER = 100
REQUEST_CODE_GET_CLIENTS_LIST = 101
REQUEST_CODE_GET_CLIENT_PUBLIC_KEY = 102
REQUEST_CODE_SEND_MESSAGE = 103
REQUEST_CODE_GET_MESSAGES = 104

MESSAGE_TYPE_ENC_KEY_REQUEST = 1
MESSAGE_TYPE_ENC_KEY_RESP = 2
MESSAGE_TYPE_TEXT_MESSAGE = 3
MESSAGE_TYPE_FILE_MESSAGE = 4

RESPONSE_CODE_REG_SUCCESS = 1000
RESPONSE_CODE_CLIENTS_LIST = 1001
RESPONSE_CODE_CLIENT_PUBLIC_KEY = 1002
RESPONSE_CODE_MESSAGE_SENT = 1003
RESPONSE_CODE_LIST_OF_MESSAGES = 1004

RESPONSE_CODE_GENERAL_ERROR = 9000

REQUEST_PAYLOAD_CLIENT_NAME = "CLIENT_NAME"
REQUEST_PAYLOAD_CLIENT_ID = "CLIENT_ID"
REQUEST_PAYLOAD_PUBLIC_KEY = "PUBLIC_KEY"
REQUEST_PAYLOAD_MESSAGE_TYPE = "MESSAGE_TYPE"
REQUEST_PAYLOAD_MESSAGE_BODY = "MESSAGE_BODY"

RESPONSE_PAYLOAD_CLIENT_ID = "CLIENT_ID"
RESPONSE_PAYLOAD_CLIENTS = "CLIENTS"
RESPONSE_PAYLOAD_PUBLIC_KEY = "PUBLIC_KEY"
RESPONSE_PAYLOAD_MESSAGE_ID = "MESSAGE_ID"
RESPONSE_PAYLOAD_MESSAGES = "MESSAGES"

STR_SIZE = 255
PUBLIC_KEY_SIZE = 160
CLIENT_ID_SIZE = 16
MESSAGE_ID_SIZE = 4
MESSAGE_LENGTH_SIZE = 4
SYMMETRIC_KEY_SIZE = 16
MESSAGE_TYPE_SIZE = 1
