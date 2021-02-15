/*
 * InteractiveMenuManager.cpp
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#include "InteractiveMenuManager.h"

#include <iostream>

InteractiveMenuManager::InteractiveMenuManager() {
}

InteractiveMenuManager::~InteractiveMenuManager() {
}

int InteractiveMenuManager::showMenuAndGetUserAction() {
	std::cout << "MessageU client at your service." << std::endl;
	std::cout << std::endl;
	std::cout << "1) Register" << std::endl;
	std::cout << "2) Request for clients list" << std::endl;
	std::cout << "3) Request for public key" << std::endl;
	std::cout << "4) Request for waiting messages" << std::endl;
	std::cout << "5) Send a text message" << std::endl;
	std::cout << "51) Send a request for symmetric key" << std::endl;
	std::cout << "52) Send your symmetric key" << std::endl;
	std::cout << "0) Exit client" << std::endl;
	std::cout << "? ";

	std::string selection;
	std::cin >> selection;
	return std::stoi(selection);
}
