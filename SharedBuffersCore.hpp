#pragma once

#include <stdint.h>
#include <stddef.h>

struct shb_CoreApi;

namespace shb {

class SharedBuffersCore {
public:
	SharedBuffersCore(shb_CoreApi* coreApi);
	const shb_CoreApi* getCoreApi();

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
	bool setLength(uint32_t id, size_t length);
	uint8_t* getData(uint32_t id);

	bool streamExists(uint32_t id);
	bool bufferExists(uint32_t id);

	void destroy(uint32_t id);

private:
	const shb_CoreApi* const coreApi;
};

}
