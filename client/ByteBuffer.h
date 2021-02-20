/*
 * ByteBuffer.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#include <iostream>

/*
 * ByteBuffer
 * ----------
 * An abstract class that allows chunked communication with bytes
 */
class ByteBuffer
{
public:
	/*
	 * Dtor
	 */
	virtual ~ByteBuffer();

	/*
	 * Returns the amount of bytes left on this buffer for reading
	 */
	virtual unsigned int getBytesLeft() = 0;

	/*
	 * Read the data into the input buffer with a maximum of len to read amount as
	 * parameter
	 */
	virtual unsigned short readData(void *destinationBuffer,
			unsigned short lenToRead) = 0;

	unsigned char  readByte();
	unsigned short readShort();
	unsigned int   readInt();

	/*
	 * Sends all the data to a given output stream
	 */
	void sendToStream(std::ostream &os);
	void sendToStream(std::ostream &os, unsigned int amount);
protected:

	/*
	 * Protected Ctor, since this is an abstract class
	 */
	ByteBuffer();
};

#endif /* BYTEBUFFER_H_ */
