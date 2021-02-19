/*
 * TestingManager.cpp
 *
 *  Created on: 18 Feb 2021
 *      Author: xmaster
 */

#include "TestingManager.h"
#include "iostream"
#include "string"
#include "ClientMetadataManager.h"
#include "InMemClientDatastore.h"
#include "ClientLogicHandler.h"
#include "CommunicationManager.h"
#include "EncryptionUtils.h"

TestingManager::TestingManager() {

}

void TestingManager::performTests() {
	std::cout << "Testing encryption..." << std::endl;
	auto pair = EncryptionUtils::generateKeypair(ENCRYPTION_ALGORITHM_RSA);
	char buff[16];
	std::memset(buff, 0, sizeof(buff));
	buff[0] = 0;
	buff[1] = 1;
	buff[2] = 2;
	buff[3] = 3;

	char buff2[1000];
	std::memset(buff2, 0, sizeof(buff));

	unsigned int cipherTextLen = EncryptionUtils::pkiEncrypt(ENCRYPTION_ALGORITHM_RSA, pair.getPublicKey(), buff, 16, buff2, 1000);
	if (buff2[0]==0 && buff2[1]==1 && buff[2]==2 && buff[3]==3){
		throw std::runtime_error("Fail RSA - cipher Text equals plain text");
	}

	std::memset(buff, 0, sizeof(buff));
	EncryptionUtils::pkiDecrypt(ENCRYPTION_ALGORITHM_RSA, pair.getPrivateKey(), buff2, cipherTextLen, buff, 16);
	if (buff[0]!=0 || buff[1]!=1 || buff[2]!=2 || buff[3]!=3){
		throw std::runtime_error("Fail RSA - decoded text not equal to original");
	}

	SymmetricKey key = EncryptionUtils::generateSymmetricKey(ENCRYPTION_ALGORITHM_AES);
	std::memset(buff2, 0, sizeof(buff));

	cipherTextLen = EncryptionUtils::symmetricEncrypt(ENCRYPTION_ALGORITHM_AES, key, buff, 16, buff2, 1000);
	if (buff2[0]==0 && buff2[1]==1 && buff[2]==2 && buff[3]==3){
		throw std::runtime_error("Fail AES - cipher Text equals plain text");
	}

	std::memset(buff, 0, sizeof(buff));
	EncryptionUtils::symmetricDecrypt(ENCRYPTION_ALGORITHM_RSA, key, buff2, cipherTextLen, buff, 16);
	if (buff[0]!=0 || buff[1]!=1 || buff[2]!=2 || buff[3]!=3){
		throw std::runtime_error("Fail AES - decoded text not equal to original");
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
		std::cout << "Pass" << std::endl;
	}
	{
		std::cout << "Test 14 - Get messages" << std::endl;
		ClientMetadataManager::singleInstance = &client1;
		ClientDatastore::setInstance(&dataStoreClient1);

		auto messages = ClientLogicHandler().performGetMessages();
		if (messages.size() != 2){
			throw std::runtime_error("Fail - expected exactly one message");
		}

		auto it = messages.begin();
		Message msg1 = *it;
		it++;
		Message msg2 = *it;

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

		if (msg1.getMessageContentTxt() != "message 1 from client2")
		{
			throw std::runtime_error("Fail - MSG1 - Unexpected message text: --" + msg1.getMessageContentTxt() + "--");
		}

		if (msg2.getMessageContentTxt() != "message 2 from client2")
		{
			throw std::runtime_error("Fail - MSG2 - Unexpected message text: --" + msg2.getMessageContentTxt() + "--");
		}

		messages = ClientLogicHandler().performGetMessages();
		if (messages.size() > 0){
			throw std::runtime_error("Fail - expected no message");
		}

		std::cout << "Pass" << std::endl;
	}

}

TestingManager::~TestingManager() {
}

