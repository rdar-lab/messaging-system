/*
 * SymmetricKey.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef SYMMETRICKEY_H_
#define SYMMETRICKEY_H_

#include "BytesContainer.h"
#include "Constants.h"

/*
 * A SymmetricKey in our system. An instance of BytesContainer of SYMMETRIC_KEY_SIZE size
 */
class SymmetricKey : public BytesContainer<SYMMETRIC_KEY_SIZE>{
public:
	SymmetricKey() : BytesContainer(){};
	SymmetricKey(unsigned char* source):BytesContainer(source){};
	SymmetricKey(std::string str):BytesContainer(str){};
	SymmetricKey(const SymmetricKey &other):BytesContainer(other){};
};

#endif /* SYMMETRICKEY_H_ */
