#include "AbstractBufferManager.hpp"

#include "SharedBuffersCore.hpp"
#include "AbstractBuffer.hpp"
#include "public_core_api.hpp"

namespace shb_internal {
using namespace shb;

static __stdcall size_t read(void* impl, uint8_t* data, size_t size) {
	return static_cast<AbstractStream*>(impl)->read(data, size);
}

static __stdcall void write(void* impl, const uint8_t* data, size_t size) {
	return static_cast<AbstractStream*>(impl)->write(data, size);
}

static __stdcall size_t getBytesLeft(void* impl) {
	return static_cast<AbstractStream*>(impl)->getBytesLeft();
}

static __stdcall size_t getReadPos(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getReadPos();
}

static __stdcall size_t getWritePos(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getWritePos();
}

static __stdcall size_t getLength(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getLength();
}

static __stdcall void setReadPos(void* impl, size_t pos) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setReadPos(pos);
}

static __stdcall void setWritePos(void* impl, size_t pos) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setWritePos(pos);
}

static __stdcall uint8_t setLength(void* impl, size_t length) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setLength(length);
}

static __stdcall uint8_t* getData(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getData();
}

static shb_StreamInterface abstractStreamInterface = {
	&read,
	&write,
	&getBytesLeft
};

static shb_BufferInterface abstractBufferInterface = {
	&getReadPos,
	&getWritePos,
	&getLength,
	&setReadPos,
	&setWritePos,
	&setLength,
	&getData
};

}

namespace shb {
using namespace shb_internal;

__stdcall uint8_t AbstractBufferManager::destroyHandler(void* impl, uint32_t bufferId) {
	return static_cast<AbstractBufferManager*>(impl)->destroyCallback(bufferId);
}

uint32_t AbstractBufferManager::shareBuffer(AbstractBuffer* buffer) {
	AbstractStream* stream = static_cast<AbstractStream*>(buffer);
	return (*core->getCoreApi()->shareBuffer)(stream, &abstractStreamInterface, &abstractBufferInterface, this, &destroyHandler);
}

uint32_t AbstractBufferManager::shareStream(AbstractStream* stream) {
	return (*core->getCoreApi()->shareStream)(stream, &abstractStreamInterface, this, &destroyHandler);
}

AbstractBufferManager::AbstractBufferManager(SharedBuffersCore* core) : core(core) {}
AbstractBufferManager::~AbstractBufferManager() {}

}
