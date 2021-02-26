/*
 * InMemClientDatastore.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: Roy Dar
 */

#include "InMemClientDatastore.h"

InMemClientDatastore::InMemClientDatastore() {

}

InMemClientDatastore::~InMemClientDatastore() {
}

std::list<ClientDef> InMemClientDatastore::getAllClients() {
	std::list<ClientDef> valueList;
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
	  valueList.push_back( it->second );
	}
	return valueList;
}

void InMemClientDatastore::clearClients() {
	clients.clear();
}

void InMemClientDatastore::addClient(ClientDef client) {
	auto existingElm = clients.find(client.getId());
	if (existingElm != clients.end())
	{
		throw std::runtime_error("Client already exists");
	}

	clients[client.getId()] = client;
}

ClientDef InMemClientDatastore::removeClient(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client not found");
	}

	ClientDef existingClient = existingElm-> second;
	clients.erase(existingElm);
	return existingClient;
}

ClientDef InMemClientDatastore::getClient(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client not found");
	}

	ClientDef existingClient = existingElm-> second;
	return existingClient;
}

int InMemClientDatastore::clientExists(ClientId clientId) {
	auto existingElm = clients.find(clientId);
	return existingElm != clients.end();
}

int InMemClientDatastore::clientExists(std::string name) {
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
		ClientDef client = it->second;
		if ( client.getName() == name ){
		  return true;
		}
	}
	return false;
}

ClientDef InMemClientDatastore::getClientByName(std::string name) {
	for (std::map<ClientId, ClientDef>::const_iterator it=clients.begin(); it!=clients.end(); ++it) {
		ClientDef client = it->second;
		if ( client.getName() == name ){
		  return client ;
		}
	}
	throw std::runtime_error("Client not found");
}

void InMemClientDatastore::updateClient(ClientDef client) {
	auto existingElm = clients.find(client.getId());
	if (existingElm == clients.end())
	{
		throw std::runtime_error("Client does not exist");
	}
	clients[client.getId()] = client;
}
