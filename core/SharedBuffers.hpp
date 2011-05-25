#pragma once

#include "core.hpp"
#include "NullBuffer.hpp"

namespace shb_internal {

class SharedStream {
protected:
	void *instance;
	shb_StreamInterface* streamInterface;

public:
	SharedStream(void *instance, shb_StreamInterface* streamInterface) :
		instance(instance),
		streamInterface(streamInterface) {}

	SharedStream() : instance(0), streamInterface(&nullStreamInterface) {}

	uint32_t read(uint8_t* data, uint32_t size) {
		return (*streamInterface->read)(instance, data, size);
	}

	void write(const uint8_t* data, uint32_t size) {
		(*streamInterface->write)(instance, data, size);
	}

	uint32_t getBytesLeft() {
		return (*streamInterface->getBytesLeft)(instance);
	}

	uint8_t destroy() {
		return (*streamInterface->destroy)(instance);
	}
};

class SharedBuffer : public SharedStream {
protected:
	shb_BufferInterface* bufferInterface;

public:
	SharedBuffer(void *instance, shb_StreamInterface* streamInterface, shb_BufferInterface* bufferInterface) :
		SharedStream(instance, streamInterface),
		bufferInterface(bufferInterface) {}

	SharedBuffer() : SharedStream(), bufferInterface(&nullBufferInterface) {}

	uint32_t getReadPos() {
		return (*bufferInterface->getReadPos)(instance);
	}

	uint32_t getWritePos() {
		return (*bufferInterface->getWritePos)(instance);
	}

	uint32_t getLength() {
		return (*bufferInterface->getLength)(instance);
	}

	void setReadPos(uint32_t pos) {
		(*bufferInterface->setReadPos)(instance, pos);
	}

	void setWritePos(uint32_t pos) {
		(*bufferInterface->setWritePos)(instance, pos);
	}

	void setLength(uint32_t length) {
		(*bufferInterface->setLength)(instance, length);
	}
};

}
