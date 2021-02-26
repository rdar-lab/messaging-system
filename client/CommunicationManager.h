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
 * The communication manager is responsible for the TCP layer.
 * This is a singleton class.
 *
 */
class CommunicationManager
{
public:
	static CommunicationManager* getInstance();
	virtual ~CommunicationManager();

	void setParams(std::string host, unsigned int port);

	Response*  sendRequest(Request* req);

private:
	CommunicationManager();

	static CommunicationManager *singleInstance;

	std::string host;
	unsigned int port;
};

#endif /* COMMUNICATIONMANAGER_H_ */
