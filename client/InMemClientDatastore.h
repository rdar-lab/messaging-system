/*
 * InMemClientDatastore.h
 *
 *  Created on: 13 Feb 2021
 *      Author: xmaster
 */

#ifndef INMEMCLIENTDATASTORE_H_
#define INMEMCLIENTDATASTORE_H_

#include "ClientDatastore.h"

class InMemClientDatastore : public ClientDatastore {
public:
	InMemClientDatastore();
	virtual ~InMemClientDatastore();
};

#endif /* INMEMCLIENTDATASTORE_H_ */
