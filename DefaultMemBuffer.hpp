#pragma once

#include "AbstractBuffer.hpp"
#include "core/core.hpp"

#include <algorithm>
#include <vector>
#include <cstring>

namespace shb {

class DefaultMemBuffer : public AbstractBuffer {
	std::vector<uint8_t> data;
	size_t readPos;
	size_t writePos;

public:
	DefaultMemBuffer() : data(), readPos(0), writePos(0) {}

	size_t getBytesLeft() {
		return data.size()-readPos;
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
		memmove(out, data.data()+readPos, size);
		readPos += size;
		return size;
	}

	/**
	 * Write array to the buffer, starting at writePos.
	 * The buffer is enlarged if required.
	 */
	void write(const uint8_t *in, size_t size) {
		size_t newWritePos = writePos+size;
		if(newWritePos > data.size()) {
			data.resize(newWritePos);
		}
		memmove(data.data()+writePos, in, size);
		writePos = newWritePos;
	}

	void writeOther(uint32_t sourceId, size_t size) {
		size_t newWritePos = writePos+size;
		if(newWritePos > data.size()) {
			data.resize(newWritePos);
		}
		shb_readData(sourceId, data.data()+writePos, size);
		writePos = newWritePos;
	}

	uint8_t destroy() {
		// TODO
		return true;
	}

	size_t getReadPos() {
		return readPos;
	}

	size_t getWritePos() {
		return writePos;
	}

	size_t getLength() {
		return data.size();
	}

	void setReadPos(size_t pos) {
		readPos = std::min(pos, data.size());
	}

	void setWritePos(size_t pos) {
		writePos = std::min(pos, data.size());
	}

	void setLength(size_t length) {
		data.resize(length);
		readPos = std::min(readPos, data.size());
		writePos = std::min(writePos, data.size());
	}
};

}
