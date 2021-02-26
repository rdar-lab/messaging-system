/*
 * SocketByteBuffer.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#ifndef SOCKETBYTEBUFFER_H_
#define SOCKETBYTEBUFFER_H_

#include "ByteBuffer.h"
#include <boost/asio.hpp>

/*
 * An implementation of a byte buffer bounded to a socket and limited to a specific size
 */
class SocketByteBuffer: public ByteBuffer
{
public:
	SocketByteBuffer(boost::asio::ip::tcp::socket *socket,
			unsigned int dataLen);

	virtual ~SocketByteBuffer();

	virtual unsigned int getBytesLeft() override;

protected:
	virtual unsigned int readDataInternal(void *destinationBuffer,
			unsigned int lenToRead) override;

private:
	boost::asio::ip::tcp::socket *socket;
	unsigned int dataLen;
	unsigned int dataRead;
};

#endif /* SOCKETBYTEBUFFER_H_ */
