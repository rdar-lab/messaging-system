/*
 * EncryptionUtils.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef ENCRYPTIONUTILS_H_
#define ENCRYPTIONUTILS_H_

#include "PrivatePublicKeyPair.h"
#include "SymmetricKey.h"

class EncryptionUtils {
public:
	static PrivatePublicKeyPair generateKeypair(unsigned int algorithm);
	static SymmetricKey generateSymmetricKey(unsigned int algorithm);

	static unsigned int pkiEncrypt(
			unsigned short algorithm,
			PublicKey publicKey,
			void *source,
			unsigned int sourceLen,
			void *destination
	);

	static unsigned int pkiDecrypt(
			int algorithm,
			PrivateKey privateKey,
			void *source,
			unsigned int sourceLen,
			void *destination
	);

	static unsigned int encryptDecrypt(
			unsigned short algorithm,
			SymmetricKey key,
			void *source,
			unsigned int sourceLen,
			void *destination
	);


private:
	EncryptionUtils();
	virtual ~EncryptionUtils();

};

#endif /* ENCRYPTIONUTILS_H_ */
