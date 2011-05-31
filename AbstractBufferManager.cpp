#include "AbstractBufferManager.hpp"

#include "AbstractBuffer.hpp"
#include "core/public_core_api.hpp"

namespace shb_internal {
using namespace shb;

static __stdcall void read(void* impl, uint8_t* data, size_t size) {
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

static __stdcall uint8_t getFragment(void* impl, shb_BufferFragment* fragment, size_t pos) {
	AbstractBuffer* buffer = static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl));
	BufferFragment resultFragment = buffer->getFragment(pos);
	fragment->start = resultFragment.getStart();
	fragment->end = resultFragment.getEnd();
	return resultFragment.isValid();
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
	&getFragment
};

}

namespace shb {
using namespace shb_internal;

AbstractBufferManager::AbstractBufferManager(const shb_CoreApi* coreApi) : coreApi(coreApi) {}
AbstractBufferManager::~AbstractBufferManager() {}

uint32_t AbstractBufferManager::shareBuffer(AbstractBuffer* buffer) {
	AbstractStream* stream = static_cast<AbstractStream*>(buffer);
	return (*coreApi->shareBuffer)(stream, &abstractStreamInterface, &abstractBufferInterface, this, &destroyHandler);
}

uint32_t AbstractBufferManager::shareStream(AbstractStream* stream) {
	return (*coreApi->shareStream)(stream, &abstractStreamInterface, this, &destroyHandler);
}

__stdcall uint8_t AbstractBufferManager::destroyHandler(void* impl, uint32_t bufferId) {
	return static_cast<AbstractBufferManager*>(impl)->destroyCallback(bufferId);
}

}
