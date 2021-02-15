/*
 * CommunicationManager.h
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#include <string>
#include "Request.h"
#include "Response.h"

/*
 * CommunicationManager
 * --------------------
 * The communication manager is responsible for the TCP layer.
 * This is a singleton class.
 *
 */
class CommunicationManager
{
public:
	/*
	 * static method to get the single instance
	 */
	static CommunicationManager* getInstance();

	/*
	 * Dtor
	 */
	virtual ~CommunicationManager();

	/*
	 * Runs the TCP server using the network card designated by 'host' and the 'port'
	 */
	void setParams(std::string host, unsigned int port);

	Response*  sendRequest(Request* req);

private:
	/*
	 * Ctor - private because this is a singleton
	 */
	CommunicationManager();

	/*
	 * The single instance
	 */
	static CommunicationManager *singleInstance;

	std::string host;
	unsigned int port;
};

#endif /* COMMUNICATIONMANAGER_H_ */
