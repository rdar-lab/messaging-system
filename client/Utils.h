/*
 * Utils.h
 *
 *  Created on: 14 Feb 2021
 *      Author: Roy Dar
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>

/*
 * A general utilities class in the system
 */
class Utils {
public:
	/*
	 * Convert a byte array to a number (little-endian)
	 */
	static unsigned int convertToNum(void *buff, unsigned short sizeOfBuffer);

	/*
	 * Convert a number to byte representation (little-endian)
	 */
	static void convertToBytes(unsigned int num, void *buff, unsigned short sizeOfBuffer);

	/*
	 * Convert binary data to a HEX string
	 */
	static std::string writeBytestoStr(const void *buff, unsigned short sizeOfBuffer);

	/*
	 * Reads an HEX string to a binary data array
	 */
	static void readBytesFromStr(std::string source, void *buff, unsigned short sizeOfBuffer);

	/*
	 * Encodes binary data as Base64
	 */
	static std::string base64Encode(const void *buff, unsigned short sizeOfBuffer);

	/*
	 * Reads Base64 encoded string to a binary array
	 */
	static unsigned short base64Decode(std::string b64, const void *buff, unsigned short sizeOfBuffer);
	virtual ~Utils();
private:
	Utils();
};

#endif /* UTILS_H_ */
