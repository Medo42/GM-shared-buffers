#include "BufferProxy.hpp"
#include "core/public_core_api.hpp"

namespace shb {

StreamProxy::StreamProxy(const shb_CoreApi* coreApi, uint32_t handle) :
		coreApi(coreApi), handle(handle) {}

bool StreamProxy::streamExists() {
	return (*coreApi->findStream)(handle) != 0;
}

void StreamProxy::destroy() {
	if(streamExists()) {
		(*coreApi->destroy)(handle);
	}
}

void StreamProxy::read(uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		(*stream->streamInterface->read)(stream->implementation, data, size);
	}
}

void StreamProxy::write(const uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		(*stream->streamInterface->write)(stream->implementation, data, size);
	}
}

size_t StreamProxy::getBytesLeft() {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->getBytesLeft)(stream->implementation);
	} else {
		return 0;
	}
}

BufferProxy::BufferProxy(const shb_CoreApi* coreApi, uint32_t handle):
		coreApi(coreApi), handle(handle) {}

bool BufferProxy::bufferExists() {
	return (*coreApi->findBuffer)(handle) != 0;
}

void BufferProxy::destroy() {
	if(bufferExists()) {
		(*coreApi->destroy)(handle);
	}
}

void BufferProxy::read(uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->streamInterface->read)(buffer->implementation, data, size);
	}
}

void BufferProxy::write(const uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->streamInterface->write)(buffer->implementation, data, size);
	}
}

size_t BufferProxy::getBytesLeft() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->getBytesLeft)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getReadPos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getReadPos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getWritePos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getWritePos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getLength() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getLength)(buffer->implementation);
	} else {
		return 0;
	}
}

void BufferProxy::setReadPos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setReadPos)(buffer->implementation, pos);
	}
}

void BufferProxy::setWritePos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setWritePos)(buffer->implementation, pos);
	}
}

bool BufferProxy::setLength(size_t length) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->setLength)(buffer->implementation, length);
	} else {
		return false;
	}
}

BufferFragment BufferProxy::getFragment(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		shb_BufferFragment fragment;
		if((*buffer->bufferInterface->getFragment)(buffer->implementation, &fragment, pos)) {
			return BufferFragment(fragment.start, fragment.end);
		}
	}
	return BufferFragment();
}

}
