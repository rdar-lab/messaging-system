/*
 * RequestReader.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include <string>
#include "Constants.h"
#include "RequestReader.h"
#include "CommunicationSerilizationHelper.h"
#include "GeneralException.h"

RequestReader::RequestReader(boost::asio::ip::tcp::socket *socket)
{
	this->socket = socket;
}

RequestReader::~RequestReader()
{
}

Request* RequestReader::readRequest()
{
	CommunicationSerilizationHelper helper(this->socket);
	unsigned int userId = helper.readInt();
	unsigned short version = helper.readByte();
	unsigned short op = helper.readByte();
	std::string fileName = "";
	if (op < 202)
	{
		fileName = helper.readStr();
	}

	ByteBuffer *payload = NULL;
	if (op == 100)
	{
		payload = helper.readBytes();
	}

	return new Request(version, userId, op, fileName, payload);

}
