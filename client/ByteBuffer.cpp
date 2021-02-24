/*
 * ByteBuffer.cpp
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#include "ByteBuffer.h"
#include "Constants.h"
#include "Utils.h"

ByteBuffer::ByteBuffer()
{
}

ByteBuffer::~ByteBuffer()
{
}

void ByteBuffer::sendToStream(std::ostream &os)
{
	this->sendToStream(os, this->getBytesLeft());
}

void ByteBuffer::sendToStream(std::ostream &os, unsigned int amountToTransfer)
{
	char buffer[BUFFER_SIZE];
	while (amountToTransfer > 0)
	{
		unsigned int amount = BUFFER_SIZE;
		if (amount > amountToTransfer){
			amount = amountToTransfer;
		}
		amount = this->readData(buffer, amount);
		if (amount <= 0)
		{
			throw std::runtime_error(
					"Invalid state detected, read data returned non positive value");
		}
		else
		{
			os.write(buffer, amount);
			amountToTransfer = amountToTransfer - amount;
		}
	}
}

unsigned int ByteBuffer::readData(void *destinationBuffer,
			unsigned int lenToRead)
{
	unsigned int lenRead = 0;

	if (lenToRead > this->getBytesLeft())
	{
		lenToRead = this->getBytesLeft();
	}

	while (lenToRead > lenRead)
	{
		int amount = lenToRead - lenRead;
		if (amount>BUFFER_SIZE)
		{
			amount = BUFFER_SIZE;
		}

		amount = this->readDataInternal(((unsigned char*)destinationBuffer)+lenRead, amount);
		if (amount <= 0)
		{
			throw std::runtime_error(
					"Invalid state detected, read data returned non positive value");
		}
		else
		{
			lenRead = lenRead + amount;
		}
	}
	return lenRead;

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

unsigned short ByteBuffer::readShort()
{
	if (this->getBytesLeft()<2){
		throw std::runtime_error("EOS");
	}

	char buffer[2];
	unsigned int amountRead = this->readData(buffer, 2);

	if (amountRead!=2){
		throw std::runtime_error("EOS");
	}

	return Utils::convertToNum(buffer, 2);
}

unsigned int ByteBuffer::readInt()
{
	if (this->getBytesLeft()<4){
		throw std::runtime_error("EOS");
	}

	char buffer[4];
	unsigned int amountRead = this->readData(buffer, 4);

	if (amountRead!=4){
		throw std::runtime_error("EOS");
	}

	return Utils::convertToNum(buffer, 4);
}
