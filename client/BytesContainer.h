/*
 * BytesContainer.h
 *
 *  Created on: 14 Feb 2021
 *      Author: xmaster
 */

#ifndef BYTESCONTAINER_H_
#define BYTESCONTAINER_H_

#include <string>
#include <iostream>
#include "Constants.h"
#include <iostream>
#include <cstring>
#include <string>
#include "Utils.h"


template <int N> class BytesContainer {
public:
	BytesContainer<N>(){
		std::memset(this->buffer, 0, sizeof(this->buffer));
	}
	BytesContainer<N>(unsigned char* source){
		std::copy(source, source + N, this->buffer);
	}

	BytesContainer<N>(std::string str){
		Utils::readBytesFromStr(str, this->buffer, N);
	}

	BytesContainer<N>(const BytesContainer<N> &other){
		std::copy(other.buffer, other.buffer + N, this->buffer);
	}

	virtual ~BytesContainer<N>(){

	}

	int write(void *destination) const{
		std::copy((char*)this->buffer, (char*)this->buffer + N, (char*)destination);
		return N;
	}

	BytesContainer<N> &operator =(const BytesContainer<N> &other){
		if (this == &other){
			return *this;
		}

		std::copy(other.buffer, other.buffer + N, this->buffer);
		return *this;
	}

	std::string toString() const{
		return Utils::writeBytestoStr(buffer, N);
	}

	int isEmpty(){
		for (int index=0; index<N; index++){
			if (this->buffer[index] != 0){
				return false;
			}
		}
		return true;
	}

	template <int N2> friend bool operator<(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator>(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator==(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator!=(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend std::ostream& operator <<(std::ostream &os, const BytesContainer<N2> &container);
private:
	unsigned char buffer[N];
};


template <int N>
bool operator<(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] < second.buffer[index]){
			return true;
		}

		if (first.buffer[index] > second.buffer[index]){
			return false;
		}
	}
	return false;
}

template <int N>
bool operator>(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] > second.buffer[index]){
			return true;
		}

		if (first.buffer[index] < second.buffer[index]){
			return false;
		}
}
	return false;
}

template <int N>
bool operator==(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] != second.buffer[index]){
			return false;
		}
	}
	return true;
}

template <int N>
bool operator!=(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] != second.buffer[index]){
			return true;
		}
	}
	return false;
}


template<int N>
std::ostream& operator <<(std::ostream &os, const BytesContainer<N> &container)
{
	os << container.toString();
	return os;
}



#endif /* BYTESCONTAINER_H_ */
