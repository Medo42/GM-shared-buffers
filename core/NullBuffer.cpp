#include "NullBuffer.hpp"

#include <stdint.h>
#include <new>

namespace shb_internal {

static __stdcall size_t read(void*, uint8_t*, size_t) { return 0; }
static __stdcall void write(void*, const uint8_t*, size_t) { }
static __stdcall size_t getBytesLeft(void*) { return 0; }

static __stdcall size_t getReadPos(void*) { return 0; }
static __stdcall size_t getWritePos(void*) { return 0; }
static __stdcall size_t getLength(void*) { return 0; }
static __stdcall void setReadPos(void*, size_t) { }
static __stdcall void setWritePos(void*, size_t) { }
static __stdcall uint8_t setLength(void*, size_t) { return false; }
static __stdcall uint8_t* getData(void*) { return 0; }

__stdcall uint8_t nullDestroyHandler(void*, uint32_t) { return false; }

shb_StreamInterface nullStreamInterface = {
	&read,
	&write,
	&getBytesLeft,
};

shb_BufferInterface nullBufferInterface = {
	&getReadPos,
	&getWritePos,
	&getLength,
	&setReadPos,
	&setWritePos,
	&setLength,
	&getData
};

}
