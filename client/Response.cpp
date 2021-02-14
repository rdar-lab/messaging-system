/*
 * Response.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "Response.h"

Response::Response(unsigned short status)
{
	this->status = status;
}

Response::Response(unsigned short status, std::string fileName)
{
	this->status = status;
	this->fileName = fileName;
}

Response::Response(unsigned short status, std::string fileName,
		ByteBuffer *payload)
{
	this->status = status;
	this->fileName = fileName;
	this->payload = payload;
}

Response::~Response()
{
	if (this->payload != NULL)
	{
		delete this->payload;
	}
}

unsigned short Response::getVersion() const
{
	return this->version;
}

unsigned short Response::getStatus() const
{
	return this->status;
}

std::string Response::getFileName() const
{
	return this->fileName;
}

ByteBuffer* Response::getPayload() const
{
	return this->payload;
}

std::ostream& operator<<(std::ostream &os, const Response &resp)
{
	os << std::string("version:");
	os << std::to_string(resp.getVersion());
	os << std::string(" ,status=");
	os << std::to_string(resp.getStatus());
	os << std::string(" ,fileName=");
	os << resp.getFileName();
	return os;
}

