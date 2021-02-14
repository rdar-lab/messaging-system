/*
 * PublicKey.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef PUBLICKEY_H_
#define PUBLICKEY_H_

#include "BytesContainer.h"
#include "Constants.h"

class PublicKey : public BytesContainer<PUBLIC_KEY_SIZE>{
public:
	PublicKey() : BytesContainer(){};
	PublicKey(unsigned short* source):BytesContainer(source){};
	PublicKey(std::string str):BytesContainer(str){};
	PublicKey(const PublicKey &other):BytesContainer(other){};
};

#endif /* PUBLICKEY_H_ */
