/*
 * ConfigurationFileReader.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#ifndef CONFIGURATIONFILEREADER_H_
#define CONFIGURATIONFILEREADER_H_
#include <string>

/*
 * ConfigurationFileReader
 * -----------------------
 * Reads the configuration file for the server.
 * Supports both a format of file that contains 'host:port' or just 'port'
 */
class ConfigurationFileReader
{
public:
	/*
	 * Ctor
	 */
	ConfigurationFileReader(std::string configurationFileName);

	/*
	 * Returns the server port from the configuration
	 */
	unsigned short getServerPort();

	/*
	 * Returns the server host from the configuration
	 */
	std::string getServerHost();

	/*
	 * Dtor
	 */
	virtual ~ConfigurationFileReader();
private:
	unsigned int serverPort;
	std::string serverHost;
};

#endif /* CONFIGURATIONFILEREADER_H_ */
