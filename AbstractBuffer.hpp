#pragma once

#include <stddef.h>
#include <stdint.h>

namespace shb {

class BufferFragment {
	uint8_t * const start;
	uint8_t * const end;

public:
	BufferFragment() : start(0), end(0) {}
	BufferFragment(uint8_t *start, uint8_t *end) : start(start), end(end) {}
	bool isValid() const { return start != 0; }
	uint8_t* getStart() const { return start; }
	uint8_t* getEnd() const { return end; }
	size_t getLength() const {
		return (end > start) ? end-start : 0;
	}
};

class AbstractStream {
public:
	/**
	 * Read size bytes into the provided data buffer. Where the data comes from is
	 * completely up to the implementation. If less data is available, the behavior
	 * is undefined.
	 */
	virtual void read(uint8_t* data, size_t size) = 0;

	/**
	 * Write size bytes from the provided data buffer. Where the data is written to
	 * is completely up to the implementation.
	 */
	virtual void write(const uint8_t* data, size_t size) = 0;

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
 * array and increase readPos by size, if size <= length-readPos. Otherwise the behaviour is undefined.
 *
 * write(data, size) will copy size bytes from the data array into the buffer, starting at the index
 * writePos, and increase writePos by size, if size <= length-writePos or the buffer can be resized
 * to the required length. Otherwise the behaviour is undefined.
 *
 * getReadPos, getWritePos, setReadPos and setWritePos return or modify the current value of readPos
 * and writePos. Attempting to set either to a value greater than length shall set it to length instead.
 *
 * getLength returns the current length. setLength(newLength) resizes the buffer so that
 * lenght=newLength, if this is possible. If newLength < length, all data with index of newLength and
 * greater is discarded. If newLength > length, the new memory available must be initialized to 0.
 * All data in the index range [0, min(length, newLength)-1] must be kept intact. If the resize is
 * successful, true is returned. Otherwise, false is returned and the contents of the buffer are not modified.
 *
 * getFragment(pos) returns two pointers (start and end) in a struct. If pos<length, start shall point
 * to the buffer byte at index pos, and end shall point directly behind the last byte of the same memory block.
 * More formally, for start<=ptr<end, ptr must point to the buffer byte at index pos+ptr-start.
 * If pos>=length, both start and end shall be null pointers. The pointers returned must remain
 * valid until a setter function or write() is called on the buffer.
 *
 * No operation performed on the buffer may cause unsafe behaviour like access to memory locations
 * outside the buffer, even if bad parameters are supplied.
 */
class AbstractBuffer : public AbstractStream {
public:
	virtual size_t getReadPos() = 0;
	virtual size_t getWritePos() = 0;
	virtual void setReadPos(size_t pos) = 0;
	virtual void setWritePos(size_t pos) = 0;

	virtual size_t getLength() = 0;
	virtual bool setLength(size_t length) = 0;

	virtual BufferFragment getFragment(size_t pos) = 0;

	/**
	 * Default implementation. This is the expected behaviour for buffers,
	 * and you should not usually need to override it.
	 */
	virtual size_t getBytesLeft() {
		return getLength() - getReadPos();
	}

	virtual ~AbstractBuffer() {}
};

}
