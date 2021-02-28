/*
 * CommunicationManager.cpp
 *
 *  Created on: 27 Dec 2020
 *      Author: Roy Dar
 */

#include "CommunicationManager.h"
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "Constants.h"
#include "Request.h"
#include "Response.h"
#include "RequestWriter.h"
#include "ResponseReader.h"

using boost::asio::ip::tcp;


CommunicationManager::CommunicationManager()
{
	this->host = "";
	this->port = 0;
	this->io_service = new boost::asio::io_service();
	this->io_context = new boost::asio::io_context();
	this->resolver = new tcp::resolver(*this->io_context);
}

CommunicationManager::~CommunicationManager()
{
	delete this->resolver;
	delete this->io_context;
	delete this->io_service;
}

CommunicationManager *CommunicationManager::singleInstance = NULL;

CommunicationManager* CommunicationManager::getInstance()
{
	// Check if the instance is initialized, and if not create it
	if (CommunicationManager::singleInstance == NULL)
	{
		CommunicationManager::singleInstance = new CommunicationManager();
	}
	return CommunicationManager::singleInstance;
}

void CommunicationManager::setParams(std::string host, unsigned int port) {
	this->host = host;
	this->port = port;
}

Response* CommunicationManager::sendRequest(Request *req) {
	boost::shared_ptr<tcp::socket> socket(new tcp::socket(*io_service));
	boost::asio::connect(*socket, resolver->resolve(host, std::to_string(port)));
	RequestWriter writer(socket);
	writer.writeRequest(req);
	ResponseReader reader(socket);
	Response* response = reader.readResponse();

	if (response->getResponseCode() == RESPONSE_CODE_GENERAL_ERROR)
	{
		delete response;
		throw std::runtime_error("Got general error response from server. Check server logs for details");
	}
	return response;
}
