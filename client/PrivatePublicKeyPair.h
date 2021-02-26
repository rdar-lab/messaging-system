/*
 * PrivatePublicKeyPair.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef PRIVATEPUBLICKEYPAIR_H_
#define PRIVATEPUBLICKEYPAIR_H_

#include <string>
#include "PrivateKey.h"
#include "PublicKey.h"

/*
 * A PKI key pair in the system. Holds both the private and the public key, and can read and write those from str
 */
class PrivatePublicKeyPair {
public:
	PrivatePublicKeyPair();
	PrivatePublicKeyPair(PrivateKey privateKey, PublicKey publicKey);
	PrivatePublicKeyPair(std::string str);
	PrivatePublicKeyPair(const PrivatePublicKeyPair &other);
	virtual ~PrivatePublicKeyPair();
	PrivateKey getPrivateKey() const;
	PublicKey getPublicKey() const;
	std::string toString();
	PrivatePublicKeyPair &operator =(const PrivatePublicKeyPair &other);
private:
	PrivateKey privateKey;
	PublicKey publicKey;
};

#endif /* PRIVATEPUBLICKEYPAIR_H_ */
