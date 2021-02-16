/*
 * CommunicationUtils.cpp
 *
 *  Created on: 2 Jan 2021
 *      Author: Roy Dar
 */

#include "CommunicationSerilizationHelper.h"
#include "SocketByteBuffer.h"
#include "Constants.h"
#include "Utils.h"


unsigned int readNum(boost::asio::ip::tcp::socket *socket,
		unsigned short sizeToRead)
{
	char buffer[sizeToRead];
	size_t bytesRead = boost::asio::read(*socket,
			boost::asio::buffer(buffer, sizeToRead));
	if (bytesRead < sizeToRead)
	{
		throw std::runtime_error("Socket closed");
	}
	return Utils::convertToNum(buffer, sizeToRead);
}

void writeNum(boost::asio::ip::tcp::socket *socket, unsigned int num,
		unsigned short sizeToWrite)
{
	char buffer[sizeToWrite];
	Utils::convertToBytes(num, buffer, sizeToWrite);

	size_t bytesWrote = boost::asio::write(*socket,
			boost::asio::buffer(buffer, sizeToWrite));
	if (bytesWrote != sizeToWrite)
	{
		throw std::runtime_error("Incorrect write size detected");
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

unsigned char CommunicationSerilizationHelper::readByte()
{
	return readNum(socket, 1);
}

void CommunicationSerilizationHelper::writeByte(unsigned char data)
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

std::string CommunicationSerilizationHelper::readStr(unsigned short size)
{
	if (size > MAX_STR_LEN)
	{
		throw std::runtime_error("Str is over the maximum length");
	}

	char buffer[size + 1];
	size_t bytesRead = boost::asio::read(*socket,
			boost::asio::buffer(buffer, size));
	if (bytesRead < size)
	{
		throw std::runtime_error("Socket closed");
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
		throw std::runtime_error("Incorrect write size detected");
	}

}

ByteBuffer* CommunicationSerilizationHelper::readBytes(unsigned int size)
{
	if (size > MAX_BYTES_LEN)
	{
		throw std::runtime_error("Bytes is over the maximum length");
	}

	return new SocketByteBuffer(this->socket, size);
}

void CommunicationSerilizationHelper::writeBytes(unsigned char *buffer, unsigned int len){
	size_t bytesWrote = boost::asio::write(*socket,
			boost::asio::buffer(buffer, len));
	if (bytesWrote != len)
	{
		throw std::runtime_error("Incorrect write size detected");
	}
}

void CommunicationSerilizationHelper::writeBytes(ByteBuffer *data)
{
	char buffer[BUFFER_SIZE];
	while (data->getBytesLeft() > 0)
	{
		unsigned short dataRead = data->readData(buffer, BUFFER_SIZE);
		size_t bytesWrote = boost::asio::write(*socket,
				boost::asio::buffer(buffer, dataRead));
		if (bytesWrote != dataRead)
		{
			throw std::runtime_error("Incorrect write size detected");
		}
	}
}
