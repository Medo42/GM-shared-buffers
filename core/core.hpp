#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
	size_t (__stdcall *read)(void* impl, uint8_t* data, size_t size);
	void (__stdcall *write)(void* impl, const uint8_t* data, size_t size);
	void (__stdcall *writeOther)(void* impl, uint32_t sourceId, size_t size);
	size_t (__stdcall *getBytesLeft)(void* impl);
	uint8_t (__stdcall *destroy)(void* impl);
} shb_StreamInterface;

typedef struct {
	size_t (__stdcall *getReadPos)(void* impl);
	size_t (__stdcall *getWritePos)(void* impl);
	size_t (__stdcall *getLength)(void* impl);
	void (__stdcall *setReadPos)(void* impl, size_t pos);
	void (__stdcall *setWritePos)(void* impl, size_t pos);
	void (__stdcall *setLength)(void* impl, size_t length);
} shb_BufferInterface;

extern "C" {
	// Functions applicable for both buffers and streams
	__stdcall size_t shb_readData(uint32_t id, uint8_t* data, size_t size);
	__stdcall void shb_writeData(uint32_t id, const uint8_t* data, size_t size);
	__stdcall void shb_writeOther(uint32_t id, uint32_t sourceId, size_t size);
	__stdcall size_t shb_getBytesLeft(uint32_t id);
	__stdcall void shb_destroy(uint32_t id);
	__stdcall uint8_t shb_streamOrBufferExists(uint32_t id);

	// Functions only applicable for buffers
	__stdcall size_t shb_getReadPos(uint32_t id);
	__stdcall size_t shb_getWritePos(uint32_t id);
	__stdcall size_t shb_getLength(uint32_t id);
	__stdcall void shb_setReadPos(uint32_t id, size_t pos);
	__stdcall void shb_setWritePos(uint32_t id, size_t pos);
	__stdcall void shb_setLength(uint32_t id, size_t length);
	__stdcall uint8_t shb_bufferExists(uint32_t id);

	__stdcall uint32_t shb_shareStream(void* stream, shb_StreamInterface* streamInterface);
	__stdcall uint32_t shb_shareBuffer(void* buffer, shb_StreamInterface* streamInterface, shb_BufferInterface* bufferInterface);
}
