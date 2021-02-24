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
 * FileByteBuffer
 * --------------
 * A byte buffer implementation for reading a from a file
 */
class FileByteBuffer: public ByteBuffer
{
public:
	/*
	 * Ctor
	 */
	FileByteBuffer(std::string filePath);

	/*
	 * Dtor
	 */
	virtual ~FileByteBuffer();

	/*
	 * Returns the amount of data left to read
	 */
	virtual unsigned int getBytesLeft();
protected:
	/*
	 * Reads the data based on chunks
	 */
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead);

private:
	unsigned int fileSize;
	unsigned int sizeRead;
	std::ifstream fileStream;
};

#endif /* FILEBYTEBUFFER_H_ */
