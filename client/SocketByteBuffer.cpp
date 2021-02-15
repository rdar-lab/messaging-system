/*
 * SocketByteBuffer.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include "SocketByteBuffer.h"

SocketByteBuffer::SocketByteBuffer(boost::asio::ip::tcp::socket *socket,
		unsigned int dataLen)
{
	this->socket = socket;
	this->dataLen = dataLen;
	this->dataRead = 0;
}

SocketByteBuffer::~SocketByteBuffer()
{

}

unsigned short int SocketByteBuffer::readData(void *destinationBuffer,
		unsigned short lenToRead)
{
	if (lenToRead > this->getBytesLeft())
	{
		lenToRead = this->getBytesLeft();
	}

	size_t amount = boost::asio::read(*socket,
			boost::asio::buffer(destinationBuffer, lenToRead));
	if (amount <= 0)
	{
		throw std::runtime_error(
				"Invalid state detected. Amount read is not positive");
	}

	this->dataRead = this->dataRead + amount;
	return amount;
}

unsigned int SocketByteBuffer::getBytesLeft()
{
	return this->dataLen - this->dataRead;
}
