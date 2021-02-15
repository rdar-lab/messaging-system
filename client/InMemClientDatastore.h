/*
 * InMemClientDatastore.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef INMEMCLIENTDATASTORE_H_
#define INMEMCLIENTDATASTORE_H_

#include "ClientDatastore.h"

class InMemClientDatastore : public ClientDatastore {
public:
	InMemClientDatastore();
	virtual ~InMemClientDatastore();
	virtual ClientDef getClientByName(std::string name);
	virtual void addClient(ClientDef client);
	virtual ClientDef removeClient(ClientId clientId);
	virtual int clientExists(ClientId clientId);
	virtual void updateClient(ClientDef client);
	virtual std::list<ClientDef> getAllClients();
	virtual void clearClients();
	virtual int clientExists(std::string name);
	virtual ClientDef getClient(ClientId clientId);

private:
	std::map<ClientId, ClientDef> clients;
};

#endif /* INMEMCLIENTDATASTORE_H_ */
