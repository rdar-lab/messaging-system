/*
 * BytesContainer.cpp
 *
 *  Created on: 14 Feb 2021
 *      Author: xmaster
 */

#include "BytesContainer.h"
#include <iostream>
#include <cstring>
#include <string>
#include "Utils.h"


template <int N>
inline
BytesContainer<N>::BytesContainer() {
	std::memset(this->buffer, 0, sizeof(this->buffer));
}

template <int N>
inline
BytesContainer<N>::~BytesContainer() {
}

template <int N>
inline
BytesContainer<N>::BytesContainer(unsigned short *source) {
	std::copy(source, source + N, this->buffer);
}

template <int N>
inline
BytesContainer<N>::BytesContainer(std::string str) {
	Utils::readBytesFromStr(str, this->buffer, N);
}

template <int N>
inline
BytesContainer<N>::BytesContainer(const BytesContainer &other) {
	std::copy(other.buffer, other.buffer + N, this->buffer);
}

template <int N>
inline
int BytesContainer<N>::write(void *destination) const {
	std::copy(this->buffer, this->buffer + N, destination);
	return N;
}

template <int N>
inline
BytesContainer<N>& BytesContainer<N>::operator =(const BytesContainer<N> &other) {
	if (this == &other){
		return *this;
	}

	std::copy(other.buffer, other.buffer + N, this->buffer);
	return *this;
}

template <int N>
inline
bool operator<(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] < second.buffer[index]){
			return true;
		}
	}
	return false;
}

template <int N>
inline
bool operator>(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] > second.buffer[index]){
			return true;
		}
	}
	return false;
}

template <int N>
inline
bool operator==(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] != second.buffer[index]){
			return false;
		}
	}
	return true;
}

template <int N>
inline
bool operator!=(const BytesContainer<N>& first, const BytesContainer<N>& second){
	for (int index=0; index<N; index++){
		if (first.buffer[index] != second.buffer[index]){
			return true;
		}
	}
	return false;
}

template<int N>
inline
std::string BytesContainer<N>::toString() {
	return Utils::writeBytestoStr(buffer, N);
}

template<int N>
inline
int BytesContainer<N>::isEmpty() {
	for (int index=0; index<N; index++){
		if (this->buffer[index] != 0){
			return false;
		}
	}
	return true;
}

template<int N>
inline
std::ostream& operator <<(std::ostream &os, const BytesContainer<N> &container)
{
	os << container.toString();
	return os;
}
