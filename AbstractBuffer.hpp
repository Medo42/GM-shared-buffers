#pragma once

#include <stddef.h>
#include <stdint.h>

namespace shb {

class AbstractStream {
public:
	virtual size_t read(uint8_t* data, size_t size) = 0;
	virtual void write(const uint8_t* data, size_t size) = 0;
	virtual size_t getBytesLeft() = 0;

	virtual ~AbstractStream() {}
};

class AbstractBuffer : public AbstractStream {
public:
	virtual size_t getReadPos() = 0;
	virtual size_t getWritePos() = 0;
	virtual size_t getLength() = 0;
	virtual void setReadPos(size_t pos) = 0;
	virtual void setWritePos(size_t pos) = 0;
	virtual uint8_t setLength(size_t length) = 0;
	virtual uint8_t* getData() = 0;

	virtual ~AbstractBuffer() {}
};

}
