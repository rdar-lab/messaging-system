//============================================================================
// Name        : client.cpp
// Author      : Roy Dar
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ConfigurationFileReader.h"
#include "Constants.h"
#include "CommunicationManager.h"
#include "InteractiveMenuManager.h"
#include "ClientMetadataManager.h"
#include "ClientDatastore.h"
#include "InMemClientDatastore.h"
#include "ClientActionInvoker.h"

using namespace std;

int main() {
	try
		{
			ConfigurationFileReader serverConfigReader(SERVER_CONFIGURATION_FILE_LOCATION);
			std::string host = serverConfigReader.getServerHost();
			unsigned short port = serverConfigReader.getServerPort();

			std::cout << "Server configuration: " << host << ":" << port << std::endl;
			CommunicationManager::getInstance()->setParams(host, port);

			InMemClientDatastore inMemDatastore;
			ClientDatastore::setInstance(&inMemDatastore);

			InteractiveMenuManager menuManger;
			ClientActionInvoker invoker;
			int exitInd = false;

			while (!exitInd){
					int selectedOption = menuManger.showMenuAndGetUserAction();
					try{
						exitInd = invoker.performAction(selectedOption);
					}
					catch (std::exception &exp){
						std::cout << "error: " << exp.what() << std::endl;
					}
			}
		} catch (std::exception &exp)
		{
			std::cout << "error: " << exp.what() << std::endl;
		}
		return 0;}
