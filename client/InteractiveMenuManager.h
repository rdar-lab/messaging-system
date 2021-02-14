/*
 * InteractiveMenuManager.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef INTERACTIVEMENUMANAGER_H_
#define INTERACTIVEMENUMANAGER_H_

class InteractiveMenuManager {
public:
	InteractiveMenuManager();
	virtual ~InteractiveMenuManager();
	int showMenuAndGetUserAction();
};

#endif /* INTERACTIVEMENUMANAGER_H_ */
