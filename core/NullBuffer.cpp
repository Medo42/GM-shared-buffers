#include "NullBuffer.hpp"

#include <stdint.h>

namespace nullbuffer {
	__stdcall uint32_t read(void*, uint8_t*, uint32_t) { return 0; }
	__stdcall void write(void*, const uint8_t*, uint32_t) {}
	__stdcall uint32_t getBytesLeft(void*) { return 0; }
	__stdcall uint8_t destroy(void*) { return true; }

	__stdcall uint32_t getReadPos(void*) { return 0; }
	__stdcall uint32_t getWritePos(void*) { return 0; }
	__stdcall uint32_t getLength(void*) { return 0; }
	__stdcall void setReadPos(void*, uint32_t) { }
	__stdcall void setWritePos(void*, uint32_t) { }
	__stdcall void setLength(void*, uint32_t) { }

	SharedStreamInterface streamInterface = {
		&read,
		&write,
		&getBytesLeft,
		&destroy
	};

	SharedBufferInterface bufferInterface = {
		&getReadPos,
		&getWritePos,
		&getLength,
		&setReadPos,
		&setWritePos,
		&setLength
	};
}
