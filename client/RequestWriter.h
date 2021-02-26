/*
 * RequestReader.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef REQUESTWRITER_H_
#define REQUESTWRITER_H_

#include "Request.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/*
 * Writes a "request" object to a socket
 */
class RequestWriter
{
public:
	RequestWriter(boost::shared_ptr<tcp::socket> socket);

	void writeRequest(Request*);

	virtual ~RequestWriter();
private:
	// The socket to use
	boost::shared_ptr<tcp::socket> socket;
};
#endif /* REQUESTWRITER_H_ */

