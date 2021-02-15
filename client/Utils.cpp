/*
 * Utils.cpp
 *
 *  Created on: 14 Feb 2021
 *      Author: xmaster
 */

#include "Utils.h"
#include <iostream>
#include <string>
#include <iomanip>

Utils::Utils() {
}

Utils::~Utils() {
}

/*
 * Convert byte stream to a number based on the buffer and size
 * Uses little endian
 */
unsigned int Utils::convertToNum(void *buff, unsigned short sizeOfBuffer)
{
	unsigned int result = 0;
	for (int i = sizeOfBuffer - 1; i >= 0; i--)
	{
		result = result << 8;
		unsigned char val = ((char*)buff)[i];
		result = result + val;
	}
	return result;
}

/*
 * Converts a number to a buffer of bytes uses little endian
 */
void Utils::convertToBytes(unsigned int num, void *buff, unsigned short sizeOfBuffer)
{
	unsigned int temp = num;
	for (int i = 0; i < sizeOfBuffer; i++)
	{
		char currByte = temp & 0xFF;
		((char*)buff)[i] = currByte;
		temp = temp >> 8;
	}
}

std::string Utils::writeBytestoStr(const void *buff, unsigned short sizeOfBuffer) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < sizeOfBuffer; ++i)
	{
	    ss << std::setw(2) << static_cast<unsigned>(((char*)buff)[i]);
	}
	return ss.str();
}

int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

void Utils::readBytesFromStr(std::string source, void *buff,
	unsigned short sizeOfBuffer) {
	const char* src = source.c_str();
	char* dst = (char*)buff;
	int index = 0;
	while(src[0] && src[1] && index<sizeOfBuffer)
	  {
	    *(dst+index) = char2int(src[0])*16 + char2int(src[1]);
	    src += 2;
	    index++;
	  }
}
