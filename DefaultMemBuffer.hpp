#pragma once

#include "AbstractBuffer.hpp"

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

	virtual void read(uint8_t* out, size_t size);
	virtual void write(const uint8_t *in, size_t size);

	virtual size_t getReadPos();
	virtual size_t getWritePos();
	virtual size_t getLength();
	virtual void setReadPos(size_t pos);
	virtual void setWritePos(size_t pos);
	virtual bool setLength(size_t newLength);
};

}
