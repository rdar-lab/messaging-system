/*
 * ClientMetadataReader.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */
#include <string>
#include "PrivatePublicKeyPair.h"
#include "ClientId.h"

#ifndef CLIENTMETADATAMANAGER_H_
#define CLIENTMETADATAMANAGER_H_

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
};

#endif /* CLIENTMETADATAMANAGER_H_ */
