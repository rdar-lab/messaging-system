/*
 * PublicKey.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef PUBLICKEY_H_
#define PUBLICKEY_H_

#include "BytesContainer.h"
#include "Constants.h"

/*
 * A public key in the system. A BytesContainer instance of PUBLIC_KEY_SIZE size
 */
class PublicKey : public BytesContainer<PUBLIC_KEY_SIZE>{
public:
	PublicKey() : BytesContainer(){};
	PublicKey(unsigned char* source):BytesContainer(source){};
	PublicKey(std::string str):BytesContainer(str){};
	PublicKey(const PublicKey &other):BytesContainer(other){};
};

#endif /* PUBLICKEY_H_ */
