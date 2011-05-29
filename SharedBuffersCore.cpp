#include "SharedBuffersCore.hpp"
#include "core/public_core_api.hpp"

namespace shb {

SharedBuffersCore::SharedBuffersCore(shb_CoreApi* coreApi) :
		coreApi(coreApi) {}

const shb_CoreApi* SharedBuffersCore::getCoreApi() {
	return coreApi;
}

// Functions applicable to streams and buffers
void SharedBuffersCore::read(uint32_t id, uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(id);
	if(stream) {
		(*stream->streamInterface->read)(stream->implementation, data, size);
	}
}

void SharedBuffersCore::write(uint32_t id, const uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(id);
	if(stream) {
		(*stream->streamInterface->write)(stream->implementation, data, size);
	}
}

size_t SharedBuffersCore::getBytesLeft(uint32_t id) {
	shb_Stream* stream = (*coreApi->findStream)(id);
	if(stream) {
		return (*stream->streamInterface->getBytesLeft)(stream->implementation);
	} else {
		return 0;
	}
}

// Functions applicable to buffers only
size_t SharedBuffersCore::getReadPos(uint32_t id) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		return (*buffer->bufferInterface->getReadPos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t SharedBuffersCore::getWritePos(uint32_t id) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		return (*buffer->bufferInterface->getWritePos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t SharedBuffersCore::getLength(uint32_t id) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		return (*buffer->bufferInterface->getLength)(buffer->implementation);
	} else {
		return 0;
	}
}

void SharedBuffersCore::setReadPos(uint32_t id, size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		(*buffer->bufferInterface->setReadPos)(buffer->implementation, pos);
	}
}

void SharedBuffersCore::setWritePos(uint32_t id, size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		(*buffer->bufferInterface->setWritePos)(buffer->implementation, pos);
	}
}

bool SharedBuffersCore::setLength(uint32_t id, size_t length) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(id);
	if(buffer) {
		return (*buffer->bufferInterface->setLength)(buffer->implementation, length);
	} else {
		return false;
	}
}

bool SharedBuffersCore::streamExists(uint32_t id) {
	return (*coreApi->findStream)(id) != 0;
}

bool SharedBuffersCore::bufferExists(uint32_t id) {
	return (*coreApi->findBuffer)(id) != 0;
}

void SharedBuffersCore::destroy(uint32_t id) {
	(*coreApi->destroy)(id);
}

}
