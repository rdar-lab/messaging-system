# Messaging-system with E2E encryption

This project is a simple tcp client-server messaging system with E2E encryption support.
The server is implemented with python, and the client is implemented with C++ with Boost and Crypto++.

## General architecture

This is a simple implementation of a client-server TCP based messaging system with end-2-end encryption of messages.
Each client has a PKI pair, and will negotiate a symmetric key with each additional client it wish to communicate with.
This implementation also fully supports files (including big files).

![Alt text](resources/highlevel.png?raw=true "High level")

## Communication spec

![Alt text](resources/message-exchange.png?raw=true "High level")

### Request format

The request has the following format:

- Client ID - 16 bytes - the client ID
- Version - 1 byte - the version of the client
- Request code - 1 byte - the request code
- Payload size - 4 bytes - the size of the payload
- Payload - dynamic size - the payload data

### Response format

The response has the following format:

- Version - 1 byte - the version of the server
- Response code - 2 bytes - the server response code
- Payload size - 4 bytes - the size of the payload
- Payload - dynamic size - the payload data

### Requests and responses

#### Register request
Code = 100
Payload = Name(255 bytes), Public key(160 bytes)

Success response :
Code = 1000
Payload = Client ID(16 bytes)

#### Get client list request
Code = 101
Payload = NONE

Success response:
Code = 1001
Payload = For each client: Client ID(16 bytes), Client name (255 bytes)

#### Get public key request
Code = 102
Payload = Client ID(16 bytes)

Success response:
Code = 1002
Payload = Client ID(16 bytes), Public key (160 bytes)

#### Send message request
Code = 103
Payload = Client ID(16 bytes), Msg type(1 byte), Msg size(4 bytes), Msg content(Dynamic)

Message types:
1. Public key request
2. Public key
3. Text message
4. File message

Success response:
Code = 1003
Payload = Client ID(16 bytes), Msg ID (4 bytes)

#### Get messages request

Code = 104
Payload = NONE

Success response:
Code = 1004
Payload = For each message: Client ID(16 bytes), Msg ID(4 bytes), Msg type(1 byte), Msg size(4 bytes), Msg content(Dynamic)

#### Failure response
Code = 9000

## The server

### Main logical units

Name | Responsibility
--- | ---
server_main.py | The main entry point for the server
bytes_buffer.py | A set of classes to implement large size capable bits stream from various sources
model.py | Implementation of basic model classes
storage_manager.py | Storage management which is responsible for storing the clients and messages of the messaging system
db_storage_manager.py | An SQLite implementation of the StorageManager class
request_handler.py | Logic handler for the server. responsible for processing the requests and returning the responses
request_reader.py, response_writer.py, request_payload_parser.py, response_payload_writer.py | Reading and writing requests and responses


### How to run the server

The server requires python version 3.x to run.

To run the server:
1. Create a file named "port.info" with the port number information
2. Run the command "python server_main.py"

On the first run the SQLite DB will be created by the name "server.db".


## The client

### Main classes

Name | Responsibility
--- | ---
MessagingSystemClient | The main entry point for the client application
TestingManager | Running the test suite
ByteBuffer, BinaryByteBuffer, FileByteBuffer, SocketByteBuffer | A set of classes to implement large size capable bits stream from various sources
ClientLogicHandler | Responsible for the implementation of the client actions
InteractiveMenuManager | Responsible for showing the menu and running the user selected commands
Request, RequestReader, Response, ResponseWriter | Set of classes for request and response and reading and writing them from the socket
EncryptionUtils | Facade implementation for all encryption related operations in this project, e.g. generation of keys, and decryption and encryption 

### How to build the client

#### Windows

Using Visual studio you will need to set up Boost and Crypto++ and then compile the code.

#### Linux

The client was tested on Linux x64 / Ubuntu OS.

Using eclipse, set up Boost and Crypto++, and compile the project

### How to run the client

To run the automatic tests for the client, simply run it with the "test" argument.

To run the client normally:
1. Create a file named "server.info" on the running directory with the HOST:PORT information of the server
2. Run the "client" command
3. Register yourself to start working
