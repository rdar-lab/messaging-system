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
 * Reads the configuration file for the client.
 * The layout is 'host:port' or just 'port' (for automatic 127.0.0.1)
 */
class ConfigurationFileReader
{
public:
	ConfigurationFileReader(std::string configurationFileName);

	unsigned short getServerPort();

	std::string getServerHost();

	virtual ~ConfigurationFileReader();
private:
	unsigned int serverPort;
	std::string serverHost;
};

#endif /* CONFIGURATIONFILEREADER_H_ */
