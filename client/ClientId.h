/*
 * ClientId.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef CLIENTID_H_
#define CLIENTID_H_

#include "BytesContainer.h"
#include "Constants.h"

class ClientId : public BytesContainer<CLIENT_ID_SIZE>
{
public:
	ClientId() : BytesContainer(){};
	ClientId(unsigned short* source):BytesContainer(source){};
	ClientId(std::string str):BytesContainer(str){};
	ClientId(const ClientId &other):BytesContainer(other){};
};

#endif /* CLIENTID_H_ */
