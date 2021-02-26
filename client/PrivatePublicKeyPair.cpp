/*
 * PrivatePublicKeyPair.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#include "PrivatePublicKeyPair.h"
#include "EncryptionUtils.h"

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
	unsigned char privateKeyBuff[PRIVATE_KEY_SIZE];
	Utils::base64Decode(str, privateKeyBuff, PRIVATE_KEY_SIZE);
	this->privateKey = PrivateKey(privateKeyBuff);
	this->publicKey = EncryptionUtils::generatePublicFromPrivate(ENCRYPTION_ALGORITHM_RSA, privateKey);
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
	unsigned char privateKeyBuff[PRIVATE_KEY_SIZE];
	this->privateKey.write(privateKeyBuff);
	return Utils::base64Encode(privateKeyBuff, PRIVATE_KEY_SIZE);
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
