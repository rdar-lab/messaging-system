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
 * Responsible for reading a request from the socket
 */
class ResponseReader
{
public:
	ResponseReader(boost::shared_ptr<tcp::socket> socket);

	Response * readResponse();

	virtual ~ResponseReader();
private:
	// The socket to use
	boost::shared_ptr<tcp::socket> socket;
};

#endif /* RESPONSEREADER_H_ */
