/*
 * ClientMetadataReader.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */
#include <string>
#include "PrivatePublicKeyPair.h"
#include "ClientId.h"

#ifndef CLIENTMETADATAMANAGER_H_
#define CLIENTMETADATAMANAGER_H_

/*
 * The metadata manager is responsible for reading and saving this client information.
 * The information it holds: The ID, name, and the PKI pair
 */
class ClientMetadataManager {
public:
	static ClientMetadataManager* getInstance();
	void update(std::string clientName, ClientId clientId, PrivatePublicKeyPair pkiKey);
	std::string getClientName();
	ClientId getClientId();
	PrivatePublicKeyPair getPrivatePublicKeyPair();
	int isRegistered();

	virtual ~ClientMetadataManager();
private:
	ClientMetadataManager();
	void readData();
	std::string clientName;
	ClientId clientId;
	bool isRegistred;
	PrivatePublicKeyPair privatePublicKeyPair;
	static ClientMetadataManager *singleInstance;
	friend class TestingManager;
};

#endif /* CLIENTMETADATAMANAGER_H_ */
