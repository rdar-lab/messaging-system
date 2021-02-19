/*
 * Message.h
 *
 *  Created on: 18 Feb 2021
 *      Author: xmaster
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "ClientDef.h"

class Message {
public:
	Message(ClientDef fromClient, unsigned int messageId, std::string messageContentTxt);
	virtual ~Message();
	const ClientDef& getFromClient() const;
	const std::string& getMessageContentTxt() const;
	unsigned int getMessageId() const;

private:
	ClientDef fromClient;
	unsigned int messageId;
	std::string messageContentTxt;
};

#endif /* MESSAGE_H_ */
