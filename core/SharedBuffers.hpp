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

	size_t read(uint8_t* data, size_t size) {
		return (*streamInterface->read)(instance, data, size);
	}

	void write(const uint8_t* data, size_t size) {
		(*streamInterface->write)(instance, data, size);
	}

	void writeOther(uint32_t sourceId, size_t size) {
		return (*streamInterface->writeOther)(instance, sourceId, size);
	}

	size_t getBytesLeft() {
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

	size_t getReadPos() {
		return (*bufferInterface->getReadPos)(instance);
	}

	size_t getWritePos() {
		return (*bufferInterface->getWritePos)(instance);
	}

	size_t getLength() {
		return (*bufferInterface->getLength)(instance);
	}

	void setReadPos(size_t pos) {
		(*bufferInterface->setReadPos)(instance, pos);
	}

	void setWritePos(size_t pos) {
		(*bufferInterface->setWritePos)(instance, pos);
	}

	void setLength(size_t length) {
		(*bufferInterface->setLength)(instance, length);
	}
};

}
