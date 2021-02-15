/*
 * ResponseWriter.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "ResponseReader.h"
#include "CommunicationSerilizationHelper.h"
#include "SocketByteBuffer.h"

ResponseReader::ResponseReader(boost::shared_ptr<tcp::socket> socket)
{
	this->socket = socket;
}

ResponseReader::~ResponseReader()
{

}

Response * ResponseReader::readResponse()
{
	CommunicationSerilizationHelper helper(&(*socket));
	unsigned short version = helper.readByte();
	unsigned short responseCode = helper.readShort();
	unsigned int payloadSize = helper.readInt();
	ByteBuffer *payload = NULL;
	if (payloadSize > 0)
	{
		payload = new SocketByteBuffer(&(*socket), payloadSize);
	}
	return new Response(socket, version, responseCode, payload);
}
