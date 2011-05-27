#include "core.hpp"

#include "HandleMap.hpp"
#include "HandlePool.hpp"

using namespace shb_internal;

typedef struct {
	shb_Stream stream;
	void* destroyHandlerImpl;
	uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId);
} DestructableStream;

typedef struct {
	shb_Buffer buffer;
	void* destroyHandlerImpl;
	uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId);
} DestructableBuffer;

static HandlePool handlePool;
static HandleMap<DestructableStream> streamHandles(handlePool);
static HandleMap<DestructableBuffer> bufferHandles(handlePool);

static __stdcall shb_Stream* findStream(uint32_t id) {
	DestructableStream* stream = streamHandles.find(id);
	if(stream) {
		return &(stream->stream);
	}

	DestructableBuffer* buffer = bufferHandles.find(id);
	if(buffer) {
		return (shb_Stream*)&(buffer->buffer);
	}

	return 0;
}

static __stdcall shb_Buffer* findBuffer(uint32_t id) {
	DestructableBuffer* buffer = bufferHandles.find(id);
	if(buffer) {
		return &(buffer->buffer);
	}
	return 0;
}

static __stdcall uint32_t shareStream(
		void* stream,
		shb_StreamInterface* streamInterface,
		void* destroyHandlerImpl,
		uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) {

	DestructableStream destructableStream = {
		{
			stream,
			streamInterface
		},
		destroyHandlerImpl,
		destroyCallback
	};

	return streamHandles.allocate(destructableStream);
}

static __stdcall uint32_t shareBuffer(
		void* buffer,
		shb_StreamInterface *streamInterface,
		shb_BufferInterface *bufferInterface,
		void* destroyHandlerImpl,
		uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId)) {

	DestructableBuffer destructableBuffer = {
		{
			buffer,
			streamInterface,
			bufferInterface
		},
		destroyHandlerImpl,
		destroyCallback
	};

	return bufferHandles.allocate(destructableBuffer);
}

static __stdcall void destroy(uint32_t id) {
	DestructableStream* stream = streamHandles.find(id);
	DestructableBuffer* buffer = bufferHandles.find(id);

	if(stream) {
		if((*stream->destroyCallback)(stream->destroyHandlerImpl, id)) {
			streamHandles.release(id);
		}
	} else if(buffer) {
		if((*buffer->destroyCallback)(buffer->destroyHandlerImpl, id)) {
			bufferHandles.release(id);
		}
	}
}

static shb_CoreApi coreApi = {
	&findStream,
	&findBuffer,

	&shareStream,
	&shareBuffer,

	&destroy
};

extern "C" __stdcall shb_CoreApi* shb_getCoreApi() {
	return &coreApi;
}
