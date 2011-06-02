#pragma once

#include "AbstractBuffer.hpp"

#include <stdint.h>
#include <stddef.h>

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

	/**
	 * Read size bytes from the given stream and write them to this one.
	 * If fewer bytes are available to be read or if not everything can be written,
	 * as much as possible is written. It will always be attempted to read size bytes
	 * from the source stream, even if fewer bytes can be written.
	 *
	 * The actual number of bytes written is returned.
	 */
	size_t writeStream(AbstractStream& stream, size_t size);

	/**
	 * Read size bytes starting on position startPos in the given buffer and
	 * write them to this one. The read position of the source buffer is not modified.
	 * If fewer bytes are available to be read or if not everything can be written,
	 * as much as possible is written.
	 *
	 * The actual number of bytes written is returned.
	 */
	size_t writeBuffer(AbstractBuffer& buffer, size_t startPos, size_t size);
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

	/**
	 * Read size bytes from the given stream and write them to this buffer.
	 * If fewer bytes are available to be read or if not everything can be written,
	 * as much as possible is written. It will always be attempted to read size bytes
	 * from the source stream, even if fewer bytes can be written.
	 *
	 * The actual number of bytes written is returned.
	 */
	size_t writeStream(AbstractStream& stream, size_t size);

	/**
	 * Read size bytes starting on position startPos in the given buffer and
	 * write them to this one. The read position of the source buffer is not modified.
	 * If fewer bytes are available to be read or if not everything can be written,
	 * as much as possible is written.
	 *
	 * The actual number of bytes written is returned.
	 */
	size_t writeBuffer(AbstractBuffer& buffer, size_t startPos, size_t size);
private:
	const shb_CoreApi* const coreApi;
	uint32_t handle;
};

}
