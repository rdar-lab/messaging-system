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
 * RequestWriter
 * -------------
 * Reads the data from the socket and returns a "request" object
 */
class RequestWriter
{
public:
	/*
	 * Ctor
	 */
	RequestWriter(boost::shared_ptr<tcp::socket> socket);

	/*
	 * Reads from the socket and returns the request
	 */
	void writeRequest(Request*);

	/*
	 * Dtor
	 */
	virtual ~RequestWriter();
private:
	// The socket to use
	boost::shared_ptr<tcp::socket> socket;
};
#endif /* REQUESTWRITER_H_ */

