/*
 * FileByteBuffer.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#ifndef FILEBYTEBUFFER_H_
#define FILEBYTEBUFFER_H_

#include <string>
#include <iostream>
#include "ByteBuffer.h"
#include <fstream>

/*
 * A byte buffer implementation for reading a from a file
 */
class FileByteBuffer: public ByteBuffer
{
public:
	FileByteBuffer(std::string filePath);

	virtual ~FileByteBuffer();

	virtual unsigned int getBytesLeft();
protected:
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead);

private:
	unsigned int fileSize;
	unsigned int sizeRead;
	std::ifstream fileStream;
};

#endif /* FILEBYTEBUFFER_H_ */
