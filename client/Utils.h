/*
 * Utils.h
 *
 *  Created on: 14 Feb 2021
 *      Author: xmaster
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>

class Utils {
public:
	static unsigned int convertToNum(void *buff, unsigned short sizeOfBuffer);
	static void convertToBytes(unsigned int num, void *buff, unsigned short sizeOfBuffer);
	static std::string writeBytestoStr(const void *buff, unsigned short sizeOfBuffer);
	static void readBytesFromStr(std::string source, void *buff, unsigned short sizeOfBuffer);
	static std::string base64Encode(const void *buff, unsigned short sizeOfBuffer);
	static unsigned short base64Decode(std::string b64, const void *buff, unsigned short sizeOfBuffer);
	virtual ~Utils();
private:
	Utils();
};

#endif /* UTILS_H_ */
