/*
 * ClientActionInvoker.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientActionInvoker.h"
#include <iostream>
#include <cstring>
#include <string>
#include "EncryptionUtils.h"
#include "Request.h"
#include "Response.h"
#include "Constants.h"
#include "CommunicationManager.h"
#include "BinaryByteBuffer.h"
#include "ClientDef.h"
#include "ClientDatastore.h"
#include "Utils.h"

ClientActionInvoker::ClientActionInvoker() {

}

ClientActionInvoker::~ClientActionInvoker() {
}

void ClientActionInvoker::performRegister(){
	if (ClientMetadataManager::getInstance()->isRegistered()){
		std::cout << "ERROR: Client already registered" << std::endl;
		return;
	}

	std::string clientName;

	std::cout << "Enter client name: ";
	std::cin >> clientName;

	if (clientName.length()>STR_SIZE-1){
		throw std::runtime_error("Client name overflow");
	}

	PrivatePublicKeyPair keyPair = EncryptionUtils::generateKeypair(ENCRYPTION_ALGORITHM_RSA);
	PublicKey publicKey = keyPair.getPublicKey();

	unsigned char payload[STR_SIZE+PUBLIC_KEY_SIZE];
	std::memset(payload, 0, sizeof(payload));
	std::strncpy((char *)payload, clientName.c_str(), STR_SIZE-1);
	publicKey.write(&payload[STR_SIZE]);

	ClientId clientId;
	ByteBuffer *payloadBuffer = new BinaryByteBuffer(payload, STR_SIZE+PUBLIC_KEY_SIZE);
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_REGISTER, payloadBuffer);
	Response* resp = CommunicationManager::getInstance()->sendRequest(&req);
	try{
		if (resp->getResponseCode() == RESPONSE_CODE_REG_SUCCESS)
		{
			std::cout << "* Registration successful" << std::endl;
			unsigned char clientIdBuffer[CLIENT_ID_SIZE];
			resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
			ClientMetadataManager::getInstance()->update(clientName, ClientId(clientIdBuffer), keyPair);
			std::cout << "Client ID: " << ClientMetadataManager::getInstance()->getClientId() << std::endl;
		} else {
			std::cout << "ERR: Got unexpected response code: " << resp->getResponseCode() << std::endl;
		}
	} catch (std::exception &exp)
	{
		std::cout << "ERR:" << exp.what() << std::endl;
	}
	delete resp;
}


void ClientActionInvoker::performGetClientsList(){
	ClientId clientId = ClientMetadataManager::getInstance()->getClientId();
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_CLIENTS_LIST, NULL);
	Response* resp = CommunicationManager::getInstance()->sendRequest(&req);
	try{
		if (resp->getResponseCode() == RESPONSE_CODE_CLIENTS_LIST)
		{
			if (resp->getPayload()!=NULL){
				while (resp->getPayload()->getBytesLeft()>0)
				{
					unsigned char clientIdBuffer[CLIENT_ID_SIZE];
					resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
					ClientId clientId(clientIdBuffer);
					char nameBuff[STR_SIZE];
					resp->getPayload()->readData(nameBuff, STR_SIZE);
					std::string name(nameBuff);
					ClientDef client(clientId,  name);
					if (!ClientDatastore::getInstance()->clientExists(clientId)){
						ClientDatastore::getInstance()->addClient(client);
					}
				}
			}
			ClientDatastore::getInstance()->printClientsList();

		} else {
			std::cout << "ERR: Got unexpected response code: " << resp->getResponseCode() << std::endl;
		}
	} catch (std::exception &exp)
	{
		std::cout << "ERR:" << exp.what() << std::endl;
	}
	delete resp;
}

ClientDef ClientActionInvoker::promptAndGetClient(){
	std::string clientName;
	ClientDatastore::getInstance()->printClientsList();
	std::cout << "Enter client name: ";
	std::cin >> clientName;

	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	return ClientDatastore::getInstance()->getClientByName(clientName);
}


void ClientActionInvoker::performGetClientPublicKey(){
	ClientDef client = promptAndGetClient();
	if (!client.getPublicKey().isEmpty()){
		std::cout << "ERR: client public key already loaded" << std::endl;
		return;
	}

	ClientId clientId = client.getId();
	unsigned char payload[CLIENT_ID_SIZE];
	std::memset(payload, 0, sizeof(payload));
	clientId.write(payload);

	ByteBuffer *payloadBuffer = new BinaryByteBuffer(payload, CLIENT_ID_SIZE);
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_CLIENT_PUBLIC_KEY, payloadBuffer);
	Response* resp = CommunicationManager::getInstance()->sendRequest(&req);
	try{
		if (resp->getResponseCode() == RESPONSE_CODE_CLIENT_PUBLIC_KEY)
		{
			unsigned char clientIdBuffer[CLIENT_ID_SIZE];
			resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
			ClientId responseClientId(clientIdBuffer);

			if (responseClientId != clientId)
			{
				std::cout << "ERR: Response client ID does not match the request" << std::endl;
			} else
			{
				unsigned char publicKeyBuffer[PUBLIC_KEY_SIZE];
				resp->getPayload()->readData(publicKeyBuffer, PUBLIC_KEY_SIZE);
				PublicKey publicKey(publicKeyBuffer);
				client.setPublicKey(publicKey);
				ClientDatastore::getInstance()->updateClient(client);
				std::cout << "Public key loaded" << std::endl;
			}
		} else
		{
			std::cout << "ERR: Got unexpected response code: " << resp->getResponseCode() << std::endl;
		}
	} catch (std::exception &exp)
	{
		std::cout << "ERR:" << exp.what() << std::endl;
	}
	delete resp;
}

void ClientActionInvoker::performGetMessages(){
	ClientId clientId = ClientMetadataManager::getInstance()->getClientId();
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_MESSAGES, NULL);
	Response* resp = CommunicationManager::getInstance()->sendRequest(&req);
	try{
		if (resp->getResponseCode() == RESPONSE_CODE_LIST_OF_MESSAGES)
		{
			if (resp->getPayload()!=NULL){
				while (resp->getPayload()->getBytesLeft()>0)
				{
					unsigned char clientIdBuffer[CLIENT_ID_SIZE];
					resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
					ClientId messageClientId(clientIdBuffer);

					ClientDef messageFromClient;
					if (!ClientDatastore::getInstance()->clientExists(messageClientId)){
						std::cout << "ERR: Got unknown message client id: " << messageClientId << std::endl;
						messageFromClient = ClientDef(messageClientId, "--UNKNOWN--");
					} else {
						messageFromClient = ClientDatastore::getInstance()->getClient(messageClientId);
					}

					std::cout << "From: " << messageFromClient.getName() << std::endl;

					unsigned char messageIdBuffer[MESSAGE_ID_SIZE];
					resp->getPayload()->readData(messageIdBuffer, MESSAGE_ID_SIZE);
					unsigned int messageId = Utils::convertToNum(messageIdBuffer, MESSAGE_ID_SIZE);

					std::cout << "Msg ID: " << messageId << std::endl;

					unsigned char messageType = resp->getPayload()->readByte();

					unsigned char messageLenBuffer[MESSAGE_LENGTH_SIZE];
					resp->getPayload()->readData(messageLenBuffer, MESSAGE_LENGTH_SIZE);
					unsigned int messageLen = Utils::convertToNum(messageLenBuffer, MESSAGE_LENGTH_SIZE);

					if (messageLen > MAX_MSG_BODY_BYTES)
					{
						throw std::runtime_error("Max message len exceeded");
					}

					std::cout << "Content:" << std::endl;

					switch (messageType){
					case MESSAGE_TYPE_ENC_KEY_REQUEST:
						{
							if (messageLen > 0)
							{
								throw std::runtime_error("Message len incorrect. expected length of 0");
							}

							std::cout << "Request for symmetric key" << std::endl;
						}
						break;
					case MESSAGE_TYPE_ENC_KEY_RESP:
						{
							if (messageLen != MESSAGE_LENGTH_SIZE)
							{
								throw std::runtime_error("Message len incorrect. expected SYMMETRIC_KEY_SIZE");
							}

							if (messageFromClient.getSymmetricKey().isEmpty())
							{
								unsigned char encryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
								unsigned char decryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
								resp->getPayload()->readData(encryptedKeyBuffer, SYMMETRIC_KEY_SIZE);
								EncryptionUtils::pkiDecrypt(
										ENCRYPTION_ALGORITHM_RSA,
										ClientMetadataManager::getInstance()->getPrivatePublicKeyPair().getPrivateKey(),
										encryptedKeyBuffer,
										MESSAGE_LENGTH_SIZE,
										decryptedKeyBuffer);

								SymmetricKey key(decryptedKeyBuffer);
								messageFromClient.setSymmetricKey(key);
								ClientDatastore::getInstance()->updateClient(messageFromClient);

								std::cout << "Symmetric key received" << std::endl;
							}
							else
							{
								std::cout << "Symmetric key received, but ignored (already exists for client)" << std::endl;
							}
						}
						break;
					case MESSAGE_TYPE_TEXT_MESSAGE:
						{
							char messageBuffer[messageLen];
							resp->getPayload()->readData(messageBuffer, messageLen);
							std::string msgText(messageBuffer);
							std::cout << msgText << std::endl;
						}
						break;
					case MESSAGE_TYPE_FILE_MESSAGE:
						// TODO: implement me
						{

						}

						break;
					default:
						std::cout << "ERROR: message type unknown: " << messageType << std::endl;
					}
					std::cout << "----<EOM>-----" << std::endl;
				}
			} else {
				std::cout << "---- INBOX EMPTY -----" << std::endl;
			}
		} else {
			std::cout << "ERR: Got unexpected response code: " << resp->getResponseCode() << std::endl;
		}
	} catch (std::exception &exp)
	{
		std::cout << "ERR:" << exp.what() << std::endl;
	}
	delete resp;
}

void ClientActionInvoker::sendMessage(ClientDef destinationClient, unsigned char messageType, void *messageContent, unsigned int messageLen){
	unsigned int payloadLen = messageLen + CLIENT_ID_SIZE + MESSAGE_ID_SIZE + 1;
	unsigned char payload[payloadLen];
	std::memset(payload, 0, sizeof(payload));
	destinationClient.getId().write(payload);
	payload[CLIENT_ID_SIZE] = messageType;
	Utils::convertToBytes(messageLen, payload+CLIENT_ID_SIZE+1, MESSAGE_LENGTH_SIZE);
	std::copy((char*)messageContent, (char*)messageContent+messageLen, (char*)payload+CLIENT_ID_SIZE+MESSAGE_LENGTH_SIZE+1);

	ByteBuffer *payloadBuffer = new BinaryByteBuffer(payload, payloadLen);
	Request req(
			ClientMetadataManager::getInstance()->getClientId(),
			CLIENT_VERSION,
			REQUEST_CODE_SEND_MESSAGE,
			payloadBuffer);
	Response* resp = CommunicationManager::getInstance()->sendRequest(&req);
	if (resp->getResponseCode() == RESPONSE_CODE_MESSAGE_SENT)
	{
		std::cout << "Message sent..." << std::endl;
	} else
	{
		std::cout << "Message sending ERR. invalid response code: " << resp->getResponseCode() << std::endl;
	}
	delete resp;

}

void ClientActionInvoker::performSendTextMessage(){
	ClientDef client = promptAndGetClient();
	if (client.getSymmetricKey().isEmpty()){
		std::cout << "ERR: client symmetric key not loaded. First ask for key" << std::endl;
		return;
	}

	std::string messageText;
	std::cout << "Enter message: ";
	std::cin >> messageText;

	unsigned char encryptedMessageBuffer[messageText.length()+EXTRA_MESSAGE_BUFFER];
	std::memset(encryptedMessageBuffer, 0, sizeof(encryptedMessageBuffer));
	unsigned int encryptedTextLen = EncryptionUtils::encryptDecrypt(
			ENCRYPTION_ALGORITHM_AES,
			client.getSymmetricKey(),
			(void*)messageText.c_str(),
			messageText.length()+1,
			encryptedMessageBuffer);

	sendMessage(client, MESSAGE_TYPE_TEXT_MESSAGE, encryptedMessageBuffer, encryptedTextLen);
}

void ClientActionInvoker::performSendRequestForSymmetricKey(){
	ClientDef client = promptAndGetClient();

	if (client.getPublicKey().isEmpty()){
		std::cout << "ERR: client public key not loaded. Request it first" << std::endl;
		return;
	}


	if (!client.getSymmetricKey().isEmpty()){
		std::cout << "ERR: client symmetric key already loaded" << std::endl;
		return;
	}

	sendMessage(client, MESSAGE_TYPE_ENC_KEY_REQUEST, NULL, 0);
}

void ClientActionInvoker::performSendSymmetricKey(){
	ClientDef client = promptAndGetClient();
	if (client.getPublicKey().isEmpty()){
		std::cout << "ERR: client public key not loaded. Request it first" << std::endl;
		return;
	}

	if (client.getSymmetricKey().isEmpty()){
		SymmetricKey key = EncryptionUtils::generateSymmetricKey(ENCRYPTION_ALGORITHM_AES);
		client.setSymmetricKey(key);
		ClientDatastore::getInstance()->updateClient(client);
	}

	unsigned char decryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
	client.getSymmetricKey().write(decryptedKeyBuffer);
	unsigned char encryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
	unsigned int encrypterdMsgLen = EncryptionUtils::pkiEncrypt(
			ENCRYPTION_ALGORITHM_RSA,
			client.getPublicKey(),
			decryptedKeyBuffer,
			SYMMETRIC_KEY_SIZE,
			encryptedKeyBuffer);

	sendMessage(client, MESSAGE_TYPE_ENC_KEY_RESP, encryptedKeyBuffer, encrypterdMsgLen);
}

int ClientActionInvoker::performAction(int selectedOption){
	if (selectedOption == 0){
		return 1;
	}

	if (selectedOption == 1){
		performRegister();
		return 0;
	}

	if (!ClientMetadataManager::getInstance()->isRegistered()){
		std::cout << "ERROR: Client not registered" << std::endl;
		return 0;
	}

	switch(selectedOption){
	case 2:
		performGetClientsList();
		break;
	case 3:
		performGetClientPublicKey();
		break;
	case 4:
		performGetMessages();
		break;
	case 5:
		performSendTextMessage();
		break;
	case 51:
		performSendRequestForSymmetricKey();
		break;
	case 52:
		performSendSymmetricKey();
		break;
	default:
		std::cout << "Error: invalid option selected: " << selectedOption;
	}
	return false;
}
