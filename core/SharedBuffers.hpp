#pragma once

#include "core.hpp"
#include "NullBuffer.hpp"

namespace shb_internal {

class SharedStream {
protected:
	void *instance;
	shb_StreamInterface* streamInterface;

	void *destroyHandler;
	uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId);

public:
	SharedStream(
			void *instance,
			shb_StreamInterface* streamInterface,
			void *destroyHandler,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) :
		instance(instance),
		streamInterface(streamInterface),
		destroyHandler(destroyHandler),
		destroyCallback(destroyCallback) {}

	SharedStream() :
		instance(0),
		streamInterface(&nullStreamInterface) ,
		destroyHandler(0),
		destroyCallback(&nullDestroyHandler) {}

	size_t read(uint8_t* data, size_t size) {
		return (*streamInterface->read)(instance, data, size);
	}

	void write(const uint8_t* data, size_t size) {
		(*streamInterface->write)(instance, data, size);
	}

	size_t getBytesLeft() {
		return (*streamInterface->getBytesLeft)(instance);
	}

	uint8_t destroy(uint32_t id) {
		return (*destroyCallback)(destroyHandler, id);
	}
};

class SharedBuffer : public SharedStream {
protected:
	shb_BufferInterface* bufferInterface;

public:
	SharedBuffer(
			void* instance,
			shb_StreamInterface* streamInterface,
			shb_BufferInterface* bufferInterface,
			void *destroyHandler,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) :
		SharedStream(instance, streamInterface, destroyHandler, destroyCallback),
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

	uint8_t setLength(size_t length) {
		return (*bufferInterface->setLength)(instance, length);
	}

	uint8_t* getData() {
		return (*bufferInterface->getData)(instance);
	}
};

}
