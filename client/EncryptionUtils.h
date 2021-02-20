/*
 * EncryptionUtils.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef ENCRYPTIONUTILS_H_
#define ENCRYPTIONUTILS_H_

//#define ENC_DEBUG

#include "PrivatePublicKeyPair.h"
#include "SymmetricKey.h"

class EncryptionUtils {
public:
	static PrivatePublicKeyPair generateKeypair(unsigned int algorithm);
	static PublicKey generatePublicFromPrivate(unsigned int algorithm, PrivateKey privateKey);

	static SymmetricKey generateSymmetricKey(unsigned int algorithm);

	static unsigned int pkiEncrypt(
			unsigned short algorithm,
			PublicKey publicKey,
			void *source,
			unsigned int sourceLen,
			void *destination,
			unsigned int destLen
	);

	static unsigned int pkiDecrypt(
			int algorithm,
			PrivateKey privateKey,
			void *source,
			unsigned int sourceLen,
			void *destination,
			unsigned int destLen
	);

	static unsigned int symmetricEncrypt(
			unsigned short algorithm,
			SymmetricKey key,
			void *source,
			unsigned int sourceLen,
			void *destination,
			unsigned int destLen
	);

	static unsigned int symmetricDecrypt(
			unsigned short algorithm,
			SymmetricKey key,
			void *source,
			unsigned int sourceLen,
			void *destination,
			unsigned int destLen
	);

	static void symmetricEncryptFile(
			unsigned short algorithm,
			SymmetricKey key,
			std::string sourceFileName,
			std::string destFileName
	);

	static void symmetricDecryptFile(
			unsigned short algorithm,
			SymmetricKey key,
			std::string sourceFileName,
			std::string destFileName
	);

private:
	EncryptionUtils();
	virtual ~EncryptionUtils();

};

#endif /* ENCRYPTIONUTILS_H_ */
