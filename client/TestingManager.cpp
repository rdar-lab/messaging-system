/*
 * TestingManager.cpp
 *
 *  Created on: 18 Feb 2021
 *      Author: Roy Dar
 */

#include "TestingManager.h"
#include "iostream"
#include "fstream"
#include "string"
#include "ClientMetadataManager.h"
#include "InMemClientDatastore.h"
#include "ClientLogicHandler.h"
#include "CommunicationManager.h"
#include "EncryptionUtils.h"

TestingManager::TestingManager() {

}

void TestingManager::performTests() {
	unsigned char buff[10000];
	std::memset(buff, 0, sizeof(buff));
	unsigned char buff2[10000];
	std::memset(buff2, 0, sizeof(buff2));
	{
		std::cout << "Testing base 64..." << std::endl;
		buff[0] = 0;
		buff[1] = 1;
		buff[2] = 2;
		buff[3] = 3;
		std::string encoded = Utils::base64Encode(buff, 4);
		int decodedSize = Utils::base64Decode(encoded, buff2, 1000);
		if (decodedSize != 4)
		{
			throw std::runtime_error("Fail - decoded len incorrect");
		}

		if (buff[0]!=0 || buff[1]!=1 || buff[2]!=2 || buff[3]!=3){
			throw std::runtime_error("Fail BASE64 - decoded text not equal to original");
		}
	}
	{
		std::cout << "Testing encryption..." << std::endl;
		auto pair = EncryptionUtils::generateKeypair(ENCRYPTION_ALGORITHM_RSA);
		std::memset(buff, 0, sizeof(buff));
		buff[0] = 0;
		buff[1] = 1;
		buff[2] = 2;
		buff[3] = 3;

		std::memset(buff2, 0, sizeof(buff2));

		unsigned int cipherTextLen = EncryptionUtils::pkiEncrypt(ENCRYPTION_ALGORITHM_RSA, pair.getPublicKey(), buff, 16, buff2, 1000);
		if (buff2[0]==0 && buff2[1]==1 && buff[2]==2 && buff[3]==3){
			throw std::runtime_error("Fail RSA - cipher Text equals plain text");
		}

		std::memset(buff, 0, sizeof(buff));
		EncryptionUtils::pkiDecrypt(ENCRYPTION_ALGORITHM_RSA, pair.getPrivateKey(), buff2, cipherTextLen, buff, 16);
		if (buff[0]!=0 || buff[1]!=1 || buff[2]!=2 || buff[3]!=3){
			throw std::runtime_error("Fail RSA - decoded text not equal to original");
		}

		std::memset(buff, 0, sizeof(buff));
		for (int index=0; index<5000; index++){
			buff[index] = index;
		}

		SymmetricKey key = EncryptionUtils::generateSymmetricKey(ENCRYPTION_ALGORITHM_AES);
		std::memset(buff2, 0, sizeof(buff2));

		cipherTextLen = EncryptionUtils::symmetricEncrypt(ENCRYPTION_ALGORITHM_AES, key, buff, 5000, buff2, 10000);
		if (buff2[0]==0 && buff2[1]==1 && buff[2]==2 && buff[3]==3){
			throw std::runtime_error("Fail AES - cipher Text equals plain text");
		}

		std::memset(buff, 0, sizeof(buff));
		int decodedSize = EncryptionUtils::symmetricDecrypt(ENCRYPTION_ALGORITHM_AES, key, buff2, cipherTextLen, buff, 10000);
		if (decodedSize != 5000)
		{
			throw std::runtime_error("Fail AES - Decoded size does not match");
		}

		for (int index=0; index<500; index++){
			if (buff[index] != (unsigned char)index){
				throw std::runtime_error("Fail AES - decoded text not equal to original. index=" + std::to_string(index) + ". value=" + std::to_string(buff[index]));
			}
		}

		std::string tmpFile = std::string(std::tmpnam(NULL));
		std::ofstream out{tmpFile, std::ios::binary};
		out << "This is the the line in the file" << std::endl;
		out.close();

		std::string tmpFile2 = std::string(std::tmpnam(NULL));
		std::string tmpFile3 = std::string(std::tmpnam(NULL));
		EncryptionUtils::symmetricEncryptFile(ENCRYPTION_ALGORITHM_AES, key, tmpFile, tmpFile2);
		EncryptionUtils::symmetricDecryptFile(ENCRYPTION_ALGORITHM_AES, key, tmpFile2, tmpFile3);
		std::ifstream in{tmpFile3, std::ios::binary};
		std::string tmpLine;
		std::getline(in,tmpLine);
		if (tmpLine != "This is the the line in the file"){
			throw std::runtime_error("Fail - AES encrypt file - Unexpected text in file: --" + tmpLine + "--");
		}

	}


	std::cout << "Running tests - make sure to have a clean server" << std::endl;

	CommunicationManager::getInstance()->setParams("127.0.0.1", 1234);

	ClientMetadataManager client1;
	InMemClientDatastore dataStoreClient1;
	ClientMetadataManager client2;
	InMemClientDatastore dataStoreClient2;
	ClientMetadataManager::singleInstance = &client1;
	ClientDatastore::setInstance(&dataStoreClient1);
	{
		std::cout << "Test 1 - register client1" << std::endl;
		ClientLogicHandler().performRegister("client1");
		std::cout << "Pass" << std::endl;
	}

	{
		std::cout << "Test 2 - register again" << std::endl;
		try{
			ClientLogicHandler().performRegister("client1");
			throw std::runtime_error("Fail - double register should have failed");
		} catch (std::exception &err){
			std::cout << "Pass" << std::endl;
		}
	}
	{
		std::cout << "Test 3 - get clients list" << std::endl;
		ClientLogicHandler().performGetClientsList();
		if (ClientDatastore::getInstance()->getAllClients().size() > 0){
			throw std::runtime_error("Fail - more clients then expected");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 4 - get messages" << std::endl;
		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() > 0){
			throw std::runtime_error("Fail - Expected no messages");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 5 - Register client 2 under the name client1" << std::endl;
		ClientMetadataManager::singleInstance = &client2;
		ClientDatastore::setInstance(&dataStoreClient2);
		try{
			ClientLogicHandler().performRegister("client1");
			throw std::runtime_error("Fail - register under the name client1 should have failed");
		} catch (std::exception &err){
			std::cout << "Pass" << std::endl;
		}

	}
	{
		std::cout << "Test 6 - Register client 2" << std::endl;
		ClientMetadataManager::singleInstance = &client2;
		ClientDatastore::setInstance(&dataStoreClient2);
		ClientLogicHandler().performRegister("client2");
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 7 - get clients list" << std::endl;
		ClientLogicHandler().performGetClientsList();
		auto clients = ClientDatastore::getInstance()->getAllClients();
		if (clients.size() != 1){
			throw std::runtime_error("Fail - expected exactly one clients");
		}
		if (clients.begin()->getName() != "client1")
		{
			throw std::runtime_error("Fail - client name is not correct");
		}
		if (!clients.begin()->getPublicKey().isEmpty() || !clients.begin()->getSymmetricKey().isEmpty()){
			throw std::runtime_error("Fail - both keys should not be loaded");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 8 - load public key" << std::endl;
		ClientLogicHandler().performGetClientPublicKey("client1");
		auto clients = ClientDatastore::getInstance()->getAllClients();
		if (clients.size() != 1){
			throw std::runtime_error("Fail - expected exactly one clients");
		}
		if (clients.begin()->getName() != "client1")
		{
			throw std::runtime_error("Fail - client name is not correct");
		}
		if (clients.begin()->getPublicKey().isEmpty()){
			throw std::runtime_error("Fail - public key should not be empty");
		}

		if (!clients.begin()->getSymmetricKey().isEmpty()){
			throw std::runtime_error("Fail - symmetric key should be empty");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 9 - send request for symmetric key" << std::endl;
		ClientLogicHandler().performSendRequestForSymmetricKey("client1");
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 10 - read messages" << std::endl;
		ClientMetadataManager::singleInstance = &client1;
		ClientDatastore::setInstance(&dataStoreClient1);
		ClientLogicHandler().performGetClientsList();
		ClientLogicHandler().performGetClientPublicKey("client2");
		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() != 1){
			throw std::runtime_error("Fail - expected exactly one message");
		}
		if (messages.begin()->getFromClient().getName()!="client2"){
			throw std::runtime_error("Fail - expected message from client2");
		}
		if (messages.begin()->getFromClient().getId()!=client2.getClientId()){
			throw std::runtime_error("Fail - expected message from client2 ID");
		}
		if (messages.begin()->getMessageContentTxt() != "Request for symmetric key")
		{
			throw std::runtime_error("Fail - Unexpected message text");
		}
		messages = ClientLogicHandler().performGetMessages();
		if (messages.size() > 0){
			throw std::runtime_error("Fail - expected no message");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 11 - send symmetric key" << std::endl;
		ClientLogicHandler().performSendSymmetricKey("client2");

		auto clients = ClientDatastore::getInstance()->getAllClients();
		if (clients.size() != 1){
			throw std::runtime_error("Fail - expected exactly one clients");
		}
		if (clients.begin()->getName() != "client2")
		{
			throw std::runtime_error("Fail - client name is not correct");
		}
		if (clients.begin()->getPublicKey().isEmpty()){
			throw std::runtime_error("Fail - public key should not be empty");
		}

		if (clients.begin()->getSymmetricKey().isEmpty()){
			throw std::runtime_error("Fail - symmetric key should not be empty");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 12 - read symmetric key" << std::endl;
		ClientMetadataManager::singleInstance = &client2;
		ClientDatastore::setInstance(&dataStoreClient2);

		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() != 1){
			throw std::runtime_error("Fail - expected exactly one message");
		}
		if (messages.begin()->getFromClient().getName()!="client1"){
			throw std::runtime_error("Fail - expected message from client1");
		}
		if (messages.begin()->getFromClient().getId()!=client1.getClientId()){
			throw std::runtime_error("Fail - expected message from client1 ID");
		}
		if (messages.begin()->getMessageContentTxt() != "Symmetric key received")
		{
			throw std::runtime_error("Fail - Unexpected message text");
		}
		messages = ClientLogicHandler().performGetMessages();
		if (messages.size() > 0){
			throw std::runtime_error("Fail - expected no message");
		}
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 13 - Send messages" << std::endl;
		ClientLogicHandler().performSendTextMessage("client1", "message 1 from client2");
		ClientLogicHandler().performSendTextMessage("client1", "message 2 from client2");

		char longMessage[5001];
		for (int index=0; index<5000; index++){
			longMessage[index] = '0' + (index%10);
		}
		longMessage[5000] = 0;

		ClientLogicHandler().performSendTextMessage("client1", std::string(longMessage));

		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 14 - Get messages" << std::endl;
		ClientMetadataManager::singleInstance = &client1;
		ClientDatastore::setInstance(&dataStoreClient1);

		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() != 3){
			throw std::runtime_error("Fail - expected exactly one message");
		}

		auto it = messages.begin();
		Message msg1 = *it;
		it++;
		Message msg2 = *it;
		it++;
		Message msg3 = *it;

		if (msg1.getFromClient().getName()!="client2"){
			throw std::runtime_error("Fail - expected message from client2");
		}

		if (msg1.getFromClient().getId()!=client2.getClientId()){
			throw std::runtime_error("Fail - expected message from client2 ID");
		}

		if (msg2.getFromClient().getName()!="client2"){
			throw std::runtime_error("Fail - expected message from client2");
		}

		if (msg2.getFromClient().getId()!=client2.getClientId()){
			throw std::runtime_error("Fail - expected message from client2 ID");
		}

		if (msg3.getFromClient().getName()!="client2"){
			throw std::runtime_error("Fail - expected message from client2");
		}

		if (msg3.getFromClient().getId()!=client2.getClientId()){
			throw std::runtime_error("Fail - expected message from client2 ID");
		}

		if (msg1.getMessageContentTxt() != "message 1 from client2")
		{
			throw std::runtime_error("Fail - MSG1 - Unexpected message text: --" + msg1.getMessageContentTxt() + "--");
		}

		if (msg2.getMessageContentTxt() != "message 2 from client2")
		{
			throw std::runtime_error("Fail - MSG2 - Unexpected message text: --" + msg2.getMessageContentTxt() + "--");
		}

		std::string longMsg = msg3.getMessageContentTxt();
		if (longMsg.length() != 5000)
		{
			throw std::runtime_error("Fail - MSG3 - got unexpected length: " + std::to_string(longMsg.length()));
		}

		const char* data = longMsg.data();
		for (int index=0; index<5000; index++){
			if (data[index]!= ('0' + (index%10))){
				throw std::runtime_error("Fail - MSG3 - got unexpected token on index: " +
						std::to_string(index) + ", value=" + std::to_string(data[index]) +
						", expected=" + std::to_string('A' + (index%10))
				);
			}
		}

		messages = ClientLogicHandler().performGetMessages();
		if (messages.size() > 0){
			throw std::runtime_error("Fail - expected no message");
		}

		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 15 - Send file message" << std::endl;
		std::string tmpFile = std::string(std::tmpnam(NULL));
		std::ofstream out{tmpFile, std::ios::binary};
		out << "This is the the line in the file" << std::endl;
		out.close();
		ClientLogicHandler().performSendFile("client2", tmpFile);
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 16 - Get file message" << std::endl;
		ClientMetadataManager::singleInstance = &client2;
		ClientDatastore::setInstance(&dataStoreClient2);
		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() != 1){
			throw std::runtime_error("Fail - expected exactly one message");
		}

		auto it = messages.begin();
		Message msg1 = *it;

		if (msg1.getFromClient().getName()!="client1"){
			throw std::runtime_error("Fail - expected message from client1");
		}

		if (msg1.getFromClient().getId()!=client1.getClientId()){
			throw std::runtime_error("Fail - expected message from client1 ID");
		}

		if (msg1.getMessageContentTxt().find("FILE: ")!=0)
		{
			throw std::runtime_error("Fail - MSG1 - Unexpected message text: --" + msg1.getMessageContentTxt() + "--");
		}

		std::string fileName = msg1.getMessageContentTxt().substr(6, msg1.getMessageContentTxt().size());
		std::ifstream in{fileName, std::ios::binary};
		std::string tmpLine;
		std::getline(in,tmpLine);
		if (tmpLine != "This is the the line in the file"){
			throw std::runtime_error("Fail - MSG1 - Unexpected text in file: --" + tmpLine + "--");
		}
		std::cout << "Pass" << std::endl;
	}

}

TestingManager::~TestingManager() {
}

