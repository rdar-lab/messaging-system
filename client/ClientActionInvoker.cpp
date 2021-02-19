/*
 * ClientActionInvoker.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientActionInvoker.h"
#include "ClientLogicHandler.h"
#include "ClientDatastore.h"

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

	ClientLogicHandler().performRegister(clientName);

	std::cout << "* Registration Successful" << std::endl;
	std::cout << "Client ID: " << ClientMetadataManager::getInstance()->getClientId() << std::endl;
}


void ClientActionInvoker::performGetClientsList(){
	ClientLogicHandler().performGetClientsList();
	ClientDatastore::getInstance()->printClientsList();
}

std::string ClientActionInvoker::promptAndGetClient(){
	std::string clientName;
	ClientDatastore::getInstance()->printClientsList();
	std::cout << "Enter client name: ";
	std::cin >> clientName;

	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	return clientName;
}


void ClientActionInvoker::performGetClientPublicKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performGetClientPublicKey(clientName);
	std::cout << "Public key loaded" << std::endl;
}

void ClientActionInvoker::performGetMessages(){
	std::list<Message> messages = ClientLogicHandler().performGetMessages();
	if (messages.size()>0)
	{
		for (auto it=messages.begin(); it!=messages.end(); it++){
			std::cout << "From: " << it->getFromClient().getName() << std::endl;
			std::cout << "Msg ID: " << it->getMessageId() << std::endl;
			std::cout << "Content:" << std::endl;
			std::cout << it->getMessageContentTxt() << std::endl;
			std::cout << "----<EOM>-----" << std::endl;
		}
	} else
	{
		std::cout << "---- INBOX EMPTY -----" << std::endl;
	}
}

void ClientActionInvoker::performSendTextMessage(){
	std::string clientName = promptAndGetClient();

	std::string messageText;
	std::cout << "Enter message: ";
	std::cin >> messageText;

	ClientLogicHandler().performSendTextMessage(clientName, messageText);
	std::cout << "Message sent" << std::endl;
}

void ClientActionInvoker::performSendRequestForSymmetricKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performSendRequestForSymmetricKey(clientName);
	std::cout << "Request sent" << std::endl;
}

void ClientActionInvoker::performSendSymmetricKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performSendSymmetricKey(clientName);
	std::cout << "Key sent" << std::endl;
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
