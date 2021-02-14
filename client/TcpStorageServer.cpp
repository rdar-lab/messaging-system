//============================================================================
// Name        : tcp-storage-server.cpp
// Author      : 
// Version     :
// Copyright   : All copyrights to Roy Dar
// License     : MIT
// Description : TCP files storage server
//============================================================================

#include <iostream>
#include "ConfigurationFileReader.h"
#include "CommunicationManager.h"
#include "Constants.h"
#include "RequestHandlerFactory.h"
#include "UploadFileHandler.h"
#include "DownloadFileHandler.h"
#include "DeleteFileHandler.h"
#include "GetFilesListHandler.h"
#include "FilesRepository.h"

int main()
{
	// Register handlers
	RequestHandlerFactory::getInstance()->registerHandler(
			new UploadFileHandler());
	RequestHandlerFactory::getInstance()->registerHandler(
			new DownloadFileHandler());
	RequestHandlerFactory::getInstance()->registerHandler(
			new DeleteFileHandler());
	RequestHandlerFactory::getInstance()->registerHandler(
			new GetFilesListHandler());

	// Make a call to init the storage location
	FilesRepository::getInstance();

	try
	{
		ConfigurationFileReader reader(SERVER_CONFIGURATION_FILE_LOCATION);
		std::string host = reader.getServerHost();
		unsigned short port = reader.getServerPort();

		std::cout << "String server at " << host << ":" << port << std::endl;
		CommunicationManager::getInstance()->runServer(host, port);
	} catch (std::exception &exp)
	{
		std::cout << "error: " << exp.what() << std::endl;
	}
	return 0;
}
