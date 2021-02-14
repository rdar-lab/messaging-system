/*
 * StrBytesBuffer.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include "StrByteBuffer.h"

#include <algorithm>
#include <iterator>

StrByteBuffer::StrByteBuffer(std::string str)
{
	this->str = str;
	this->offset = 0;
}

StrByteBuffer::~StrByteBuffer()
{
}

unsigned short int StrByteBuffer::readData(void *destinationBuffer,
		unsigned short int lenToReadInput)
{
	unsigned short amountToRead = getBytesLeft();
	if (amountToRead > lenToReadInput)
	{
		amountToRead = lenToReadInput;
	}

	std::string subStr = this->str.substr(this->offset, amountToRead);
	const char *charArr = subStr.c_str();

	std::copy(charArr, charArr + (amountToRead), destinationBuffer);

	this->offset = this->offset + amountToRead;
	return amountToRead;
}

unsigned int StrByteBuffer::getBytesLeft()
{
	return this->str.length() - this->offset;
}
