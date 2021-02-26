//============================================================================
// Name        : client.cpp
// Author      : Roy Dar
// Version     :
// Copyright   : 
// Description : A messaging system client implementation written in C++
//============================================================================

#include <iostream>
#include "ConfigurationFileReader.h"
#include "Constants.h"
#include "CommunicationManager.h"
#include "InteractiveMenuManager.h"
#include "ClientMetadataManager.h"
#include "ClientDatastore.h"
#include "InMemClientDatastore.h"
#include "TestingManager.h"

using namespace std;

int main(int argv, char* argc[]) {
	try
		{
			// If there is a "test" parameter invoke all tests
			if (argv > 1 && std::string(argc[1]) == "test")
			{
				TestingManager::performTests();
				return 0;
			}

			ConfigurationFileReader serverConfigReader(SERVER_CONFIGURATION_FILE_LOCATION);
			std::string host = serverConfigReader.getServerHost();
			unsigned short port = serverConfigReader.getServerPort();

			std::cout << "Server configuration: " << host << ":" << port << std::endl;
			CommunicationManager::getInstance()->setParams(host, port);

			if (ClientMetadataManager::getInstance()->isRegistered()){
				std::cout << "Client ID: " << ClientMetadataManager::getInstance()->getClientId() << std::endl;
				std::cout << "Client name: " << ClientMetadataManager::getInstance()->getClientName() << std::endl;
				std::cout << "Client Public Key: " << ClientMetadataManager::getInstance()->getPrivatePublicKeyPair().getPublicKey() << std::endl;
			} else {
				std::cout << "* NOT REGISTERED *" << std::endl;
			}

			InMemClientDatastore inMemDatastore;
			ClientDatastore::setInstance(&inMemDatastore);

			InteractiveMenuManager menuManager;
			menuManager.goIntoInteractiveMode();

		} catch (std::exception &exp)
		{
			std::cout << "error: " << exp.what() << std::endl;
		}
		return 0;}
