/*
 * GeneralException.h
 *
 *  Created on: 1 Jan 2021
 *      Author: Roy Dar
 */

#ifndef GENERALEXCEPTION_H_
#define GENERALEXCEPTION_H_

#include <string>
#include <stdexcept>

/*
 * GeneralException
 * ----------------
 * A general exception class
 */
class GeneralException: public std::runtime_error
{
public:
	/*
	 * Ctor
	 */
	GeneralException(std::string exceptionMsg);

	/*
	 * Dtor
	 */
	virtual ~GeneralException();
};

#endif /* GENERALEXCEPTION_H_ */
