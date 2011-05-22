#pragma once

#include <stdint.h>

typedef struct {
	uint32_t (__stdcall *read)(void*, uint8_t*, uint32_t);
	void (__stdcall *write)(void*, const uint8_t*, uint32_t);
	uint32_t (__stdcall *getBytesLeft)(void*);
	uint8_t (__stdcall *destroy)(void*);
} SharedStreamInterface;

typedef struct {
	uint32_t (__stdcall *getreadpos)(void*);
	uint32_t (__stdcall *getwritepos)(void*);
	uint32_t (__stdcall *getlength)(void*);
	void (__stdcall *setreadpos)(void*, uint32_t);
	void (__stdcall *setwritepos)(void*, uint32_t);
	void (__stdcall *setlength)(void*, uint32_t);
} SharedBufferInterface;

extern "C" {
	// Functions applicable for both buffers and streams
	__stdcall uint32_t readData(uint32_t id, uint8_t* data, uint32_t size);
	__stdcall void writeData(uint32_t id, const uint8_t* data, uint32_t size);
	__stdcall uint32_t getBytesLeft(uint32_t id);
	__stdcall void destroyStreamOrBuffer(uint32_t id);
	__stdcall uint8_t streamOrBufferExists(uint32_t id);

	// Functions only applicable for buffers
	__stdcall uint32_t getReadPos(uint32_t id);
	__stdcall uint32_t getWritePos(uint32_t id);
	__stdcall uint32_t getLength(uint32_t id);
	__stdcall void setReadPos(uint32_t id, uint32_t pos);
	__stdcall void setWritePos(uint32_t id, uint32_t pos);
	__stdcall void setLength(uint32_t id, uint32_t length);
	__stdcall uint8_t bufferExists(uint32_t id);

	__stdcall uint32_t addStream(void* stream, SharedStreamInterface* streamInterface);
	__stdcall uint32_t addBuffer(void* buffer, SharedStreamInterface* streamInterface, SharedBufferInterface* bufferInterface);
}
