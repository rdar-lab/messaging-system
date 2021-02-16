/*
 * CommunicationUtils.h
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#ifndef COMMUNICATIONSERILIZATIONHELPER_H_
#define COMMUNICATIONSERILIZATIONHELPER_H_

#include <string>
#include "ByteBuffer.h"
#include <boost/asio.hpp>

/*
 * CommunicationSerilizationHelper
 * -------------------------------
 * Helper for sending and reading data from the socket
 */
class CommunicationSerilizationHelper
{
public:
	/*
	 * Ctor
	 */
	CommunicationSerilizationHelper(boost::asio::ip::tcp::socket *socket);

	/*
	 * Dtor
	 */
	virtual ~CommunicationSerilizationHelper();

	/*
	 * Reads an unsigned number of size of 1 byte
	 */
	unsigned char readByte();

	/*
	 * Writes an unsigned number of size of 1 byte
	 */
	void writeByte(unsigned char data);

	/*
	 * Reads an unsigned number of size of 2 byte
	 */
	unsigned short readShort();

	/*
	 * Writes an unsigned number of size of 2 byte
	 */
	void writeShort(unsigned short data);

	/*
	 * Reads an unsigned number of size of 4 byte
	 */
	unsigned int readInt();

	/*
	 * Writes an unsigned number of size of 4 byte
	 */
	void writeInt(unsigned int data);

	/*
	 * Reads an str from the socket. the size of the str will be read first
	 */
	std::string readStr(unsigned short size);

	/*
	 * Writes an str from the socket. the size of the str will be written first
	 */
	void writeStr(std::string data);

	/*
	 * Reads a bytes array from the socket. The size of the bytes buffer will first be read
	 * This will not read the data from the socket, but instead wrap it and allows to work
	 * with chunks
	 */
	ByteBuffer* readBytes(unsigned int size);

	/*
	 * Writes the full data from the bytes buffer into the socket
	 */
	void writeBytes(ByteBuffer *data);

	void writeBytes(unsigned char *buffer, unsigned int len);
private:
	// The socket to be used
	boost::asio::ip::tcp::socket *socket;
};

#endif /* COMMUNICATIONSERILIZATIONHELPER_H_ */
