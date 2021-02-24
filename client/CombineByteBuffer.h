/*
 * CombineByteBuffer.h
 *
 *  Created on: 20 Feb 2021
 *      Author: xmaster
 */

#ifndef COMBINEBYTEBUFFER_H_
#define COMBINEBYTEBUFFER_H_

#include "ByteBuffer.h"

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
