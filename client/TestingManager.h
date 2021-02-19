/*
 * TestingManager.h
 *
 *  Created on: 18 Feb 2021
 *      Author: xmaster
 */

#ifndef TESTINGMANAGER_H_
#define TESTINGMANAGER_H_

class TestingManager {
public:
	static void performTests();
	virtual ~TestingManager();
private:
	TestingManager();
};

#endif /* TESTINGMANAGER_H_ */
