/*
 * ClientDatastore.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientDatastore.h"
#include <string>
#include <iostream>

ClientDatastore::ClientDatastore() {

}

ClientDatastore::~ClientDatastore() {
}

ClientDatastore *ClientDatastore::singleInstance = NULL;

ClientDatastore* ClientDatastore::getInstance()
{
	return ClientDatastore::singleInstance;
}

void ClientDatastore::setInstance(ClientDatastore *instance) {
	ClientDatastore::singleInstance = instance;
}


void ClientDatastore::printClientsList() {
	std::list<ClientDef> clientsList = this->getAllClients();
	std::cout << "* Clients:" << std::endl;
	for (std::list<ClientDef>::const_iterator it=clientsList.begin(); it!=clientsList.end(); ++it) {
		ClientDef client = *it;
		std::cout << "\t" << client << std::endl;
	}
}
