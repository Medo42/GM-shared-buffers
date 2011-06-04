// Included from AbstractBufferManager.hpp

#include "../AbstractBuffer.hpp"
#include "../core/public_core_api.hpp"

namespace shb_internal {
using namespace shb;

inline __stdcall size_t read(void* impl, uint8_t* data, size_t size) {
	return static_cast<AbstractStream*>(impl)->read(data, size);
}

inline __stdcall size_t peek(void* impl, uint8_t* data, size_t size) {
	return static_cast<AbstractStream*>(impl)->peek(data, size);
}

inline __stdcall size_t skip(void* impl, size_t size) {
	return static_cast<AbstractStream*>(impl)->skip(size);
}

inline __stdcall size_t write(void* impl, const uint8_t* data, size_t size) {
	return static_cast<AbstractStream*>(impl)->write(data, size);
}

inline __stdcall size_t getBytesLeft(void* impl) {
	return static_cast<AbstractStream*>(impl)->getBytesLeft();
}

inline __stdcall size_t getReadPos(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getReadPos();
}

inline __stdcall size_t getWritePos(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getWritePos();
}

inline __stdcall size_t getLength(void* impl) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->getLength();
}

inline __stdcall void setReadPos(void* impl, size_t pos) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setReadPos(pos);
}

inline __stdcall void setWritePos(void* impl, size_t pos) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setWritePos(pos);
}

inline __stdcall uint8_t setLength(void* impl, size_t length) {
	return static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl))->setLength(length);
}

inline __stdcall void getFragment(void* impl, shb_BufferFragment* fragment, size_t pos) {
	AbstractBuffer* buffer = static_cast<AbstractBuffer*>(static_cast<AbstractStream*>(impl));
	BufferFragment resultFragment = buffer->getFragment(pos);
	fragment->start = resultFragment.getStart();
	fragment->end = resultFragment.getEnd();
}

inline shb_StreamInterface* getAbstractStreamInterface() {
	static shb_StreamInterface abstractStreamInterface = {
		&read,
		&peek,
		&skip,
		&write,
		&getBytesLeft
	};
	return &abstractStreamInterface;
}

inline shb_BufferInterface* getAbstractBufferInterface() {
	static shb_BufferInterface abstractBufferInterface = {
		&getReadPos,
		&getWritePos,
		&getLength,
		&setReadPos,
		&setWritePos,
		&setLength,
		&getFragment
	};
	return &abstractBufferInterface;
}

}

namespace shb {
using namespace shb_internal;

inline AbstractBufferManager::AbstractBufferManager(const shb_CoreApi* coreApi) : coreApi(coreApi) {}
inline AbstractBufferManager::~AbstractBufferManager() {}

inline uint32_t AbstractBufferManager::shareBuffer(AbstractBuffer* buffer) {
	AbstractStream* stream = static_cast<AbstractStream*>(buffer);
	return (*coreApi->shareBuffer)(stream, getAbstractStreamInterface(), getAbstractBufferInterface(), this, &destroyHandler);
}

inline uint32_t AbstractBufferManager::shareStream(AbstractStream* stream) {
	return (*coreApi->shareStream)(stream, getAbstractStreamInterface(), this, &destroyHandler);
}

inline __stdcall uint8_t AbstractBufferManager::destroyHandler(void* impl, uint32_t bufferId) {
	return static_cast<AbstractBufferManager*>(impl)->destroyCallback(bufferId);
}

}
