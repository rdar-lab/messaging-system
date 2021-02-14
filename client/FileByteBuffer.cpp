/*
 * FileByteBuffer.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "FileByteBuffer.h"
#include "GeneralException.h"

namespace fs = boost::filesystem;

FileByteBuffer::FileByteBuffer(std::string filePath)
{
	this->fileSize = fs::file_size(filePath);
	this->sizeRead = 0;
	this->fileStream.open(filePath, std::ios::binary);
}

FileByteBuffer::~FileByteBuffer()
{
	this->fileStream.close();
}

unsigned short int FileByteBuffer::readData(char *destinationBuffer,
		unsigned short int lenToRead)
{
	int amount = this->fileStream.readsome(destinationBuffer, lenToRead);

	if (amount <= 0)
	{
		throw GeneralException(
				"Invalid state detected. Amount read is not positive");
	}

	this->sizeRead = this->sizeRead + amount;
	return amount;
}

unsigned int FileByteBuffer::getBytesLeft()
{
	return this->fileSize - this->sizeRead;
}
