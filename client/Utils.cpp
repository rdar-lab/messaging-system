/*
 * Utils.cpp
 *
 *  Created on: 14 Feb 2021
 *      Author: Roy Dar
 */

#include "Utils.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

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


static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Utils::base64Encode(const void *buff, unsigned short sizeOfBuffer) {
	  std::string ret;
	  int i = 0;
	  int j = 0;
	  unsigned char char_array_3[3];
	  unsigned char char_array_4[4];
	  unsigned char *buf = (unsigned char*)buff;

	  while (sizeOfBuffer--) {
	    char_array_3[i++] = *(buf++);
	    if (i == 3) {
	      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	      char_array_4[3] = char_array_3[2] & 0x3f;

	      for(i = 0; (i <4) ; i++)
	        ret += base64_chars[char_array_4[i]];
	      i = 0;
	    }
	  }

	  if (i)
	  {
	    for(j = i; j < 3; j++)
	      char_array_3[j] = '\0';

	    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	    char_array_4[3] = char_array_3[2] & 0x3f;

	    for (j = 0; (j < i + 1); j++)
	      ret += base64_chars[char_array_4[j]];

	    while((i++ < 3))
	      ret += '=';
	  }

	  return ret;

}

unsigned short Utils::base64Decode(std::string b64, const void *buff,
		unsigned short sizeOfBuffer) {
	  int in_len = b64.size();
	  int i = 0;
	  int j = 0;
	  int in_ = 0;
	  unsigned char char_array_4[4], char_array_3[3];
	  int buffWriteLoc = 0;

	  while (in_len-- && ( b64[in_] != '=') && is_base64(b64[in_])) {
	    char_array_4[i++] = b64[in_]; in_++;
	    if (i ==4) {
	      for (i = 0; i <4; i++){
	        char_array_4[i] = base64_chars.find(char_array_4[i]);
	      }

	      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	      for (i = 0; (i < 3); i++){
	    	  ((char*)buff)[buffWriteLoc] = char_array_3[i];
	    	  buffWriteLoc++;
	      }
	      i = 0;
	    }
	  }

	  if (i) {
	    for (j = i; j <4; j++){
	      char_array_4[j] = 0;
	    }

	    for (j = 0; j <4; j++){
	      char_array_4[j] = base64_chars.find(char_array_4[j]);
	    }

	    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	    for (j = 0; (j < i - 1); j++){
	    	((char*)buff)[buffWriteLoc] = char_array_3[j];
	    	buffWriteLoc++;
	    }
	  }

	  return buffWriteLoc;
}

std::string Utils::generateTmpFilename() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    std::stringstream uuid_stream;
    uuid_stream << uuid;

	return std::string(P_tmpdir) + std::string("/") + uuid_stream.str();
}
