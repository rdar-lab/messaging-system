/*
 * ClientLogicHandler.cpp
 *
 *  Created on: 18 Feb 2021
 *      Author: xmaster
 */

#include "ClientLogicHandler.h"
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
#include "ClientMetadataManager.h"
#include "Utils.h"


ClientLogicHandler::ClientLogicHandler() {
	this->resp = NULL;
}

ClientLogicHandler::~ClientLogicHandler() {
	if (this->resp != NULL)
	{
		delete this->resp;
		this->resp = NULL;
	}
}


ClientId ClientLogicHandler::performRegister(std::string clientName){
	if (ClientMetadataManager::getInstance()->isRegistered()){
		throw std::runtime_error("Client already registered");
	}

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
	this->resp = CommunicationManager::getInstance()->sendRequest(&req);
	if (this->resp->getResponseCode() == RESPONSE_CODE_REG_SUCCESS)
	{
		unsigned char clientIdBuffer[CLIENT_ID_SIZE];
		this->resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
		ClientMetadataManager::getInstance()->update(clientName, ClientId(clientIdBuffer), keyPair);
		return ClientMetadataManager::getInstance()->getClientId();
	} else {
		throw std::runtime_error("Got unexpected response code: " + std::to_string(this->resp->getResponseCode()));
	}
}


void ClientLogicHandler::performGetClientsList(){
	ClientId clientId = ClientMetadataManager::getInstance()->getClientId();
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_CLIENTS_LIST, NULL);
	this->resp = CommunicationManager::getInstance()->sendRequest(&req);
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
	} else {
		throw std::runtime_error("Got unexpected response code: " + std::to_string(resp->getResponseCode()));
	}
}

void ClientLogicHandler::performGetClientPublicKey(std::string clientName){
	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	ClientDef client = ClientDatastore::getInstance()->getClientByName(clientName);

	if (!client.getPublicKey().isEmpty()){
		throw std::runtime_error("client public key already loaded");
		return;
	}

	ClientId clientId = client.getId();
	unsigned char payload[CLIENT_ID_SIZE];
	std::memset(payload, 0, sizeof(payload));
	clientId.write(payload);

	ByteBuffer *payloadBuffer = new BinaryByteBuffer(payload, CLIENT_ID_SIZE);
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_CLIENT_PUBLIC_KEY, payloadBuffer);
	this->resp = CommunicationManager::getInstance()->sendRequest(&req);
	if (resp->getResponseCode() == RESPONSE_CODE_CLIENT_PUBLIC_KEY)
	{
		unsigned char clientIdBuffer[CLIENT_ID_SIZE];
		resp->getPayload()->readData(clientIdBuffer, CLIENT_ID_SIZE);
		ClientId responseClientId(clientIdBuffer);

		if (responseClientId != clientId)
		{
			throw std::runtime_error("Response client ID does not match the request");
		} else
		{
			unsigned char publicKeyBuffer[PUBLIC_KEY_SIZE];
			resp->getPayload()->readData(publicKeyBuffer, PUBLIC_KEY_SIZE);
			PublicKey publicKey(publicKeyBuffer);
			client.setPublicKey(publicKey);
			ClientDatastore::getInstance()->updateClient(client);
		}
	} else
	{
		throw std::runtime_error("Got unexpected response code: " + std::to_string(resp->getResponseCode()));
	}

}

std::list<Message> ClientLogicHandler::performGetMessages(){
	ClientId clientId = ClientMetadataManager::getInstance()->getClientId();
	Request req(clientId, CLIENT_VERSION, REQUEST_CODE_GET_MESSAGES, NULL);
	resp = CommunicationManager::getInstance()->sendRequest(&req);
	if (resp->getResponseCode() == RESPONSE_CODE_LIST_OF_MESSAGES)
	{
		std::list<Message> result;
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

				unsigned char messageIdBuffer[MESSAGE_ID_SIZE];
				resp->getPayload()->readData(messageIdBuffer, MESSAGE_ID_SIZE);
				unsigned int messageId = Utils::convertToNum(messageIdBuffer, MESSAGE_ID_SIZE);

				unsigned char messageType = resp->getPayload()->readByte();

				unsigned char messageLenBuffer[MESSAGE_LENGTH_SIZE];
				resp->getPayload()->readData(messageLenBuffer, MESSAGE_LENGTH_SIZE);
				unsigned int messageLen = Utils::convertToNum(messageLenBuffer, MESSAGE_LENGTH_SIZE);

				if (messageLen > MAX_MSG_BODY_BYTES)
				{
					throw std::runtime_error("Max message len exceeded");
				}

				std::string messageContentTxt;
				switch (messageType){
				case MESSAGE_TYPE_ENC_KEY_REQUEST:
					{
						if (messageLen > 0)
						{
							throw std::runtime_error("Message len incorrect. expected length of 0");
						}

						messageContentTxt = "Request for symmetric key";
					}
					break;
				case MESSAGE_TYPE_ENC_KEY_RESP:
					{
						if (messageFromClient.getSymmetricKey().isEmpty())
						{
							unsigned char encryptedKeyBuffer[BUFFER_SIZE];
							unsigned char decryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
							resp->getPayload()->readData(encryptedKeyBuffer, messageLen);
							EncryptionUtils::pkiDecrypt(
									ENCRYPTION_ALGORITHM_RSA,
									ClientMetadataManager::getInstance()->getPrivatePublicKeyPair().getPrivateKey(),
									encryptedKeyBuffer,
									messageLen,
									decryptedKeyBuffer,
									SYMMETRIC_KEY_SIZE
							);

							SymmetricKey key(decryptedKeyBuffer);
							messageFromClient.setSymmetricKey(key);
							ClientDatastore::getInstance()->updateClient(messageFromClient);

							messageContentTxt = "Symmetric key received";
						}
						else
						{
							messageContentTxt = "Symmetric key received, but ignored (already exists for client)";
						}
					}
					break;
				case MESSAGE_TYPE_TEXT_MESSAGE:
					{
						char messageBuffer[messageLen];
						resp->getPayload()->readData(messageBuffer, messageLen);

						unsigned char decryptedMessageBuffer[messageLen+EXTRA_MESSAGE_BUFFER];
						std::memset(decryptedMessageBuffer, 0, sizeof(decryptedMessageBuffer));
						EncryptionUtils::symmetricDecrypt(
								ENCRYPTION_ALGORITHM_AES,
								messageFromClient.getSymmetricKey(),
								(void*)messageBuffer,
								messageLen,
								(void*)decryptedMessageBuffer,
								messageLen+EXTRA_MESSAGE_BUFFER
						);

						std::string msgText((char*)decryptedMessageBuffer);
						messageContentTxt = msgText;
					}
					break;
				case MESSAGE_TYPE_FILE_MESSAGE:
					// TODO: implement me
					{

					}

					break;
				default:
					messageContentTxt = "ERROR: message type unknown: " + std::to_string(messageType);
				}

				result.push_back(Message(messageFromClient, messageId, messageContentTxt));
			}
		}
		return result;
	} else {
		throw std::runtime_error("Got unexpected response code: " + std::to_string(resp->getResponseCode()));
	}
}

unsigned int ClientLogicHandler::sendMessage(ClientDef destinationClient, unsigned char messageType, void *messageContent, unsigned int messageLen){
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
	this->resp = CommunicationManager::getInstance()->sendRequest(&req);
	if (resp->getResponseCode() != RESPONSE_CODE_MESSAGE_SENT)
	{
		throw std::runtime_error("Message sending ERR. invalid response code: " + std::to_string(resp->getResponseCode()));
	}
	return resp->getPayload()->readInt();

}

void ClientLogicHandler::performSendTextMessage(std::string clientName, std::string messageText){
	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	ClientDef client = ClientDatastore::getInstance()->getClientByName(clientName);
	if (client.getSymmetricKey().isEmpty()){
		throw std::runtime_error("client symmetric key not loaded. First ask for key");
	}

	unsigned char encryptedMessageBuffer[messageText.length()+EXTRA_MESSAGE_BUFFER];
	std::memset(encryptedMessageBuffer, 0, sizeof(encryptedMessageBuffer));
	unsigned int encryptedTextLen = EncryptionUtils::symmetricEncrypt(
			ENCRYPTION_ALGORITHM_AES,
			client.getSymmetricKey(),
			(void*)messageText.c_str(),
			messageText.length()+1,
			encryptedMessageBuffer,
			messageText.length()+EXTRA_MESSAGE_BUFFER
	);

	sendMessage(client, MESSAGE_TYPE_TEXT_MESSAGE, encryptedMessageBuffer, encryptedTextLen);
}

void ClientLogicHandler::performSendRequestForSymmetricKey(std::string clientName){
	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	ClientDef client = ClientDatastore::getInstance()->getClientByName(clientName);

	if (client.getPublicKey().isEmpty()){
		throw std::runtime_error("client public key not loaded. Request it first");
	}


	if (!client.getSymmetricKey().isEmpty()){
		throw std::runtime_error("client symmetric key already loaded");
	}

	sendMessage(client, MESSAGE_TYPE_ENC_KEY_REQUEST, NULL, 0);
}

void ClientLogicHandler::performSendSymmetricKey(std::string clientName){
	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	ClientDef client = ClientDatastore::getInstance()->getClientByName(clientName);

	if (client.getPublicKey().isEmpty()){
		throw std::runtime_error("client public key not loaded. Request it first");
	}

	if (client.getSymmetricKey().isEmpty()){
		SymmetricKey key = EncryptionUtils::generateSymmetricKey(ENCRYPTION_ALGORITHM_AES);
		client.setSymmetricKey(key);
		ClientDatastore::getInstance()->updateClient(client);
	}

	unsigned char decryptedKeyBuffer[SYMMETRIC_KEY_SIZE];
	client.getSymmetricKey().write(decryptedKeyBuffer);
	unsigned char encryptedKeyBuffer[BUFFER_SIZE];
	unsigned int encrypterdMsgLen = EncryptionUtils::pkiEncrypt(
			ENCRYPTION_ALGORITHM_RSA,
			client.getPublicKey(),
			decryptedKeyBuffer,
			SYMMETRIC_KEY_SIZE,
			encryptedKeyBuffer,
			BUFFER_SIZE
			);

	sendMessage(client, MESSAGE_TYPE_ENC_KEY_RESP, encryptedKeyBuffer, encrypterdMsgLen);
}

