/*
 * Response.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <string>
#include "ByteBuffer.h"

/*
 * Response
 * --------
 * An object representation of a response in our system
 */
class Response
{
public:
	Response(unsigned short version, unsigned short responseCode, ByteBuffer *payload);
	virtual ~Response();

	unsigned short getVersion() const;
	unsigned short getResponseCode() const;
	ByteBuffer* getPayload() const;

	friend std::ostream& operator<<(std::ostream &os, const Response &resp);
private:
	ByteBuffer *payload = NULL;

};
#endif /* RESPONSE_H_ */

