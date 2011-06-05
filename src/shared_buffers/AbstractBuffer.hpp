#pragma once

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

namespace shb {

class AbstractStream {
public:
	/**
	 * Read size bytes into the provided data buffer. Where the data comes from is
	 * completely up to the implementation. If less data is available, as much as is
	 * available will be copied to the beginning of the buffer.
	 *
	 * The actual number of bytes read is returned.
	 */
	virtual size_t read(uint8_t* data, size_t size) = 0;

	/**
	 * peek behaves like read, except that the data is not removed from the stream.
	 * That is, subsequent calls to read must return the same data that is returned
	 * by the peek operation.
	 */
	virtual size_t peek(uint8_t* data, size_t size) = 0;

	/**
	 * skip behaves like read, except that the data is removed from the stream without
	 * being copied. The actual number of bytes skipped is returned.
	 */
	virtual size_t skip(size_t size) = 0;

	/**
	 * Write size bytes from the provided data buffer. Where the data is written to
	 * is completely up to the implementation. If less than size bytes can be written,
	 * as much data as possible is written instead. Implementations should clearly
	 * document if and when this can happen.
	 *
	 * The actual number of bytes written is returned.
	 */
	virtual size_t write(const uint8_t* data, size_t size) = 0;

	/**
	 * Return the number of bytes that can be read at the moment.
	 */
	virtual size_t getBytesLeft() = 0;

	virtual ~AbstractStream() {}
};

/**
 * In order to allow buffer users to rely on some common behaviour, implementations should follow the
 * specification below. It's not formally complete, but I think it gets the major points across. In short,
 * implementations should behave like you would expect a memory buffer to behave.
 *
 * A buffer is a collection of bytes indexed by a contiguous range of numbers starting from 0.
 * length is the number of bytes in the buffer, and is thus equal to the highest index plus one.
 * readPos and writePos are numbers in the range [0, length].
 *
 * read(data, size) will copy size bytes starting from the one indexed by readPos into the data
 * array, if size <= length-readPos. Otherwise as many bytes as possible are copied. readPos is incremented
 * by the number of bytes actually read, and that number is returned.
 *
 * write(data, size) will copy size bytes from the data array into the buffer, starting at the index
 * writePos, if size <= length-writePos or the buffer can be resized to the required length. Otherwise
 * as many bytes as possible are copied. writePos is incremented by the number of bytes actually written,
 * and that number is returned.
 *
 * getReadPos, getWritePos, setReadPos and setWritePos return or modify the current value of readPos
 * and writePos. Attempting to set either to a value greater than length shall set it to length instead.
 *
 * getLength returns the current length. setLength(newLength) resizes the buffer so that
 * lenght=newLength, if this is possible. If newLength < length, all data with index of newLength and
 * greater is discarded. If newLength > length, the new memory available must be initialized to 0.
 * All data in the index range [0, min(length, newLength)-1] must be kept intact. If the resize is
 * successful, true is returned. Otherwise, false is returned and the contents of the buffer are not modified.
 * In that case, the new length of the buffer may be anything between the old size and the requested size.
 */
class AbstractBuffer : public AbstractStream {
public:
	virtual size_t read(uint8_t* data, size_t size);
	virtual size_t write(const uint8_t* data, size_t size);
	virtual size_t getReadPos() = 0;
	virtual size_t getWritePos() = 0;
	virtual void setReadPos(size_t pos) = 0;
	virtual void setWritePos(size_t pos) = 0;

	virtual size_t getLength() = 0;
	virtual bool setLength(size_t length) = 0;

	/**
	 * Default implementation using read(), getReadPos() and setReadPos().
	 */
	virtual size_t peek(uint8_t* data, size_t size);

	/**
	 * Default implementation using getReadPos() and setReadPos().
	 */
	virtual size_t skip(size_t size);

	/**
	 * Default implementation using getLength() and getReadPos().
	 */
	virtual size_t getBytesLeft();

	/**
	 * Convenience function.
	 */
	void clear() {
		setLength(0);
	}

	/**
	 * Convenience function.
	 */
	size_t read(uint8_t* data, size_t startPos, size_t size) {
		size_t oldReadPos = getReadPos();
		setReadPos(startPos);
		size_t ammountRead = read(data, size);
		setReadPos(oldReadPos);
		return ammountRead;
	}

	virtual ~AbstractBuffer() {}
};

/**
 * Read size bytes from the source stream or buffer and write them to the destination
 * stream or buffer.
 *
 * If fewer bytes are available to be read from source or if not everything can be written,
 * as much as possible is written. It will always be attempted to read size bytes from the
 * source stream, or the ammount returned by source.getBytesLeft() (whichever is smaller),
 * even if fewer bytes can be written.
 *
 * The actual number of bytes written is returned.
 */
size_t copyStream(AbstractStream& dest, AbstractStream& source, size_t size);

/**
 * Read size bytes starting on position sourceStartPos in the source buffer and
 * write them to the dest stream or buffer.
 *
 * The read position of the source buffer will be preserved. If fewer bytes are available
 * to be read or if not everything can be written, as much as possible is written.
 *
 * The actual number of bytes written is returned.
 */
size_t copyBuffer(AbstractStream& dest, AbstractBuffer& source, size_t sourceStartPos, size_t size);

}

#include "impl/AbstractBuffer.ipp"
