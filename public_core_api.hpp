#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
	size_t (__stdcall *read)(void* impl, uint8_t* data, size_t size);
	void (__stdcall *write)(void* impl, const uint8_t* data, size_t size);
	size_t (__stdcall *getBytesLeft)(void* impl);
} shb_StreamInterface;

typedef struct {
	size_t (__stdcall *getReadPos)(void* impl);
	size_t (__stdcall *getWritePos)(void* impl);
	size_t (__stdcall *getLength)(void* impl);
	void (__stdcall *setReadPos)(void* impl, size_t pos);
	void (__stdcall *setWritePos)(void* impl, size_t pos);
	uint8_t (__stdcall *setLength)(void* impl, size_t length);
	uint8_t* (__stdcall *getData)(void* impl);
} shb_BufferInterface;

typedef struct {
	void* implementation;
	shb_StreamInterface* streamInterface;
} shb_Stream;

typedef struct {
	void* implementation;
	shb_StreamInterface* streamInterface;
	shb_BufferInterface* bufferInterface;
} shb_Buffer;

typedef struct {
	shb_Stream* (__stdcall *findStream)(uint32_t id);
	shb_Buffer* (__stdcall *findBuffer)(uint32_t id);

	uint32_t (__stdcall *shareStream)(
			void* stream,
			shb_StreamInterface* streamInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId));

	uint32_t (__stdcall *shareBuffer)(
			void* buffer,
			shb_StreamInterface* streamInterface,
			shb_BufferInterface* bufferInterface,
			void* destroyHandlerImpl,
			uint8_t (__stdcall *destroyCallback)(void* impl, uint32_t bufferId));

	void (__stdcall *destroy)(uint32_t id);
} shb_CoreApi;
