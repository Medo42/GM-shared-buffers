#include "core.hpp"

#include "HandleMap.hpp"
#include "HandlePool.hpp"
#include "SharedBuffers.hpp"

using namespace shb_internal;

static HandlePool handlePool;
static HandleMap<SharedStream> streamHandles(handlePool);
static HandleMap<SharedBuffer> bufferHandles(handlePool);

static SharedBuffer findBuffer(uint32_t handle) {
	SharedBuffer *buffer = bufferHandles.find(handle);
	if(buffer) {
		return *buffer;
	} else {
		return SharedBuffer();
	}
}

static SharedStream findStreamOrBuffer(uint32_t handle) {
	SharedStream* stream = streamHandles.find(handle);
	if(stream) {
		return *stream;
	} else {
		return findBuffer(handle);
	}
}

extern "C" {
	// Functions applicable for both buffers and streams
	__stdcall size_t shb_readData(uint32_t id, uint8_t* data, size_t size) {
		return findStreamOrBuffer(id).read(data, size);
	}

	__stdcall void shb_writeData(uint32_t id, const uint8_t* data, size_t size) {
		return findStreamOrBuffer(id).write(data, size);
	}

	__stdcall size_t shb_getBytesLeft(uint32_t id) {
		return findStreamOrBuffer(id).getBytesLeft();
	}

	__stdcall uint8_t shb_streamOrBufferExists(uint32_t id) {
		return (streamHandles.find(id) != 0) || (bufferHandles.find(id) != 0);
	}

	// Functions only applicable for buffers
	__stdcall size_t shb_getReadPos(uint32_t id) {
		return findBuffer(id).getReadPos();
	}

	__stdcall size_t shb_getWritePos(uint32_t id) {
		return findBuffer(id).getWritePos();
	}

	__stdcall size_t shb_getLength(uint32_t id) {
		return findBuffer(id).getLength();
	}

	__stdcall void shb_setReadPos(uint32_t id, size_t pos) {
		return findBuffer(id).setReadPos(pos);
	}

	__stdcall void shb_setWritePos(uint32_t id, size_t pos) {
		return findBuffer(id).setWritePos(pos);
	}

	__stdcall uint8_t shb_setLength(uint32_t id, size_t length) {
		return findBuffer(id).setLength(length);
	}

	__stdcall uint8_t* getData(uint32_t id) {
		return findBuffer(id).getData();
	}

	__stdcall uint8_t shb_bufferExists(uint32_t id) {
		return (bufferHandles.find(id) != 0);
	}

	__stdcall uint32_t shb_shareStream(
			void* stream,
			shb_StreamInterface* streamInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) {
		SharedStream sharedStream(stream, streamInterface, destroyHandlerImpl, destroyCallback);
		return streamHandles.allocate(sharedStream);
	}

	__stdcall uint32_t shb_shareBuffer(
			void* buffer,
			shb_StreamInterface *streamInterface,
			shb_BufferInterface *bufferInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) {
		SharedBuffer sharedBuffer(buffer, streamInterface, bufferInterface, destroyHandlerImpl, destroyCallback);
		return bufferHandles.allocate(sharedBuffer);
	}

	__stdcall void shb_destroy(uint32_t id) {
		SharedStream stream = findStreamOrBuffer(id);
		if(findStreamOrBuffer(id).destroy(id)) {
			// We don't know here whether this is a stream or buffer handle,
			// so we just release both - there can be no overlap so it's fine.
			streamHandles.release(id);
			bufferHandles.release(id);
		}
	}
}
