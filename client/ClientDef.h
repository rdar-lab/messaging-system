/*
 * ClientDef.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef CLIENTDEF_H_
#define CLIENTDEF_H_

#include "ClientId.h"
#include <string>
#include <iostream>
#include "PublicKey.h"
#include "SymmetricKey.h"

/*
 * A client object holds the information about each client, like ID, name, PK and SymmetricKey (after negotiation)
 */
class ClientDef {
public:
	ClientDef();
	ClientDef(ClientId clientId, std::string clientName);
	ClientDef(const ClientDef &other);
	virtual ~ClientDef();
	std::string getName() const;
	ClientId getId() const;
	PublicKey getPublicKey() const;
	void setPublicKey(PublicKey publicKey);
	SymmetricKey getSymmetricKey() const;
	void setSymmetricKey(SymmetricKey key);
	ClientDef& operator=(const ClientDef& other);
	friend std::ostream& operator <<(std::ostream &os, const ClientDef &client);
private:
	std::string name;
	ClientId id;
	PublicKey publicKey;
	SymmetricKey symmetricKey;
};

#endif /* CLIENTDEF_H_ */
