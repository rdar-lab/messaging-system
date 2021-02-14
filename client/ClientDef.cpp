/*
 * ClientDef.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "ClientDef.h"

ClientDef::ClientDef() {
	this->name = "";
	this->id = ClientId();
	this->publicKey = PublicKey();
	this->symmetricKey = SymmetricKey();
}

ClientDef::~ClientDef() {
}

ClientDef::ClientDef(ClientId clientId, std::string clientName) {
	this->name = clientName;
	this->id = clientId;
	this->publicKey = PublicKey();
	this->symmetricKey = SymmetricKey();
}

ClientDef::ClientDef(const ClientDef &other) {
	this->name = other.getName();
	this->id = other.getId();
	this->publicKey = other.getPublicKey();
	this->symmetricKey = other.getSymmetricKey();
}

std::string ClientDef::getName() const{
	return this->name;
}

ClientId ClientDef::getId() const{
	return this->id;
}

PublicKey ClientDef::getPublicKey() const{
	return this->publicKey;
}

void ClientDef::setPublicKey(PublicKey publicKey) {
	this->publicKey = publicKey;
}

SymmetricKey ClientDef::getSymmetricKey() const{
	return this->symmetricKey;
}

void ClientDef::setSymmetricKey(SymmetricKey key) {
	this->symmetricKey = key;
}

std::ostream& operator <<(std::ostream &os, const ClientDef &client){
	os << "ID:" << client.getId() << ", Name:" << client.getName();

	os << ", PublicKey:";

	if (client.getPublicKey().isEmpty())
	{
		os << "-";
	} else
	{
		os << "+";
	}

	os << ", SymmetricKey:";

	if (client.getSymmetricKey().isEmpty())
	{
		os << "-";
	} else
	{
		os << "+";
	}

	return os;
}

ClientDef& ClientDef::operator =(const ClientDef &other) {
	if (this == &other){
		return *this;
	}

	this->name = other.getName();
	this->id = other.getId();
	this->publicKey = other.getPublicKey();
	this->symmetricKey = other.getSymmetricKey();

	return *this;
}
