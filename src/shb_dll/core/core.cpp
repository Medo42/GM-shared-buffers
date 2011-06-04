#include "core.hpp"

#include "HandleMap.hpp"
#include "HandlePool.hpp"


namespace {
using namespace shb_internal;

struct DestructableStream {
	shb_Stream stream;
	void* destroyHandlerImpl;
	uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId);
};

struct DestructableBuffer {
	shb_Buffer buffer;
	void* destroyHandlerImpl;
	uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId);
};

HandlePool handlePool;
HandleMap<DestructableStream> streamHandles(handlePool);
HandleMap<DestructableBuffer> bufferHandles(handlePool);

__stdcall shb_Stream* findStream(uint32_t id) {
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

__stdcall shb_Buffer* findBuffer(uint32_t id) {
	DestructableBuffer* buffer = bufferHandles.find(id);
	if(buffer) {
		return &(buffer->buffer);
	}
	return 0;
}

__stdcall uint32_t shareStream(
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

__stdcall uint32_t shareBuffer(
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

__stdcall void destroy(uint32_t id) {
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

shb_CoreApi coreApi = {
	&shareStream,
	&findStream,

	&shareBuffer,
	&findBuffer,

	&destroy
};

}

extern "C" __stdcall shb_CoreApi* shb_getCoreApiV1() {
	return &coreApi;
}
