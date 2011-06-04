#pragma once

#include "AbstractBuffer.hpp"

struct shb_CoreApi;

namespace shb {

class StreamProxy : public AbstractStream {
public:
	StreamProxy(const shb_CoreApi* coreApi, uint32_t handle);
	bool streamExists();
	void destroy();

	virtual size_t read(uint8_t* data, size_t size);
	virtual size_t peek(uint8_t* data, size_t size);
	virtual size_t skip(size_t size);
	virtual size_t write(const uint8_t* data, size_t size);
	virtual size_t getBytesLeft();
private:
	const shb_CoreApi* const coreApi;
	uint32_t handle;
};

class BufferProxy : public AbstractBuffer {
public:
	BufferProxy(const shb_CoreApi* coreApi, uint32_t handle);
	bool bufferExists();
	void destroy();

	virtual size_t read(uint8_t* data, size_t size);
	virtual size_t peek(uint8_t* data, size_t size);
	virtual size_t skip(size_t size);
	virtual size_t write(const uint8_t* data, size_t size);
	virtual size_t getBytesLeft();

	virtual size_t getReadPos();
	virtual size_t getWritePos();
	virtual size_t getLength();
	virtual void setReadPos(size_t pos);
	virtual void setWritePos(size_t pos);
	virtual bool setLength(size_t length);
	virtual BufferFragment getFragment(size_t pos);

private:
	const shb_CoreApi* const coreApi;
	uint32_t handle;
};

}

#include "impl/BufferProxy.ipp"
