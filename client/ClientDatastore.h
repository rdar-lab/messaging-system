/*
 * ClientDatastore.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef CLIENTDATASTORE_H_
#define CLIENTDATASTORE_H_

#include "ClientDef.h"
#include "ClientId.h"
#include <map>
#include <list>

class ClientDatastore {
public:
	static ClientDatastore* getInstance();
	static void setInstance(ClientDatastore* instance);
	std::list<ClientDef> getAllClients();
	void clearClients();
	void addClient(ClientDef client);
	ClientDef removeClient(ClientId clientId);
	ClientDef getClient(ClientId clientId);
	int clientExists(ClientId clientId);
	int clientExists(std::string name);
	ClientDef getClientByName(std::string name);
	void updateClient(ClientDef client);
	void printClientsList();

	virtual ~ClientDatastore();
private:
	ClientDatastore();
	static ClientDatastore *singleInstance;
	std::map<ClientId, ClientDef> clients;
};

#endif /* CLIENTDATASTORE_H_ */
