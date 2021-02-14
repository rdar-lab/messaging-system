/*
 * BinaryByteBuffer.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "BinaryByteBuffer.h"

BinaryByteBuffer::BinaryByteBuffer(void *data, unsigned int len)
{
	this->data = data;
	this->len = len;
	this->offset = 0;
}

BinaryByteBuffer::~BinaryByteBuffer()
{
}

unsigned short int BinaryByteBuffer::readData(void *destinationBuffer,
		unsigned short int lenToReadInput)
{
	unsigned short amountToRead = getBytesLeft();
	if (amountToRead > lenToReadInput)
	{
		amountToRead = lenToReadInput;
	}

	std::copy((char*)this->data, (char*)this->data + (amountToRead), (char*)destinationBuffer);

	this->offset = this->offset + amountToRead;
	return amountToRead;
}

unsigned int BinaryByteBuffer::getBytesLeft()
{
	return this->len - this->offset;
}
