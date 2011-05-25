#pragma once

#include <stdint.h>
#include <stddef.h>

namespace shb {

class AbstractStream {
public:
	virtual size_t read(uint8_t* data, size_t size) = 0;
	virtual void write(const uint8_t* data, size_t size) = 0;
	virtual void writeOther(uint32_t sourceId, size_t size) = 0;
	virtual size_t getBytesLeft() = 0;
	virtual uint8_t destroy() = 0;

	virtual ~AbstractStream() {}
};

class AbstractBuffer : public AbstractStream {
public:
	virtual size_t getReadPos() = 0;
	virtual size_t getWritePos() = 0;
	virtual size_t getLength() = 0;
	virtual void setReadPos(size_t pos) = 0;
	virtual void setWritePos(size_t pos) = 0;
	virtual void setLength(size_t length) = 0;

	virtual ~AbstractBuffer() {}
};

uint32_t shareStream(AbstractStream *stream);
uint32_t shareBuffer(AbstractBuffer *buffer);

}
