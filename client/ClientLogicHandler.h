/*
 * ClientLogicHandler.h
 *
 *  Created on: 18 Feb 2021
 *      Author: Roy Dar
 */

#ifndef CLIENTLOGICHANDLER_H_
#define CLIENTLOGICHANDLER_H_

#include "Response.h"
#include "ClientId.h"
#include "ClientDef.h"
#include <list>
#include "Message.h"

/*
 * This class is responsible for the implementation of the main logic actions of the client in the messaging system.
 * Such as: Registration, sending and receiving messages, negotiation of keys...
 */
class ClientLogicHandler {
public:
	ClientLogicHandler();
	virtual ~ClientLogicHandler();
	ClientId performRegister(std::string clientName);
	void performGetClientsList();
	void performGetClientPublicKey(std::string clientName);
	std::list<Message> performGetMessages();
	void performSendTextMessage(std::string clientName, std::string messageText);
	void performSendFile(std::string clientName, std::string fileName);
	void performSendRequestForSymmetricKey(std::string clientName);
	void performSendSymmetricKey(std::string clientName);
private:
	unsigned int sendMessage(ClientDef destinationClient, unsigned char messageType, void *messageContent, unsigned int messageLen);
	unsigned int sendMessage(ClientDef destinationClient, unsigned char messageType, ByteBuffer* messageContentBuffer);
	Response *resp;
};

#endif /* CLIENTLOGICHANDLER_H_ */
