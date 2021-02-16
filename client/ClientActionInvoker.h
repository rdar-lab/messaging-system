/*
 * ClientActionInvoker.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef CLIENTACTIONINVOKER_H_
#define CLIENTACTIONINVOKER_H_

#include "ClientMetadataManager.h"
#include "ClientDef.h"

class ClientActionInvoker {
public:
	ClientActionInvoker();
	int performAction(int selectedOption);
	virtual ~ClientActionInvoker();
private:
	ClientDef promptAndGetClient();
	void sendMessage(ClientDef destinationClient, unsigned char messageType, void *messageContent, unsigned int messageLen);
	void performRegister();
	void performGetClientsList();
	void performGetClientPublicKey();
	void performGetMessages();
	void performSendTextMessage();
	void performSendRequestForSymmetricKey();
	void performSendSymmetricKey();
};

#endif /* CLIENTACTIONINVOKER_H_ */
