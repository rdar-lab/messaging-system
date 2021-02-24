/*
 * BinaryByteBuffer.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef BINARYBYTEBUFFER_H_
#define BINARYBYTEBUFFER_H_

#include "ByteBuffer.h"

class BinaryByteBuffer : public ByteBuffer {
public:
	BinaryByteBuffer(void *data, unsigned int len);
	virtual ~BinaryByteBuffer();
	virtual unsigned int getBytesLeft();
protected:
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead);
private:
	void* data;
	unsigned int offset;
	unsigned int len;


};

#endif /* BINARYBYTEBUFFER_H_ */
