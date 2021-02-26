/*
 * Message.cpp
 *
 *  Created on: 18 Feb 2021
 *      Author: Roy Dar
 */

#include "Message.h"

Message::Message(ClientDef fromClient, unsigned int messageId, std::string messageContentTxt) {
	this->fromClient = fromClient;
	this->messageId = messageId;
	this->messageContentTxt = messageContentTxt;
}

Message::~Message() {
}

const ClientDef& Message::getFromClient() const {
	return fromClient;
}

const std::string& Message::getMessageContentTxt() const {
	return messageContentTxt;
}

unsigned int Message::getMessageId() const {
	return messageId;
}
