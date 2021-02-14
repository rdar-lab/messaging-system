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

Request::Request(unsigned short version, unsigned int userId, unsigned short op,
		std::string fileName, ByteBuffer *payload)
{
	this->version = version;
	this->userId = userId;
	this->op = op;
	this->fileName = fileName;
	this->payload = payload;
}

unsigned short Request::getVersion() const
{
	return version;
}

unsigned int Request::getUserId() const
{
	return userId;
}

unsigned short Request::getOp() const
{
	return op;
}

std::string Request::getFileName() const
{
	return this->fileName;
}

ByteBuffer* Request::getPayload() const
{
	return this->payload;
}

std::ostream& operator<<(std::ostream &os, const Request &req)
{
	os << std::string("version:");
	os << std::to_string(req.getVersion());
	os << std::string(" ,usedId=");
	os << std::to_string(req.getUserId());
	os << std::string(" ,op=");
	os << std::to_string(req.getOp());
	os << std::string(" ,fileName=");
	os << req.getFileName();
	return os;
}
