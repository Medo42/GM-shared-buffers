#include "public_api.h"

#include "HandleMap.hpp"
#include "HandlePool.hpp"
#include "SharedBuffers.hpp"
#include "NullBuffer.hpp"

HandlePool handlePool;
HandleMap<SharedStream> streamHandles(handlePool);
HandleMap<SharedBuffer> bufferHandles(handlePool);

static SharedBuffer findBuffer(uint32_t handle) {
	SharedBuffer *buffer = bufferHandles.find(handle);
	if(buffer) {
		return *buffer;
	} else {
		return SharedBuffer();
	}
}

/**
 * Find the stream or buffer for the given handle. If a buffer is found, it is automatically
 * converted to a stream. If nothing is found, a stream backed by the null buffer implementation
 * is returned.
 */
static SharedStream findStream(uint32_t handle) {
	SharedStream* stream = streamHandles.find(handle);
	if(stream) {
		return *stream;
	} else {
		return findBuffer(handle);
	}
}

extern "C" {
	// Functions applicable for both buffers and streams
	__stdcall uint32_t readData(uint32_t id, uint8_t* data, uint32_t size) {
		return findStream(id).read(data, size);
	}

	__stdcall void writeData(uint32_t id, const uint8_t* data, uint32_t size) {
		findStream(id).write(data, size);
	}

	__stdcall uint32_t getBytesLeft(uint32_t id) {
		return findStream(id).getBytesLeft();
	}

	__stdcall void destroyStreamOrBuffer(uint32_t id) {
		SharedStream stream = findStream(id);
		if(findStream(id).destroy()) {
			// We don't know here whether this is a stream or buffer handle,
			// so we just release both - there can be no overlap so it's fine.
			streamHandles.release(id);
			bufferHandles.release(id);
		}
	}

	__stdcall uint8_t streamOrBufferExists(uint32_t id) {
		return (streamHandles.find(id) != 0) || (bufferHandles.find(id) != 0);
	}

	// Functions only applicable for buffers
	__stdcall uint32_t getReadPos(uint32_t id) {
		return findBuffer(id).getreadpos();
	}

	__stdcall uint32_t getWritePos(uint32_t id) {
		return findBuffer(id).getwritepos();
	}

	__stdcall uint32_t getLength(uint32_t id) {
		return findBuffer(id).getlength();
	}

	__stdcall void setReadPos(uint32_t id, uint32_t pos) {
		findBuffer(id).setreadpos(pos);
	}

	__stdcall void setWritePos(uint32_t id, uint32_t pos) {
		findBuffer(id).setwritepos(pos);
	}

	__stdcall void setLength(uint32_t id, uint32_t length) {
		findBuffer(id).setlength(length);
	}

	__stdcall uint8_t bufferExists(uint32_t id) {
		return (bufferHandles.find(id) != 0);
	}

	__stdcall uint32_t addStream(void* stream, SharedStreamInterface* streamInterface) {
		SharedStream sharedStream(stream, streamInterface);
		return streamHandles.allocate(sharedStream);
	}

	__stdcall uint32_t addBuffer(void* buffer, SharedStreamInterface *streamInterface, SharedBufferInterface *bufferInterface) {
		SharedBuffer sharedBuffer(buffer, streamInterface, bufferInterface);
		return bufferHandles.allocate(sharedBuffer);
	}
}
