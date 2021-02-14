///*
// * CommunicationManager.cpp
// *
// *  Created on: 27 Dec 2020
// *      Author: Roy Dar
// */
//
//#include "CommunicationManager.h"
//#include <boost/asio.hpp>
//#include <iostream>
//#include <chrono>
//#include <thread>
//#include "Constants.h"
//#include "Request.h"
//#include "Response.h"
//#include "RequestReader.h"
//#include "ResponseWriter.h"
//#include "RequestHandlerFactory.h"
//
//CommunicationManager::CommunicationManager()
//{
//}
//
//CommunicationManager::~CommunicationManager()
//{
//}
//
//CommunicationManager *CommunicationManager::singleInstance = NULL;
//
//CommunicationManager* CommunicationManager::getInstance()
//{
//	// Check if the instance is initialized, and if not create it
//	if (CommunicationManager::singleInstance == NULL)
//	{
//		CommunicationManager::singleInstance = new CommunicationManager();
//	}
//	return CommunicationManager::singleInstance;
//}
//
//void CommunicationManager::runServer(std::string host, unsigned int port)
//{
//	std::cout << "Starting server..." << std::endl;
//	boost::asio::io_service io_service;
//
//	boost::asio::io_context io_context;
//
//	boost::asio::ip::address address = boost::asio::ip::address_v4::from_string(
//			host);
//
//	boost::asio::ip::tcp::acceptor acceptor(io_context,
//			boost::asio::ip::tcp::endpoint(address, port));
//	std::cout << "Server started on port " << port << std::endl;
//	for (;;)
//	{
//		if (this->numOfConnections >= MAX_NUMBER_OF_TCP_CONNECTIONS)
//		{
//			std::cout << "Max number of connections reached, waiting..."
//					<< std::endl;
//			while (this->numOfConnections >= MAX_NUMBER_OF_TCP_CONNECTIONS)
//			{
//				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//			}
//			std::cout << "Connections available, resuming listen" << std::endl;
//		}
//		socket_ptr socket(new boost::asio::ip::tcp::socket(io_service));
//		acceptor.accept(*socket);
//		std::cout << "New connection detected" << std::endl;
//		this->numOfConnections++;
//
//		// Threaded implementation
//		std::thread(CommunicationManager::handleClient, socket).detach();
//
//		// Non-Threaded implementation
////		CommunicationManager::handleClient(socket);
//	}
//
//	std::cout << "Incorrect state detected, server quit" << std::endl;
//}
//
//void CommunicationManager::handleClient(socket_ptr socket)
//{
//	std::string remote_address = socket->remote_endpoint().address().to_string()
//			+ ":" + std::to_string(socket->remote_endpoint().port());
//
//	std::cout << remote_address << ": Handling new client request" << std::endl;
//	Request *req = NULL;
//	Response *resp = NULL;
//
//	try
//	{
//		try
//		{
//			std::cout << remote_address << ": Reading request" << std::endl;
//			RequestReader reader(&(*socket));
//			req = reader.readRequest();
//
//			std::cout << remote_address << ": Handling request: " << *req
//					<< std::endl;
//			RequestHandler *handler =
//					RequestHandlerFactory::getInstance()->getHandlerForRequest(
//							req);
//			if (handler == NULL)
//			{
//				std::cout << remote_address << ": Unknown request" << std::endl;
//				resp = new Response(GENERAL_FAILURE_STATUS);
//			}
//			else
//			{
//				resp = handler->handle(req);
//			}
//
//		} catch (std::exception &exp)
//		{
//			std::cerr << remote_address << ": Exception in handling request: "
//					<< exp.what() << std::endl;
//			resp = new Response(GENERAL_FAILURE_STATUS);
//		}
//
//		std::cout << remote_address << ": Sending response: " << *resp
//				<< std::endl;
//		ResponseWriter writer(&(*socket));
//		writer.writeResponse(resp);
//	} catch (std::exception &exp)
//	{
//		std::cerr << remote_address << ": Exception  in client thread: "
//				<< exp.what() << std::endl;
//	}
//
//	if (req != NULL)
//	{
//		delete req;
//	}
//
//	if (resp != NULL)
//	{
//		delete resp;
//	}
//	CommunicationManager::getInstance()->numOfConnections--;
//	socket->close();
//	std::cout << remote_address << ": Request handling finished" << std::endl;
//
//}
