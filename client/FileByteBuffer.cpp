/*
 * FileByteBuffer.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "FileByteBuffer.h"

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

unsigned int FileByteBuffer::readDataInternal(void *destinationBuffer,
		unsigned int lenToRead)

{
	if (!this->fileStream.good()) {
		throw std::runtime_error(
			"Invalid state detected. Stream error detected");
	}

	if (lenToRead > this->getBytesLeft()) {
		lenToRead = this->getBytesLeft();
	}

	this->fileStream.read((char*)destinationBuffer, lenToRead);

	if (this->fileStream.fail()) {
		throw std::runtime_error(
			"Invalid state detected. Failure detected");
	}

	this->sizeRead = this->sizeRead + lenToRead;
	return lenToRead;
}

unsigned int FileByteBuffer::getBytesLeft()
{
	return this->fileSize - this->sizeRead;
}
