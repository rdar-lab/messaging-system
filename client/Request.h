/*
 * Request.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <string>
#include "ByteBuffer.h"
#include "ClientId.h"

/*
 * Request
 * -------
 * An object implementation of the request in our system
 */
class Request
{
public:
	Request(
			ClientId clientId,
			unsigned short version,
			unsigned short requestCode,
			ByteBuffer *payload);
	virtual ~Request();

	ClientId getClientId() const;
	unsigned short getVersion() const;
	unsigned short getRequestCode() const;
	ByteBuffer* getPayload() const;
	friend std::ostream& operator<<(std::ostream &os, const Request &req);
private:
	ByteBuffer *payload = NULL;
	ClientId clientId;
	unsigned short version;
	unsigned short requestCode;
};
#endif /* REQUEST_H_ */

