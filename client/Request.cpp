/*
 * Request.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "Request.h"

Request::~Request()
{
	if (payload != NULL)
	{
		delete payload;
	}
}

Request::Request(ClientId clientId, unsigned short version,
		unsigned short requestCode, ByteBuffer *payload) {
	this->clientId = clientId;
	this->version = version;
	this->requestCode = requestCode;
	this->payload = payload;
}

ClientId Request::getClientId() const {
	return clientId;
}

unsigned short Request::getVersion() const {
	return version;
}

unsigned short Request::getRequestCode() const {
	return requestCode;
}

ByteBuffer* Request::getPayload() const {
	return payload;
}

std::ostream& operator<<(std::ostream &os, const Request &req)
{
	os << std::string("version:");
	os << std::to_string(req.getVersion());
	os << std::string(", client id:");
	os << req.getClientId();
	os << std::string(" ,request code=");
	os << req.getRequestCode();
	return os;
}
