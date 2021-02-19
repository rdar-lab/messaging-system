/*
 * ClientLogicHandler.h
 *
 *  Created on: 18 Feb 2021
 *      Author: xmaster
 */

#ifndef CLIENTLOGICHANDLER_H_
#define CLIENTLOGICHANDLER_H_

#include "Response.h"
#include "ClientId.h"
#include "ClientDef.h"
#include <list>
#include "Message.h"

class ClientLogicHandler {
public:
	ClientLogicHandler();
	virtual ~ClientLogicHandler();
	ClientId performRegister(std::string clientName);
	void performGetClientsList();
	void performGetClientPublicKey(std::string clientName);
	std::list<Message> performGetMessages();
	void performSendTextMessage(std::string clientName, std::string messageText);
	void performSendRequestForSymmetricKey(std::string clientName);
	void performSendSymmetricKey(std::string clientName);
private:
	unsigned int sendMessage(ClientDef destinationClient, unsigned char messageType, void *messageContent, unsigned int messageLen);
	Response *resp;
};

#endif /* CLIENTLOGICHANDLER_H_ */
