/*
 * ConfigurationFileReader.cpp
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#include "ConfigurationFileReader.h"
#include <iostream>
#include <fstream>
#include <string>

ConfigurationFileReader::ConfigurationFileReader(
		std::string configurationFileName)
{
	std::ifstream confFile(configurationFileName);
	if (confFile.is_open())
	{
		std::string error_text = "";
		try
		{
			std::string line;
			getline(confFile, line);
			const std::size_t splitLoc = line.find(':');
			if (splitLoc == std::string::npos)
			{
				this->serverHost = "127.0.0.1";
				this->serverPort = std::stoi(line);
			}
			else
			{
				this->serverHost = line.substr(0, splitLoc);
				this->serverPort = std::stoi(line.substr(splitLoc + 1));
			}
		} catch (std::exception &exp)
		{
			error_text = exp.what();
		}

		confFile.close();

		if (error_text.length() > 0)
		{
			throw std::runtime_error(
					"Error parsing configuration file: " + error_text);
		}
	}

	else
	{
		throw std::runtime_error(
				"Unable to open configuration file " + configurationFileName);
	}
}

unsigned short ConfigurationFileReader::getServerPort()
{
	return this->serverPort;
}

std::string ConfigurationFileReader::getServerHost()
{
	return this->serverHost;
}

ConfigurationFileReader::~ConfigurationFileReader()
{
}
