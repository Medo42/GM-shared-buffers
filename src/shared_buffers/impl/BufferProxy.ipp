// Included from BufferProxy.hpp

#include "../core/public_core_api.hpp"

namespace shb {

inline StreamProxy::StreamProxy(const shb_CoreApi* coreApi, uint32_t handle) :
		coreApi(coreApi), handle(handle) {}

inline bool StreamProxy::streamExists() {
	return (*coreApi->findStream)(handle) != 0;
}

inline void StreamProxy::destroy() {
	if(streamExists()) {
		(*coreApi->destroy)(handle);
	}
}

inline size_t StreamProxy::read(uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->read)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t StreamProxy::peek(uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->peek)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t StreamProxy::skip(size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->skip)(stream->implementation, size);
	} else {
		return 0;
	}
}

inline size_t StreamProxy::write(const uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->write)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t StreamProxy::getBytesLeft() {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->getBytesLeft)(stream->implementation);
	} else {
		return 0;
	}
}

inline BufferProxy::BufferProxy(const shb_CoreApi* coreApi, uint32_t handle):
		coreApi(coreApi), handle(handle) {}

inline bool BufferProxy::bufferExists() {
	return (*coreApi->findBuffer)(handle) != 0;
}

inline void BufferProxy::destroy() {
	if(bufferExists()) {
		(*coreApi->destroy)(handle);
	}
}

inline size_t BufferProxy::read(uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->read)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::peek(uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->peek)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::skip(size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->skip)(buffer->implementation, size);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::write(const uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->write)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::getBytesLeft() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->getBytesLeft)(buffer->implementation);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::getReadPos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getReadPos)(buffer->implementation);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::getWritePos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getWritePos)(buffer->implementation);
	} else {
		return 0;
	}
}

inline size_t BufferProxy::getLength() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getLength)(buffer->implementation);
	} else {
		return 0;
	}
}

inline void BufferProxy::setReadPos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setReadPos)(buffer->implementation, pos);
	}
}

inline void BufferProxy::setWritePos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setWritePos)(buffer->implementation, pos);
	}
}

inline bool BufferProxy::setLength(size_t length) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->setLength)(buffer->implementation, length);
	} else {
		return false;
	}
}

}
