// Included from DefaultMemBuffer.hpp

#include <algorithm>
#include <limits>
#include <cstring>

namespace shb {

inline DefaultMemBuffer::DefaultMemBuffer() : data(0), readPos(0), writePos(0), capacity(0), length(0) {
	setLength(0);
}

/**
 * Copy size bytes from the current read position of the buffer
 * into the provided array. If less data is available,
 * as much data as available is copied, and the rest of the
 * provided array is not changed. The number of bytes
 * actually read is returned.
 */
inline size_t DefaultMemBuffer::read(uint8_t* out, size_t size) {
	size = std::min(size, getBytesLeft());
	memmove(out, data+readPos, size);
	readPos += size;
	return size;
}

/**
 * Write array to the buffer, starting at writePos.
 * The buffer is enlarged if required.
 */
inline size_t DefaultMemBuffer::write(const uint8_t *in, size_t size) {
	if(size > length-writePos && size <= std::numeric_limits<size_t>::max() - writePos) {
		setLength(writePos+size);
	}
	size = std::min(size, length-writePos);
	memmove(data+writePos, in, size);
	writePos += size;
	return size;
}

inline size_t DefaultMemBuffer::getReadPos() {
	return readPos;
}

inline size_t DefaultMemBuffer::getWritePos() {
	return writePos;
}

inline size_t DefaultMemBuffer::getLength() {
	return length;
}

inline void DefaultMemBuffer::setReadPos(size_t pos) {
	readPos = std::min(pos, length);
}

inline void DefaultMemBuffer::setWritePos(size_t pos) {
	writePos = std::min(pos, length);
}

inline bool DefaultMemBuffer::setLength(size_t newLength) {
	size_t newCapacity = capacity;
	if(newLength > capacity) {
		// grow by at least 50% to ensure constant amortized insert time
		// but as little as possible beyond that to conserve memory.
		newCapacity = std::max(capacity + capacity / 2, newLength);
	} else if(newLength <= capacity / 2) {
		// keep 25% more than required
		newCapacity = newLength + newLength / 4;
	}
	newCapacity = std::max(newCapacity, MIN_CAPACITY);

	if(newCapacity != capacity) {
		void *temp = realloc(data, newCapacity);
		if(temp==NULL) {
			// Out of memory, Capt'n
			return false;
		}
		data = static_cast<uint8_t*>(temp);
	}

	if(newLength > length) {
		memset(data + length, 0, newLength - length);
	}

	length = newLength;
	capacity = newCapacity;
	readPos = std::min(readPos, length);
	writePos = std::min(writePos, length);
	return true;
}

}