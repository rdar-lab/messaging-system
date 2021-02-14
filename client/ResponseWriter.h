/*
 * ResponseWriter.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef RESPONSEWRITER_H_
#define RESPONSEWRITER_H_

#include "Response.h"
#include <boost/asio.hpp>

/*
 * ResponseWriter
 * --------------
 * Responsible for writing a response to the socket
 */
class ResponseWriter
{
public:
	/*
	 * Ctor
	 */
	ResponseWriter(boost::asio::ip::tcp::socket *socket);

	/*
	 * Writes the response to the socket
	 */
	void writeResponse(Response *resp);

	/*
	 * Dtor
	 */
	virtual ~ResponseWriter();
private:
	// The socket to use
	boost::asio::ip::tcp::socket *socket;
};

#endif /* RESPONSEWRITER_H_ */
