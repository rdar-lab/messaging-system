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


template <int N> class BytesContainer {
public:
	BytesContainer<N>();
	BytesContainer<N>(unsigned short* source);
	BytesContainer<N>(std::string str);
	BytesContainer<N>(const BytesContainer<N> &other);

	virtual ~BytesContainer<N>();
	int write(void *destination) const;
	BytesContainer<N> &operator =(const BytesContainer<N> &other);
	std::string toString();
	int isEmpty();
	template <int N2> friend bool operator<(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator>(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator==(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend bool operator!=(const BytesContainer<N2>& first, const BytesContainer<N2>& second);
	template <int N2> friend std::ostream& operator <<(std::ostream &os, const BytesContainer<N2> &container);
private:
	unsigned short buffer[N];
};

#endif /* BYTESCONTAINER_H_ */
