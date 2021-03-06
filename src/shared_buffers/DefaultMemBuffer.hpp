#pragma once

#include "AbstractBuffer.hpp"

namespace shb {

class DefaultMemBuffer : public AbstractBuffer {
public:
	DefaultMemBuffer();

	virtual size_t read(uint8_t* out, size_t size);
	virtual size_t write(const uint8_t *in, size_t size);

	virtual size_t getReadPos();
	virtual size_t getWritePos();
	virtual size_t getLength();
	virtual void setReadPos(size_t pos);
	virtual void setWritePos(size_t pos);
	virtual bool setLength(size_t newLength);

private:
	// Default copying would cause Bad Things(tm) to happen, so we forbid it.
	DefaultMemBuffer(const DefaultMemBuffer&);
	DefaultMemBuffer& operator=(const DefaultMemBuffer&);

	static const size_t MIN_CAPACITY = 16;
	uint8_t* data;

	size_t readPos;
	size_t writePos;

	size_t capacity;
	size_t length;
};

}

#include "impl/DefaultMemBuffer.ipp"
