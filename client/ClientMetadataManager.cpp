/*
 * ClientMetadataReader.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientMetadataManager.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

ClientMetadataManager::ClientMetadataManager() {
	this->isRegistred = false;

	std::ifstream clientMetadataFile(CLIENT_CONFIGURATION_FILE_LOCATION);
	if (clientMetadataFile.is_open())
	{
		try
		{
			std::string line;
			getline(clientMetadataFile, line);
			this->clientName = line;

			getline(clientMetadataFile, line);
			this->clientId = ClientId(line);

			getline(clientMetadataFile, line);
			this->privatePublicKeyPair = PrivatePublicKeyPair(line);

			this->isRegistred = true;

		} catch (std::exception &exp)
		{
			std::cout << "ERR: Error while reading client metadata file: " << exp.what() << std::endl;
		}

		clientMetadataFile.close();
	}

	if (!this->isRegistred)
	{
		this->clientName = "";
		this->clientId = ClientId();
		this->privatePublicKeyPair = PrivatePublicKeyPair();
	}

}

ClientMetadataManager::~ClientMetadataManager() {
}


ClientMetadataManager *ClientMetadataManager::singleInstance = NULL;

ClientMetadataManager* ClientMetadataManager::getInstance()
{
	// Check if the instance is initialized, and if not create it
	if (ClientMetadataManager::singleInstance == NULL)
	{
		ClientMetadataManager::singleInstance = new ClientMetadataManager();
	}
	return ClientMetadataManager::singleInstance;
}


void ClientMetadataManager::update(std::string clientName, ClientId clientId,
		PrivatePublicKeyPair pkiKey) {

	bool fileWrote = false;
	std::ofstream clientMetadataFile(CLIENT_CONFIGURATION_FILE_LOCATION);
	if (clientMetadataFile.is_open())
	{
		try
		{
			clientMetadataFile << clientName << std::endl;
			clientMetadataFile << clientId << std::endl;
			clientMetadataFile << pkiKey.toString() << std::endl;
			fileWrote = true;
		} catch (std::exception &exp)
		{
			std::cout << "ERR: Error while writing client metadata file: " << exp.what() << std::endl;
		}

		clientMetadataFile.close();
	} else {
		std::cout << "ERR: Error while writing client metadata file" << std::endl;
	}

	if (fileWrote)
	{
		this->clientName = clientName;
		this->clientId = clientId;
		this->privatePublicKeyPair = pkiKey;
		this->isRegistred = true;
	}

}

std::string ClientMetadataManager::getClientName() {
	return clientName;
}

ClientId ClientMetadataManager::getClientId() {
	return clientId;
}

PrivatePublicKeyPair ClientMetadataManager::getPrivatePublicKeyPair() {
	return privatePublicKeyPair;
}

int ClientMetadataManager::isRegistered() {
	return isRegistred;
}
