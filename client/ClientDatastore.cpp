/*
 * ClientDatastore.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientDatastore.h"
#include <string>
#include <iostream>

ClientDatastore::ClientDatastore() {

}

ClientDatastore::~ClientDatastore() {
}

ClientDatastore *ClientDatastore::singleInstance = NULL;

ClientDatastore* ClientDatastore::getInstance()
{
	return ClientDatastore::singleInstance;
}

void ClientDatastore::setInstance(ClientDatastore *instance) {
	ClientDatastore::singleInstance = instance;
}

std::list<ClientDef> ClientDatastore::getAllClients() {
	std::list<ClientDef> valueList;
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
	  valueList.push_back( it->second );
	}
	return valueList;
}

void ClientDatastore::clearClients() {
	clients.clear();
}

void ClientDatastore::addClient(ClientDef client) {
	auto existingElm = clients.find(client.getId());
	if (existingElm != clients.end())
	{
		throw std::runtime_error("Client already exists");
	}

	clients[client.getId()] = client;
}

ClientDef ClientDatastore::removeClient(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client not found");
	}

	ClientDef existingClient = existingElm-> second;
	clients.erase(existingElm);
	return existingClient;
}

ClientDef ClientDatastore::getClient(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client not found");
	}

	ClientDef existingClient = existingElm-> second;
	return existingClient;
}

int ClientDatastore::clientExists(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	return existingElm != clients.end();
}

int ClientDatastore::clientExists(std::string name) {
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
		ClientDef client = it->second;
		if ( client.getName() == name ){
		  return true;
		}
	}
	return false;
}

ClientDef ClientDatastore::getClientByName(std::string name) {
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
		ClientDef client = it->second;
		if ( client.getName() == name ){
		  return client ;
		}
	}
	throw std::runtime_error("Client not found");
}

void ClientDatastore::updateClient(ClientDef client) {
	auto existingElm = clients.find(client.getId());
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client does not exist");
	}
	clients[client.getId()] = client;
}

void ClientDatastore::printClientsList() {
	std::cout << "* Clients:" << std::endl;
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
		ClientDef client = it->second;
		std::cout << "\t" << client << std::endl;
	}
}
