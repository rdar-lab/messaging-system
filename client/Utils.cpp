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
	char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	std::string res = "";
	for( int i = 0; i < sizeOfBuffer; ++i )
	{
	    unsigned char const byte = ((char*)buff)[i];

	    res += hex_chars[ ( byte & 0xF0 ) >> 4 ];
	    res += hex_chars[ ( byte & 0x0F ) >> 0 ];
	}

	return res;
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
