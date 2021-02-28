/*
 * InteractiveMenuManager.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#include "InteractiveMenuManager.h"
#include "ClientMetadataManager.h"
#include "ClientLogicHandler.h"
#include "ClientDatastore.h"

#include <iostream>

InteractiveMenuManager::InteractiveMenuManager() {
}

InteractiveMenuManager::~InteractiveMenuManager() {
}

int InteractiveMenuManager::showMenuAndGetUserAction() {
	std::cout << "********************************" << std::endl;
	std::cout << "MessageU client at your service." << std::endl;
	std::cout << std::endl;
	std::cout << "1) Register" << std::endl;
	std::cout << "2) Request for clients list" << std::endl;
	std::cout << "3) Request for public key" << std::endl;
	std::cout << "4) Request for waiting messages" << std::endl;
	std::cout << "5) Send a text message" << std::endl;
	std::cout << "50) Send a file message" << std::endl;
	std::cout << "51) Send a request for symmetric key" << std::endl;
	std::cout << "52) Send your symmetric key" << std::endl;
	std::cout << "0) Exit client" << std::endl;
	std::cout << "? ";

	std::string selection;
	std::cin >> selection;
	return std::stoi(selection);
}

void InteractiveMenuManager::performRegister(){
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


void InteractiveMenuManager::performGetClientsList(){
	ClientLogicHandler().performGetClientsList();
	ClientDatastore::getInstance()->printClientsList();
}

std::string InteractiveMenuManager::promptAndGetClient(){
	std::string clientName;
	ClientDatastore::getInstance()->printClientsList();
	std::cout << "Enter client name: ";
	std::cin >> clientName;

	if (!ClientDatastore::getInstance()->clientExists(clientName)){
		throw std::runtime_error("client does not exist");
	}

	return clientName;
}


void InteractiveMenuManager::performGetClientPublicKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performGetClientPublicKey(clientName);
	std::cout << "Public key loaded" << std::endl;
}

void InteractiveMenuManager::performGetMessages(){
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

void InteractiveMenuManager::performSendTextMessage(){
	std::string clientName = promptAndGetClient();

	std::string messageText;
	std::cout << "Enter message: ";
	std::ws(std::cin);
	std::getline(std::cin, messageText);

	ClientLogicHandler().performSendTextMessage(clientName, messageText);
	std::cout << "Message sent" << std::endl;
}

void InteractiveMenuManager::performSendRequestForSymmetricKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performSendRequestForSymmetricKey(clientName);
	std::cout << "Request sent" << std::endl;
}

void InteractiveMenuManager::performSendFileMessage() {
	std::string clientName = promptAndGetClient();

	std::string fileName;
	std::cout << "Enter file name: ";
	std::ws(std::cin);
	std::getline(std::cin, fileName);

	ClientLogicHandler().performSendFile(clientName, fileName);
	std::cout << "File sent" << std::endl;
}

void InteractiveMenuManager::performSendSymmetricKey(){
	std::string clientName = promptAndGetClient();
	ClientLogicHandler().performSendSymmetricKey(clientName);
	std::cout << "Key sent" << std::endl;
}

int InteractiveMenuManager::performAction(int selectedOption){
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
	case 50:
		performSendFileMessage();
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

void InteractiveMenuManager::goIntoInteractiveMode() {
	int exitInd = false;

	while (!exitInd){
		try{
			int selectedOption = showMenuAndGetUserAction();
			exitInd = performAction(selectedOption);
		}
		catch (std::exception &exp){
			std::cout << "error: " << exp.what() << std::endl;
		}
	}
}
