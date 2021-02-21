/*
 * InteractiveMenuManager.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef INTERACTIVEMENUMANAGER_H_
#define INTERACTIVEMENUMANAGER_H_

#include <string>


class InteractiveMenuManager {
public:
	InteractiveMenuManager();
	virtual ~InteractiveMenuManager();
	void goIntoInteractiveMode();
private:
	int showMenuAndGetUserAction();
	int performAction(int selectedOption);
	std::string promptAndGetClient();
	void performRegister();
	void performGetClientsList();
	void performGetClientPublicKey();
	void performGetMessages();
	void performSendFileMessage();
	void performSendTextMessage();
	void performSendRequestForSymmetricKey();
	void performSendSymmetricKey();
};

#endif /* INTERACTIVEMENUMANAGER_H_ */
