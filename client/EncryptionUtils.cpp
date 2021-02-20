/*
 * EncryptionUtils.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "EncryptionUtils.h"
#include <cstring>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "rsa.h"
#include "filters.h"
#include "osrng.h"
#include "simple.h"
#include "aes.h"
#include "ccm.h"
#include "filters.h"
#include "files.h"



EncryptionUtils::EncryptionUtils() {

}

EncryptionUtils::~EncryptionUtils() {
}

PrivatePublicKeyPair EncryptionUtils::generateKeypair(unsigned int algorithm) {
	#ifdef ENC_DEBUG
		std::cout << "Generating Key pain..." << std::endl;
	#endif

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, RSA_KEY_BITS);

	CryptoPP::RSA::PrivateKey privateKey( params );
	unsigned char privateKeyBytes[PRIVATE_KEY_SIZE];
	std::memset(privateKeyBytes, 0, sizeof(privateKeyBytes));
	CryptoPP::ArraySink privateSink(privateKeyBytes, PRIVATE_KEY_SIZE);
	privateKey.Save(privateSink);

	CryptoPP::RSA::PublicKey publicKey(params);
	unsigned char publicKeyBytes[PUBLIC_KEY_SIZE];
	std::memset(publicKeyBytes, 0, sizeof(publicKeyBytes));
	CryptoPP::ArraySink publicSink(publicKeyBytes, PUBLIC_KEY_SIZE);
	publicKey.Save(publicSink);

	#ifdef ENC_DEBUG
		std::cout << "PrivateKey:" << Utils::writeBytestoStr(privateKeyBytes, PRIVATE_KEY_SIZE) << std::endl;
		std::cout << "PublicKey:"  << Utils::writeBytestoStr(publicKeyBytes, PUBLIC_KEY_SIZE) << std::endl;
	#endif

	return PrivatePublicKeyPair(PrivateKey(privateKeyBytes), PublicKey(publicKeyBytes));
}

PublicKey EncryptionUtils::generatePublicFromPrivate(unsigned int algorithm,
		PrivateKey privateKey) {
	unsigned char privateKeyBytes[PRIVATE_KEY_SIZE];
	std::memset(privateKeyBytes, 0, sizeof(privateKeyBytes));
	privateKey.write(privateKeyBytes);

	#ifdef ENC_DEBUG
		std::cout << "Generating public key from private key..." << std::endl;
		std::cout << "PrivateKey:" << Utils::writeBytestoStr(privateKeyBytes, PRIVATE_KEY_SIZE) << std::endl;
	#endif

	CryptoPP::RSA::PrivateKey cpPrivateKey;
	CryptoPP::ArraySource privateSource((const unsigned char*)privateKeyBytes, PRIVATE_KEY_SIZE, true);
	cpPrivateKey.Load(privateSource);

	CryptoPP::RSA::PublicKey publicKey(cpPrivateKey);

	unsigned char publicKeyBytes[PUBLIC_KEY_SIZE];
	std::memset(publicKeyBytes, 0, sizeof(publicKeyBytes));
	CryptoPP::ArraySink publicSink(publicKeyBytes, PUBLIC_KEY_SIZE);
	publicKey.Save(publicSink);

	#ifdef ENC_DEBUG
		std::cout << "PublicKey:"  << Utils::writeBytestoStr(publicKeyBytes, PUBLIC_KEY_SIZE) << std::endl;
	#endif

	return PublicKey(publicKeyBytes);
}



SymmetricKey EncryptionUtils::generateSymmetricKey(unsigned int algorithm) {
	#ifdef ENC_DEBUG
		std::cout << "Generating symmetric key..." << std::endl;
	#endif

	CryptoPP::AutoSeededRandomPool prng;
	unsigned char key[SYMMETRIC_KEY_SIZE];
	std::memset(key, 0, sizeof(key));
	prng.GenerateBlock(key, sizeof(key));

	#ifdef ENC_DEBUG
		std::cout << "SymmetricKey:" << Utils::writeBytestoStr(key, SYMMETRIC_KEY_SIZE) << std::endl;
	#endif

	return SymmetricKey(key);
}

unsigned int EncryptionUtils::pkiEncrypt(unsigned short algorithm,
		PublicKey publicKey, void *source, unsigned int sourceLen,
		void *destination, unsigned int destLen) {

	unsigned char publicKeyBytes[PUBLIC_KEY_SIZE];
	std::memset(publicKeyBytes, 0, sizeof(publicKeyBytes));
	publicKey.write(publicKeyBytes);

	#ifdef ENC_DEBUG
		std::cout << "Performing PKI encrypt..." << std::endl;
		std::cout << "PlainText:" << Utils::writeBytestoStr((char*)source, sourceLen) << std::endl;
		std::cout << "PublicKey:" << Utils::writeBytestoStr(publicKeyBytes, PUBLIC_KEY_SIZE) << std::endl;
	#endif

	CryptoPP::RSA::PublicKey cryptoPk;
	CryptoPP::ArraySource publicSource((const unsigned char*)publicKeyBytes, PUBLIC_KEY_SIZE, true);
	cryptoPk.Load(publicSource);

	CryptoPP::RSAES_OAEP_SHA_Encryptor e(cryptoPk);

	std::memset(destination, 0, destLen);
	CryptoPP::ArraySink* destSink = new CryptoPP::ArraySink((unsigned char*)destination, destLen);

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::PK_EncryptorFilter* filter = new CryptoPP::PK_EncryptorFilter(rng, e, destSink);

	CryptoPP::ArraySource dataSource((const unsigned char*)source, sourceLen, true, filter);

	unsigned int finalLen = destSink->TotalPutLength();

	#ifdef ENC_DEBUG
		std::cout << "CipherText:" << Utils::writeBytestoStr((char*)destination, finalLen) << std::endl;
	#endif

	return finalLen;
}

unsigned int EncryptionUtils::pkiDecrypt(int algorithm, PrivateKey privateKey,
		void *source, unsigned int sourceLen, void *destination, unsigned int destLen) {
	unsigned char privateKeyBytes[PRIVATE_KEY_SIZE];
	std::memset(privateKeyBytes, 0, sizeof(privateKeyBytes));
	privateKey.write(privateKeyBytes);

	#ifdef ENC_DEBUG
		std::cout << "Performing PKI decrypt..." << std::endl;
		std::cout << "CipherText:" << Utils::writeBytestoStr((char*)source, sourceLen) << std::endl;
		std::cout << "PrivateKey:"  << Utils::writeBytestoStr(privateKeyBytes, PRIVATE_KEY_SIZE) << std::endl;
	#endif

	CryptoPP::RSA::PrivateKey cryptoPk;
	CryptoPP::ArraySource privateSource((const unsigned char*)privateKeyBytes, PRIVATE_KEY_SIZE, true);
	cryptoPk.Load(privateSource);

	CryptoPP::RSAES_OAEP_SHA_Decryptor d(cryptoPk);

	std::memset(destination, 0, destLen);
	CryptoPP::ArraySink* destSink = new CryptoPP::ArraySink((unsigned char*)destination, destLen);

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::PK_DecryptorFilter* filter = new CryptoPP::PK_DecryptorFilter(rng, d, destSink);

	CryptoPP::ArraySource dataSource((const unsigned char*)source, sourceLen, true, filter);

	unsigned int finalLen = destSink->TotalPutLength();

	#ifdef ENC_DEBUG
		std::cout << "PlainText:" << Utils::writeBytestoStr((char*)destination, finalLen) << std::endl;
	#endif

	return finalLen;
}

unsigned int EncryptionUtils::symmetricEncrypt(unsigned short algorithm,
		SymmetricKey key, void *source, unsigned int sourceLen,
		void *destination, unsigned int destLen) {
	CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;
	unsigned char keyBuffer[SYMMETRIC_KEY_SIZE];
	key.write(keyBuffer);

	#ifdef ENC_DEBUG
		std::cout << "Performing symmetric encrypt..." << std::endl;
		std::cout << "PlainText:" << Utils::writeBytestoStr((char*)source, sourceLen) << std::endl;
		std::cout << "SymmetricKey:" << Utils::writeBytestoStr(keyBuffer, SYMMETRIC_KEY_SIZE) << std::endl;
	#endif

	unsigned char iv[CryptoPP::AES::BLOCKSIZE];
	std::memset(iv, 0, sizeof(iv));

	e.SetKeyWithIV(keyBuffer, sizeof(keyBuffer), iv);

	std::memset(destination, 0, destLen);
	CryptoPP::StreamTransformationFilter filter(e);
	filter.Put((const unsigned char*)source, sourceLen);
	filter.MessageEnd();

	unsigned int finalLen = filter.Get((unsigned char*)destination,destLen);

	#ifdef ENC_DEBUG
		std::cout << "CipherText:" << Utils::writeBytestoStr((char*)destination, finalLen) << std::endl;
	#endif

	return finalLen;
}

unsigned int EncryptionUtils::symmetricDecrypt(unsigned short algorithm,
		SymmetricKey key, void *source, unsigned int sourceLen,
		void *destination, unsigned int destLen) {
	CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
	unsigned char keyBuffer[SYMMETRIC_KEY_SIZE];
	key.write(keyBuffer);

	#ifdef ENC_DEBUG
		std::cout << "Performing symmetric decrypt..." << std::endl;
		std::cout << "CipherText:" << Utils::writeBytestoStr((char*)source, sourceLen) << std::endl;
		std::cout << "SymmetricKey:" << Utils::writeBytestoStr(keyBuffer, SYMMETRIC_KEY_SIZE) << std::endl;
	#endif

	unsigned char iv[CryptoPP::AES::BLOCKSIZE];
	std::memset(iv, 0, sizeof(iv));

	d.SetKeyWithIV(keyBuffer, sizeof(keyBuffer), iv);

	std::memset(destination, 0, destLen);
	CryptoPP::StreamTransformationFilter filter(d);
	filter.Put((const unsigned char*)source, sourceLen);
	filter.MessageEnd();

	unsigned int finalLen = filter.Get((unsigned char*)destination,destLen);

	#ifdef ENC_DEBUG
		std::cout << "PlainText:" << Utils::writeBytestoStr((char*)destination, finalLen) << std::endl;
	#endif

	return finalLen;
}

void EncryptionUtils::symmetricEncryptFile(unsigned short algorithm,
		SymmetricKey key, std::string sourceFileName,
		std::string destFileName) {
	CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;
	unsigned char keyBuffer[SYMMETRIC_KEY_SIZE];
	key.write(keyBuffer);

	#ifdef ENC_DEBUG
		std::cout << "Performing symmetric encrypt file..." << std::endl;
		std::cout << "File len before: " << boost::filesystem::file_size(sourceFileName) << std::endl;
	#endif

	unsigned char iv[CryptoPP::AES::BLOCKSIZE];
	std::memset(iv, 0, sizeof(iv));

	e.SetKeyWithIV(keyBuffer, sizeof(keyBuffer), iv);

	std::ifstream in{sourceFileName, std::ios::binary};
	if (not in.good()){
		throw std::runtime_error("File not found");
	}

	std::ofstream out{destFileName, std::ios::binary};
	try{
		CryptoPP::FileSource{in, /*pumpAll=*/true,
						   new CryptoPP::StreamTransformationFilter{
							   e, new CryptoPP::FileSink{out}}};
		in.close();
		out.close();
	} catch (...){
		try{
			in.close();
			out.close();
		} catch (...){

		}
		throw;
	}

	#ifdef ENC_DEBUG
		std::cout << "File len after: " << boost::filesystem::file_size(destFileName) << std::endl;
	#endif

}

void EncryptionUtils::symmetricDecryptFile(unsigned short algorithm,
		SymmetricKey key, std::string sourceFileName,
		std::string destFileName) {
	CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
	unsigned char keyBuffer[SYMMETRIC_KEY_SIZE];
	key.write(keyBuffer);

	#ifdef ENC_DEBUG
		std::cout << "Performing symmetric decrypt file..." << std::endl;
		std::cout << "File len before: " << boost::filesystem::file_size(sourceFileName) << std::endl;
	#endif

	unsigned char iv[CryptoPP::AES::BLOCKSIZE];
	std::memset(iv, 0, sizeof(iv));

	d.SetKeyWithIV(keyBuffer, sizeof(keyBuffer), iv);

	std::ifstream in{sourceFileName, std::ios::binary};
	if (not in.good()){
		throw std::runtime_error("File not found");
	}

	std::ofstream out{destFileName, std::ios::binary};
	try{
		CryptoPP::FileSource{in, /*pumpAll=*/true,
						   new CryptoPP::StreamTransformationFilter{
							   d, new CryptoPP::FileSink{out}}};
		in.close();
		out.close();
	} catch (...){
		try{
			in.close();
			out.close();
		} catch (...){

		}
		throw;
	}

	#ifdef ENC_DEBUG
		std::cout << "File len after: " << boost::filesystem::file_size(destFileName) << std::endl;
	#endif
}
