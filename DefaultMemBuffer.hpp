#pragma once

#include "AbstractBuffer.hpp"

#include <algorithm>
#include <cstring>

namespace shb {

class DefaultMemBuffer : public AbstractBuffer {
	static const size_t MIN_CAPACITY = 16;
	uint8_t* data;

	size_t readPos;
	size_t writePos;

	size_t capacity;
	size_t length;

public:
	DefaultMemBuffer();

	virtual size_t read(uint8_t* out, size_t size);
	virtual void write(const uint8_t *in, size_t size);
	virtual size_t getBytesLeft();

	virtual size_t getReadPos();
	virtual size_t getWritePos();
	virtual size_t getLength();
	virtual void setReadPos(size_t pos);
	virtual void setWritePos(size_t pos);
	virtual uint8_t setLength(size_t newLength);
	virtual uint8_t* getData();
};

}
