/*
 * ResponseWriter.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef RESPONSEREADER_H_
#define RESPONSEREADER_H_

#include "Response.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/*
 * ResponseReader
 * --------------
 * Responsible for writing a response to the socket
 */
class ResponseReader
{
public:
	/*
	 * Ctor
	 */
	ResponseReader(boost::shared_ptr<tcp::socket> socket);

	Response * readResponse();

	/*
	 * Dtor
	 */
	virtual ~ResponseReader();
private:
	// The socket to use
	boost::shared_ptr<tcp::socket> socket;
};

#endif /* RESPONSEREADER_H_ */
