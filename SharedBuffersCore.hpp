#pragma once

#include <stdint.h>
#include <stddef.h>
#include "public_core_api.hpp"

namespace shb {

class SharedBuffersCore {
	const shb_CoreApi* const coreApi;

public:
	SharedBuffersCore(shb_CoreApi* coreApi) : coreApi(coreApi) {}
	const shb_CoreApi* getCoreApi() { return coreApi; }

	// Functions applicable to streams and buffers
	size_t read(uint32_t id, uint8_t* data, size_t size);
	void write(uint32_t id, const uint8_t* data, size_t size);
	size_t getBytesLeft(uint32_t id);

	// Functions applicable to buffers only
	size_t getReadPos(uint32_t id);
	size_t getWritePos(uint32_t id);
	size_t getLength(uint32_t id);
	void setReadPos(uint32_t id, size_t pos);
	void setWritePos(uint32_t id, size_t pos);
	uint8_t setLength(uint32_t id, size_t length);
	uint8_t* getData(uint32_t id);

	uint8_t streamExists(uint32_t id);
	uint8_t bufferExists(uint32_t id);

	void destroy(uint32_t id);
};

}
