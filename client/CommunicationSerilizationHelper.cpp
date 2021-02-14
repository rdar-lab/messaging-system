/*
 * CommunicationUtils.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include "CommunicationSerilizationHelper.h"
#include "GeneralException.h"
#include "SocketByteBuffer.h"
#include "Constants.h"

/*
 * Convert byte stream to a number based on the buffer and size
 * Uses little endian
 */
unsigned int convertToNum(void *buff, unsigned short sizeOfBuffer)
{
	unsigned int result = 0;
	for (int i = sizeOfBuffer - 1; i >= 0; i--)
	{
		result = result << 8;
		unsigned char val = buff[i];
		result = result + val;
	}
	return result;
}

/*
 * Converts a number to a buffer of bytes uses little endian
 */
void convertToBytes(unsigned int num, void *buff, unsigned short sizeOfBuffer)
{
	unsigned int temp = num;
	for (int i = 0; i < sizeOfBuffer; i++)
	{
		char currByte = temp & 0xFF;
		buff[i] = currByte;
		temp = temp >> 8;
	}
}

unsigned int readNum(boost::asio::ip::tcp::socket *socket,
		unsigned short sizeToRead)
{
	char buffer[sizeToRead];
	size_t bytesRead = boost::asio::read(*socket,
			boost::asio::buffer(buffer, sizeToRead));
	if (bytesRead < sizeToRead)
	{
		throw GeneralException("Socket closed");
	}
	return convertToNum(buffer, sizeToRead);
}

void writeNum(boost::asio::ip::tcp::socket *socket, unsigned int num,
		unsigned short sizeToWrite)
{
	char buffer[sizeToWrite];
	convertToBytes(num, buffer, sizeToWrite);

	size_t bytesWrote = boost::asio::write(*socket,
			boost::asio::buffer(buffer, sizeToWrite));
	if (bytesWrote != sizeToWrite)
	{
		throw GeneralException("Incorrect write size detected");
	}
}

CommunicationSerilizationHelper::CommunicationSerilizationHelper(
		boost::asio::ip::tcp::socket *socket)
{
	this->socket = socket;
}

CommunicationSerilizationHelper::~CommunicationSerilizationHelper()
{
}

unsigned short CommunicationSerilizationHelper::readByte()
{
	return readNum(socket, 1);
}

void CommunicationSerilizationHelper::writeByte(unsigned short data)
{
	writeNum(socket, data, 1);
}

unsigned short CommunicationSerilizationHelper::readShort()
{
	return readNum(socket, 2);
}

void CommunicationSerilizationHelper::writeShort(unsigned short data)
{
	writeNum(socket, data, 2);
}

unsigned int CommunicationSerilizationHelper::readInt()
{
	return readNum(socket, 4);
}

void CommunicationSerilizationHelper::writeInt(unsigned int data)
{
	writeNum(socket, data, 4);
}

std::string CommunicationSerilizationHelper::readStr()
{
	unsigned short size = readShort();
	if (size > MAX_STR_LEN)
	{
		throw GeneralException("Str is over the maximum length");
	}

	char buffer[size + 1];
	size_t bytesRead = boost::asio::read(*socket,
			boost::asio::buffer(buffer, size));
	if (bytesRead < size)
	{
		throw GeneralException("Socket closed");
	}
	buffer[size] = 0;
	return std::string(buffer);
}

void CommunicationSerilizationHelper::writeStr(std::string data)
{
	unsigned short dataLen = data.length();
	writeShort(dataLen);
	size_t bytesWrote = boost::asio::write(*socket,
			boost::asio::buffer(data, data.length()));
	if (bytesWrote != data.length())
	{
		throw GeneralException("Incorrect write size detected");
	}

}

ByteBuffer* CommunicationSerilizationHelper::readBytes()
{
	unsigned int size = readInt();

	if (size > MAX_BYTES_LEN)
	{
		throw GeneralException("Bytes is over the maximum length");
	}

	return new SocketByteBuffer(this->socket, size);
}

void CommunicationSerilizationHelper::writeBytes(ByteBuffer *data)
{
	char buffer[BUFFER_SIZE];
	writeInt(data->getBytesLeft());
	while (data->getBytesLeft() > 0)
	{
		unsigned short dataRead = data->readData(buffer, BUFFER_SIZE);
		size_t bytesWrote = boost::asio::write(*socket,
				boost::asio::buffer(buffer, dataRead));
		if (bytesWrote != dataRead)
		{
			throw GeneralException("Incorrect write size detected");
		}
	}
}
