/*
 * SymmetricKey.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef SYMMETRICKEY_H_
#define SYMMETRICKEY_H_

#include "BytesContainer.h"
#include "Constants.h"

class SymmetricKey : public BytesContainer<SYMMETRIC_KEY_SIZE>{
public:
	SymmetricKey() : BytesContainer(){};
	SymmetricKey(unsigned short* source):BytesContainer(source){};
	SymmetricKey(std::string str):BytesContainer(str){};
	SymmetricKey(const SymmetricKey &other):BytesContainer(other){};
};

#endif /* SYMMETRICKEY_H_ */
