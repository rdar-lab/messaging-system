/*
 * ClientId.h
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#ifndef CLIENTID_H_
#define CLIENTID_H_

#include "BytesContainer.h"
#include "Constants.h"

/*
 * A clientID in the system. An instance of BytesContainer of CLIENT_ID_SIZE size
 */
class ClientId : public BytesContainer<CLIENT_ID_SIZE>
{
public:
	ClientId() : BytesContainer(){};
	ClientId(unsigned char* source):BytesContainer(source){};
	ClientId(std::string str):BytesContainer(str){};
	ClientId(const ClientId &other):BytesContainer(other){};
};

#endif /* CLIENTID_H_ */
