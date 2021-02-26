/*
 * CombineByteBuffer.h
 *
 *  Created on: 20 Feb 2021
 *      Author: Roy Dar
 */

#ifndef COMBINEBYTEBUFFER_H_
#define COMBINEBYTEBUFFER_H_

#include "ByteBuffer.h"

/*
 * An implementation of ByteBuffer that allows combination of two other byte buffers without
 * Loading them into the memory
 */
class CombineByteBuffer: public ByteBuffer {
public:
	CombineByteBuffer(ByteBuffer* first, ByteBuffer* second);
	virtual ~CombineByteBuffer();
	virtual unsigned int getBytesLeft();
protected:
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead);
private:
	ByteBuffer* first;
	ByteBuffer* second;
};

#endif /* COMBINEBYTEBUFFER_H_ */
