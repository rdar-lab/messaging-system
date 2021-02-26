/*
 * ClientDatastore.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef CLIENTDATASTORE_H_
#define CLIENTDATASTORE_H_

#include "ClientDef.h"
#include "ClientId.h"
#include <map>
#include <list>

/*
 * A Datastore for the client.
 * The datastore is responsible for holding all the clients and their information
 * This is an abstract/interface of the datastore
 *
 */
class ClientDatastore {
public:
	static ClientDatastore* getInstance();
	static void setInstance(ClientDatastore* instance);
	virtual std::list<ClientDef> getAllClients() = 0 ;
	virtual void clearClients() = 0;
	virtual void addClient(ClientDef client) = 0;
	virtual ClientDef removeClient(ClientId clientId) = 0;
	virtual ClientDef getClient(ClientId clientId) = 0;
	virtual int clientExists(ClientId clientId) = 0;
	virtual int clientExists(std::string name) = 0;
	virtual ClientDef getClientByName(std::string name) = 0;
	virtual void updateClient(ClientDef client) = 0;
	void printClientsList();

	virtual ~ClientDatastore();
protected:
	ClientDatastore();
private:
	static ClientDatastore *singleInstance;
};

#endif /* CLIENTDATASTORE_H_ */
