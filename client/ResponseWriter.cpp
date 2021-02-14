/*
 * ResponseWriter.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "ResponseWriter.h"
#include "CommunicationSerilizationHelper.h"

ResponseWriter::ResponseWriter(boost::asio::ip::tcp::socket *socket)
{
	this->socket = socket;
}

ResponseWriter::~ResponseWriter()
{
}

void ResponseWriter::writeResponse(Response *resp)
{
	CommunicationSerilizationHelper helper(socket);
	helper.writeByte(resp->getVersion());
	helper.writeShort(resp->getStatus());

	if (resp->getStatus() < 1002)
	{
		helper.writeStr(resp->getFileName());
	}

	if (resp->getStatus() < 212)
	{
		helper.writeBytes(resp->getPayload());
	}
}
