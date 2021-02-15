/*
 * PrivatePublicKeyPair.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "PrivatePublicKeyPair.h"

PrivatePublicKeyPair::~PrivatePublicKeyPair() {
}

PrivatePublicKeyPair::PrivatePublicKeyPair() {
	this->privateKey = PrivateKey();
	this->publicKey = PublicKey();
}

PrivatePublicKeyPair::PrivatePublicKeyPair(PrivateKey privateKey,
		PublicKey publicKey) {
	this->privateKey = privateKey;
	this->publicKey = publicKey;
}

PrivatePublicKeyPair::PrivatePublicKeyPair(std::string str) {
	std::string privateKeyStr = str.substr(0, str.find(":"));
	std::string publicKeyStr = str.substr(str.find(":")+1, str.length());
	this->privateKey = PrivateKey(privateKeyStr);
	this->publicKey = PublicKey(publicKeyStr);
}

PrivatePublicKeyPair::PrivatePublicKeyPair(const PrivatePublicKeyPair &other) {
	this->privateKey = other.privateKey;
	this->publicKey = other.publicKey;
}

PrivateKey PrivatePublicKeyPair::getPrivateKey() const {
	return privateKey;
}

PublicKey PrivatePublicKeyPair::getPublicKey() const {
	return publicKey;
}

std::string PrivatePublicKeyPair::toString() {
	return this->privateKey.toString() + ":" + this->publicKey.toString();
}

PrivatePublicKeyPair& PrivatePublicKeyPair::operator =(
		const PrivatePublicKeyPair &other) {
	if (this == &other){
		return *this;
	}

	this->privateKey = other.privateKey;
	this->publicKey = other.publicKey;

	return *this;
}
