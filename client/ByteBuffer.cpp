/*
 * ByteBuffer.cpp
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#include "ByteBuffer.h"
#include "Constants.h"

ByteBuffer::ByteBuffer()
{
}

ByteBuffer::~ByteBuffer()
{
}

void ByteBuffer::sendToStream(std::ostream &os)
{
	char buffer[BUFFER_SIZE];
	while (this->getBytesLeft() > 0)
	{
		short amount = this->readData(buffer, BUFFER_SIZE);
		if (amount <= 0)
		{
			throw std::runtime_error(
					"Invalid state detected, read data returned non positive value");
		}
		else
		{
			os.write(buffer, amount);
		}
	}
}

unsigned char ByteBuffer::readByte()
{
	if (this->getBytesLeft()==0){
		throw std::runtime_error("EOS");
	}

	char buffer[1];
	unsigned int amountRead = this->readData(buffer, 1);

	if (amountRead!=1){
		throw std::runtime_error("EOS");
	}

	return *buffer;
}
