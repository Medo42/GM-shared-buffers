#pragma once

#include "public_api.hpp"
#include "NullBuffer.hpp"

class SharedStream {
protected:
	void *instance;
	SharedStreamInterface* streamInterface;

public:
	SharedStream(void *instance, SharedStreamInterface* streamInterface) :
		instance(instance),
		streamInterface(streamInterface) {}

	SharedStream() : instance(0), streamInterface(&nullbuffer::streamInterface) {}

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
	SharedBufferInterface* bufferInterface;

public:
	SharedBuffer(void *instance, SharedStreamInterface* streamInterface, SharedBufferInterface* bufferInterface) :
		SharedStream(instance, streamInterface),
		bufferInterface(bufferInterface) {}

	SharedBuffer() : SharedStream(), bufferInterface(&nullbuffer::bufferInterface) {}

	uint32_t getreadpos() {
		return (*bufferInterface->getreadpos)(instance);
	}

	uint32_t getwritepos() {
		return (*bufferInterface->getwritepos)(instance);
	}

	uint32_t getlength() {
		return (*bufferInterface->getlength)(instance);
	}

	void setreadpos(uint32_t pos) {
		(*bufferInterface->setreadpos)(instance, pos);
	}

	void setwritepos(uint32_t pos) {
		(*bufferInterface->setwritepos)(instance, pos);
	}

	void setlength(uint32_t length) {
		(*bufferInterface->setlength)(instance, length);
	}
};
