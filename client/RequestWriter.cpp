/*
 * RequestReader.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "RequestWriter.h"

#include <string>
#include "Constants.h"
#include "CommunicationSerilizationHelper.h"

RequestWriter::RequestWriter(boost::shared_ptr<tcp::socket>  socket)
{
	this->socket = socket;
}

RequestWriter::~RequestWriter()
{
}

void RequestWriter::writeRequest(Request* req)
{
	CommunicationSerilizationHelper helper(&(*this->socket));
	unsigned short clientIdBuffer[CLIENT_ID_SIZE];
	req->getClientId().write(clientIdBuffer);
	helper.writeBytes(clientIdBuffer, CLIENT_ID_SIZE);

	helper.writeByte(req->getVersion());

	helper.writeByte(req->getRequestCode());

	if (req->getPayload() != NULL)
	{
		helper.writeInt(req->getPayload()->getBytesLeft());
		helper.writeBytes(req->getPayload());
	} else
	{
		helper.writeInt(0);
	}

}
