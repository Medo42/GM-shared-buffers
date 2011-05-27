#include "SharedBuffersCore.hpp"

namespace shb_internal {
using namespace shb;

static __stdcall size_t read(void*, uint8_t*, size_t) { return 0; }
static __stdcall void write(void*, const uint8_t*, size_t) { }
static __stdcall size_t getBytesLeft(void*) { return 0; }

static __stdcall size_t getReadPos(void*) { return 0; }
static __stdcall size_t getWritePos(void*) { return 0; }
static __stdcall size_t getLength(void*) { return 0; }
static __stdcall void setReadPos(void*, size_t) { }
static __stdcall void setWritePos(void*, size_t) { }
static __stdcall uint8_t setLength(void*, size_t) { return false; }
static __stdcall uint8_t* getData(void*) { return 0; }

static shb_StreamInterface nullStreamInterface = {
	&read,
	&write,
	&getBytesLeft,
};

static shb_BufferInterface nullBufferInterface = {
	&getReadPos,
	&getWritePos,
	&getLength,
	&setReadPos,
	&setWritePos,
	&setLength,
	&getData
};

static shb_Buffer shbNullBuffer = {
	0,
	&nullStreamInterface,
	&nullBufferInterface
};

static shb_Stream shbNullStream = {
	0,
	&nullStreamInterface
};

}

namespace shb {
using namespace shb_internal;

static shb_Stream* findStream(const shb_CoreApi* core, uint32_t id) {
	shb_Stream* stream = (*core->findStream)(id);
	if(stream) {
		return stream;
	} else {
		return &shbNullStream;
	}
}

static shb_Buffer* findBuffer(const shb_CoreApi* core, uint32_t id) {
	shb_Buffer* buffer = (*core->findBuffer)(id);
	if(buffer) {
		return buffer;
	} else {
		return &shbNullBuffer;
	}
}

// Functions applicable to streams and buffers
size_t SharedBuffersCore::read(uint32_t id, uint8_t* data, size_t size) {
	shb_Stream* stream = findStream(coreApi, id);
	return (*stream->streamInterface->read)(stream->implementation, data, size);
}

void SharedBuffersCore::write(uint32_t id, const uint8_t* data, size_t size) {
	shb_Stream* stream = findStream(coreApi, id);
	return (*stream->streamInterface->write)(stream->implementation, data, size);
}

size_t SharedBuffersCore::getBytesLeft(uint32_t id) {
	shb_Stream* stream = findStream(coreApi, id);
	return (*stream->streamInterface->getBytesLeft)(stream->implementation);
}

// Functions applicable to buffers only
size_t SharedBuffersCore::getReadPos(uint32_t id) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->getReadPos)(buffer->implementation);
}

size_t SharedBuffersCore::getWritePos(uint32_t id) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->getWritePos)(buffer->implementation);
}

size_t SharedBuffersCore::getLength(uint32_t id) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->getLength)(buffer->implementation);
}

void SharedBuffersCore::setReadPos(uint32_t id, size_t pos) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->setReadPos)(buffer->implementation, pos);
}

void SharedBuffersCore::setWritePos(uint32_t id, size_t pos) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->setWritePos)(buffer->implementation, pos);
}

uint8_t SharedBuffersCore::setLength(uint32_t id, size_t length) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->setLength)(buffer->implementation, length);
}

uint8_t* SharedBuffersCore::getData(uint32_t id) {
	shb_Buffer* buffer = findBuffer(coreApi, id);
	return (*buffer->bufferInterface->getData)(buffer->implementation);
}

uint8_t SharedBuffersCore::streamExists(uint32_t id) {
	return (*coreApi->findStream)(id) != 0;
}

uint8_t SharedBuffersCore::bufferExists(uint32_t id) {
	return (*coreApi->findBuffer)(id) != 0;
}

void SharedBuffersCore::destroy(uint32_t id) {
	(*coreApi->destroy)(id);
}

}
