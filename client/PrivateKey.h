/*
 * PrivateKey.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef PRIVATEKEY_H_
#define PRIVATEKEY_H_

#include "BytesContainer.h"
#include "Constants.h"

class PrivateKey : public BytesContainer<PRIVATE_KEY_SIZE>{
public:
	PrivateKey() : BytesContainer(){};
	PrivateKey(unsigned short* source):BytesContainer(source){};
	PrivateKey(std::string str):BytesContainer(str){};
	PrivateKey(const PrivateKey &other):BytesContainer(other){};
};

#endif /* PRIVATEKEY_H_ */
