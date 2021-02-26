/*
 * TestingManager.h
 *
 *  Created on: 18 Feb 2021
 *      Author: Roy Dar
 */

#ifndef TESTINGMANAGER_H_
#define TESTINGMANAGER_H_

/*
 * The testing manager is responsible for running the full suite of automatic tests for the client
 */
class TestingManager {
public:
	static void performTests();
	virtual ~TestingManager();
private:
	TestingManager();
};

#endif /* TESTINGMANAGER_H_ */
