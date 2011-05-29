#pragma once

#include <stdint.h>
#include <stddef.h>

struct shb_StreamInterface {
	void (__stdcall *read)(void* impl, uint8_t* data, size_t size);
	void (__stdcall *write)(void* impl, const uint8_t* data, size_t size);
	size_t (__stdcall *getBytesLeft)(void* impl);
};

struct shb_BufferInterface {
	size_t (__stdcall *getReadPos)(void* impl);
	size_t (__stdcall *getWritePos)(void* impl);
	size_t (__stdcall *getLength)(void* impl);
	void (__stdcall *setReadPos)(void* impl, size_t pos);
	void (__stdcall *setWritePos)(void* impl, size_t pos);
	uint8_t (__stdcall *setLength)(void* impl, size_t length);
};

struct shb_Stream {
	void* implementation;
	struct shb_StreamInterface* streamInterface;
};

struct shb_Buffer {
	void* implementation;
	struct shb_StreamInterface* streamInterface;
	struct shb_BufferInterface* bufferInterface;
};

struct shb_CoreApi {
	struct shb_Stream* (__stdcall *findStream)(uint32_t id);
	struct shb_Buffer* (__stdcall *findBuffer)(uint32_t id);

	uint32_t (__stdcall *shareStream)(
			void* stream,
			struct shb_StreamInterface* streamInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId));

	uint32_t (__stdcall *shareBuffer)(
			void* buffer,
			struct shb_StreamInterface* streamInterface,
			struct shb_BufferInterface* bufferInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId));

	void (__stdcall *destroy)(uint32_t id);
};
