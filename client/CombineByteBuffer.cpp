/*
 * CombineByteBuffer.cpp
 *
 *  Created on: 20 Feb 2021
 *      Author: xmaster
 */

#include "CombineByteBuffer.h"


CombineByteBuffer::CombineByteBuffer(ByteBuffer *first, ByteBuffer *second) {
	this->first = first;
	this->second = second;
}

CombineByteBuffer::~CombineByteBuffer() {
	delete this->first;
	delete this->second;
}

unsigned short int CombineByteBuffer::readDataInternal(void *destinationBuffer,
		unsigned short int lenToRead) {
	if (lenToRead>0 && this->first->getBytesLeft()>0){
		unsigned short amountRead = this->first->readDataInternal(destinationBuffer, lenToRead);
		if (amountRead == 0){
			throw std::runtime_error("EOF");
		}
		return amountRead;
	}
	if (lenToRead>0 && this->second->getBytesLeft()>0){
		unsigned short amountRead = this->second->readDataInternal(destinationBuffer, lenToRead);
		if (amountRead == 0){
			throw std::runtime_error("EOF");
		}
		return amountRead;
	}

	throw std::runtime_error("EOF");
}



unsigned int CombineByteBuffer::getBytesLeft() {
	return this->first->getBytesLeft() + this->second->getBytesLeft();
}
