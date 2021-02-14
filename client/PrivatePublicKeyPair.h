/*
 * PrivatePublicKeyPair.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef PRIVATEPUBLICKEYPAIR_H_
#define PRIVATEPUBLICKEYPAIR_H_

#include <string>
#include "PrivateKey.h"
#include "PublicKey.h"

class PrivatePublicKeyPair {
public:
	PrivatePublicKeyPair(PrivateKey privateKey, PublicKey publicKey);
	PrivatePublicKeyPair(std::string str);
	virtual ~PrivatePublicKeyPair();
	PrivateKey getPrivateKey();
	PublicKey getPublicKey();
	std::string toString();
};

#endif /* PRIVATEPUBLICKEYPAIR_H_ */
