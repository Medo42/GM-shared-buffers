#include "NullBuffer.hpp"

#include <stdint.h>

namespace shb_internal {

static __stdcall size_t read(void*, uint8_t*, size_t) { return 0; }
static __stdcall void write(void*, const uint8_t*, size_t) { }
static __stdcall void writeOther(void* impl, uint32_t sourceId, size_t size) { }
static __stdcall size_t getBytesLeft(void*) { return 0; }
static __stdcall uint8_t destroy(void*) { return true; }

static __stdcall size_t getReadPos(void*) { return 0; }
static __stdcall size_t getWritePos(void*) { return 0; }
static __stdcall size_t getLength(void*) { return 0; }
static __stdcall void setReadPos(void*, size_t) { }
static __stdcall void setWritePos(void*, size_t) { }
static __stdcall void setLength(void*, size_t) { }

shb_StreamInterface nullStreamInterface = {
	&read,
	&write,
	&writeOther,
	&getBytesLeft,
	&destroy
};

shb_BufferInterface nullBufferInterface = {
	&getReadPos,
	&getWritePos,
	&getLength,
	&setReadPos,
	&setWritePos,
	&setLength
};

}
