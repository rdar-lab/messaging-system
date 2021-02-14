/*
 * StrBytesBuffer.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#ifndef STRBYTEBUFFER_H_
#define STRBYTEBUFFER_H_

#include "ByteBuffer.h"

/*
 * StrByteBuffer
 * -------------
 * A byte buffer based on an STR. Although the interface is the same, using this implementation,
 * the entire STR is held in memory
 */
class StrByteBuffer: public ByteBuffer
{
public:
	StrByteBuffer(std::string str);
	virtual ~StrByteBuffer();
	virtual unsigned short int readData(void *destinationBuffer,
			unsigned short int lenToRead);
	virtual unsigned int getBytesLeft();
private:
	std::string str;
	unsigned int offset;
};

#endif /* STRBYTEBUFFER_H_ */
