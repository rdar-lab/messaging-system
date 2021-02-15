/*
 * Response.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "Response.h"

Response::~Response()
{
	if (this->payload != NULL)
	{
		delete this->payload;
	}
	socket->close();
}

std::ostream& operator<<(std::ostream &os, const Response &resp)
{
	os << std::string("version:");
	os << std::to_string(resp.getVersion());
	os << std::string(" ,response code:");
	os << std::to_string(resp.getResponseCode());
	return os;
}

Response::Response(boost::shared_ptr<tcp::socket> socket, unsigned short version, unsigned short responseCode,
		ByteBuffer *payload) {
	this->socket = socket;
	this->version = version;
	this->responseCode = responseCode;
	this->payload = payload;
}

unsigned short Response::getVersion() const {
	return version;
}

unsigned short Response::getResponseCode() const {
	return responseCode;
}

ByteBuffer* Response::getPayload() const {
	return payload;
}
