/*
 * RequestReader.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef REQUESTREADER_H_
#define REQUESTREADER_H_

#include "Request.h"
#include <boost/asio.hpp>

/*
 * RequestReader
 * -------------
 * Reads the data from the socket and returns a "request" object
 */
class RequestReader
{
public:
	/*
	 * Ctor
	 */
	RequestReader(boost::asio::ip::tcp::socket *socket);

	/*
	 * Reads from the socket and returns the request
	 */
	Request* readRequest();

	/*
	 * Dtor
	 */
	virtual ~RequestReader();
private:
	// The socket to use
	boost::asio::ip::tcp::socket *socket;
};
#endif /* REQUESTREADER_H_ */

