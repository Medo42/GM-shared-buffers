#include "DefaultMemBuffer.hpp"

using namespace shb;

DefaultMemBuffer::DefaultMemBuffer() : data(0), readPos(0), writePos(0), capacity(0), length(0) {
	setLength(0);
}

/**
 * Copy size bytes from the current read position of the buffer
 * into the provided array. If less data is available,
 * as much data as available is copied, and the rest of the
 * provided array is not changed. The number of bytes
 * actually read is returned.
 */
size_t DefaultMemBuffer::read(uint8_t* out, size_t size) {
	size = std::min(size, getBytesLeft());
	memmove(out, data+readPos, size);
	readPos += size;
	return size;
}

/**
 * Write array to the buffer, starting at writePos.
 * The buffer is enlarged if required.
 */
void DefaultMemBuffer::write(const uint8_t *in, size_t size) {
	size_t newWritePos = writePos+size;
	if(newWritePos > length) {
		setLength(newWritePos);
	}
	memmove(data+writePos, in, size);
	writePos = newWritePos;
}

size_t DefaultMemBuffer::getBytesLeft() {
	return length-readPos;
}

uint8_t DefaultMemBuffer::destroy() {
	// TODO
	return true;
}

size_t DefaultMemBuffer::getReadPos() {
	return readPos;
}

size_t DefaultMemBuffer::getWritePos() {
	return writePos;
}

size_t DefaultMemBuffer::getLength() {
	return length;
}

void DefaultMemBuffer::setReadPos(size_t pos) {
	readPos = std::min(pos, length);
}

void DefaultMemBuffer::setWritePos(size_t pos) {
	writePos = std::min(pos, length);
}

uint8_t DefaultMemBuffer::setLength(size_t newLength) {
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
			throw std::bad_alloc();
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

uint8_t* DefaultMemBuffer::getData() {
	return data;
}
