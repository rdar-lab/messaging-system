/*
 * EncryptionUtils.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "EncryptionUtils.h"
#include <cstring>
#include <string>

EncryptionUtils::EncryptionUtils() {

}

EncryptionUtils::~EncryptionUtils() {
}

PrivatePublicKeyPair EncryptionUtils::generateKeypair(unsigned int algorithm) {
	// TODO: implement this
	return PrivatePublicKeyPair();
}

SymmetricKey EncryptionUtils::generateSymmetricKey(unsigned int algorithm) {
	// TODO: implement this
	return SymmetricKey();
}

unsigned int EncryptionUtils::pkiEncrypt(unsigned short algorithm,
		PublicKey publicKey, void *source, unsigned int sourceLen,
		void *destination) {
	// TODO: implement this
	std::copy((char*)source, (char*)source+sourceLen, (char*)destination);
	return sourceLen;
}

unsigned int EncryptionUtils::pkiDecrypt(int algorithm, PrivateKey privateKey,
		void *source, unsigned int sourceLen, void *destination) {
	// TODO: implement this
	std::copy((char*)source, (char*)source+sourceLen, (char*)destination);
	return sourceLen;
}

unsigned int EncryptionUtils::encryptDecrypt(unsigned short algorithm,
		SymmetricKey key, void *source, unsigned int sourceLen,
		void *destination) {
	// TODO: implement this
	std::copy((char*)source, (char*)source+sourceLen, (char*)destination);
	return sourceLen;
}
