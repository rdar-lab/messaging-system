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
 * A byte buffer based on an STR. Although the interface is the same, using this implementation,
 * the entire STR is held in memory
 */
class StrByteBuffer: public ByteBuffer
{
public:
	StrByteBuffer(std::string str);
	virtual ~StrByteBuffer();
	virtual unsigned int getBytesLeft();
protected:
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead);
private:
	std::string str;
	unsigned int offset;
};

#endif /* STRBYTEBUFFER_H_ */
