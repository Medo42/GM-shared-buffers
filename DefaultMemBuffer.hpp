#pragma once

#include "AbstractBuffer.hpp"
#include <algorithm>
#include <vector>
#include <cstring>

namespace shb {

class DefaultMemBuffer : public AbstractBuffer {
	std::vector<uint8_t> data;
	size_t readIndex;
	size_t writeIndex;

public:
	DefaultMemBuffer() : data(), readIndex(0), writeIndex(0) {}

	/**
	 * Return the number of bytes still remaining to be read.
	 */
	size_t getBytesLeft() {
		return data.size()-readIndex;
	}

	/**
	 * Copy size bytes from the current read position of the buffer
	 * into the provided array. If less data is available,
	 * as much data as available is copied, and the rest of the
	 * provided array is not changed. The number of bytes
	 * actually read is returned.
	 */
	size_t read(uint8_t* out, size_t size) {
		size = std::min(size, getBytesLeft());
		memcpy(out, data.data()+readIndex, size);
		readIndex += size;
		return size;
	}

	/**
	 * Write array to the buffer, starting at writePos.
	 * The buffer is enlarged if required.
	 */
	void write(const uint8_t *in, size_t size) {
		size_t newWriteIndex = writeIndex+size;
		if(newWriteIndex > data.size()) {
			data.resize(newWriteIndex);
		}
		memcpy(data.data()+writeIndex, in, size);
		writeIndex = newWriteIndex;
	}

	uint8_t destroy() {
		// TODO
		return true;
	}

	size_t getReadPos() {
		return readIndex;
	}

	size_t getWritePos() {
		return writeIndex;
	}

	size_t getLength() {
		return data.size();
	}

	void setReadPos(size_t pos) {
		readIndex = std::min(pos, data.size());
	}

	void setWritePos(size_t pos) {
		writeIndex = std::min(pos, data.size());
	}

	void setLength(size_t length) {
		data.resize(length);
		readIndex = std::min(readIndex, data.size());
		writeIndex = std::min(writeIndex, data.size());
	}
};

}
