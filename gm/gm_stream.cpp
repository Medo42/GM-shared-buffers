#include "gm.h"
#include "../BufferProxy.hpp"

extern shb_CoreApi* shbApi;
using namespace shb;

static inline uint32_t toHandle(double id) {
	return gm_double_to_X<uint32_t>(id);
}

static inline StreamProxy getStream(double id) {
	return StreamProxy(shbApi, toHandle(id));
}

static inline BufferProxy getBuffer(double id) {
	return BufferProxy(shbApi, toHandle(id));
}

/**
 * If id is a buffer handle, and the buffer is not large enough to write
 * bytes bytes without resizing, this function tries to resize the buffer
 * in advance so that there will only be one reallocation.
 *
 * This only really makes sense for potentially large write operations.
 */
static inline void preallocateBufferSpace(double id, size_t bytes) {
	BufferProxy buffer = getBuffer(id);
	size_t writePos = buffer.getWritePos();
	if(buffer.bufferExists() && bytes > buffer.getLength()-writePos) {
		// Beware the size_t overflow :P
		if(bytes <= std::numeric_limits<size_t>::max() - writePos) {
			buffer.setLength(writePos + bytes);
		} else {
			throw std::bad_alloc();
		}
	}
}

template<typename X>
static inline X stream_read_x(double id) {
	X value;
	getStream(id).read(reinterpret_cast<uint8_t*>(&value), sizeof(value));
	return value;
}

template<typename X>
static inline double stream_write_x(double id, double value) {
	X convertedValue = gm_double_to_X<X>(value);
	getStream(id).write(reinterpret_cast<uint8_t*>(&convertedValue), sizeof(convertedValue));
	return 0;
}

gmexport double stream_exists(double id) {
	return getStream(id).streamExists();
}

gmexport double stream_read_int8(double id) {
	return stream_read_x<int8_t>(id);
}

gmexport double stream_read_uint8(double id) {
	return stream_read_x<uint8_t>(id);
}

gmexport double stream_read_int16(double id) {
	return stream_read_x<int16_t>(id);
}

gmexport double stream_read_uint16(double id) {
	return stream_read_x<uint16_t>(id);
}

gmexport double stream_read_int32(double id) {
	return stream_read_x<int32_t>(id);
}

gmexport double stream_read_uint32(double id) {
	return stream_read_x<uint32_t>(id);
}

gmexport double stream_read_float32(double id) {
	return stream_read_x<float>(id);
}

gmexport double stream_read_float64(double id) {
	return stream_read_x<double>(id);
}

gmexport double stream_write_int8(double id, double value) {
	return stream_write_x<int8_t>(id, value);
}

gmexport double stream_write_uint8(double id, double value) {
	return stream_write_x<uint8_t>(id, value);
}

gmexport double stream_write_int16(double id, double value) {
	return stream_write_x<int16_t>(id, value);
}

gmexport double stream_write_uint16(double id, double value) {
	return stream_write_x<uint16_t>(id, value);
}

gmexport double stream_write_int32(double id, double value) {
	return stream_write_x<int32_t>(id, value);
}

gmexport double stream_write_uint32(double id, double value) {
	return stream_write_x<uint32_t>(id, value);
}

gmexport double stream_write_float32(double id, double value) {
	return stream_write_x<float>(id, value);
}

gmexport double stream_write_float64(double id, double value) {
	return stream_write_x<double>(id, value);
}

gmexport const char* stream_read_string(double id) {
	gm_returnstring.clear();
	// TODO
	return 0;
}

gmexport double stream_write_string(double id, const char* string) {
	getStream(id).write(reinterpret_cast<const uint8_t*>(string), strlen(string) + 1);
	return 1;
}

gmexport const char* stream_read_data(double id, double bytes) {
	size_t size = gm_double_to_X<size_t>(bytes);
	StreamProxy stream = getStream(id);
	if(stream.getBytesLeft() < size) {
		return "";
	} else {
		char* returnstring = gm_make_return_string(size);
		stream.read(reinterpret_cast<uint8_t*>(returnstring), size);
		return returnstring;
	}
}

gmexport double stream_write_data(double id, const char* string) {
	getStream(id).write(reinterpret_cast<const uint8_t*>(string), strlen(string));
	return 1;
}

gmexport const char* stream_read_hex(double id, double bytes) {
	size_t size = gm_double_to_X<size_t>(bytes);
	StreamProxy stream = getStream(id);
	if(size > size*2 || stream.getBytesLeft() < size) {
		return "";
	}

	/*
	 * We read the data to the end of the allocated string buffer, and then
	 * chew through it from the beginning, overwriting parts we have already
	 * processed. The last loop cycle will overwrite the source data it is
	 * working from, but that's no problem because it is copied to a first.
	 */
	char *str = gm_make_return_string(size * 2);
	stream.read(reinterpret_cast<uint8_t*>(str+size), size);
	for(size_t i = 0; i < size; ++i) {
		uint8_t a = *reinterpret_cast<uint8_t*>(i+size);
		str[2 * i]     = ((a >> 4) > 9)? 'a' + (a >> 4) - 10 : '0' + (a >> 4);
		str[2 * i + 1] = ((a & 15) > 9)? 'a' + (a & 15) - 10 : '0' + (a & 15);
	}
	return str;
}

gmexport double stream_write_hex(double id, const char* string) {
	size_t size = strlen(string) / 2;
	preallocateBufferSpace(id, size);
	StreamProxy stream = getStream(id);

	for(size_t i = 0; i < size; ++i) {
		uint8_t a = string[2 * i];
		uint8_t b = string[2 * i + 1];
		uint8_t c =
			((('0' <= a && a <= '9')? a - '0' : (('a' <= a && a <= 'f')? 10 + a - 'a' : (('A' <= a && a <= 'F')? 10 + a - 'A' : 0))) << 4) |
			(('0' <= b && b <= '9')? b - '0' : (('a' <= b && b <= 'f')? 10 + b - 'a' : (('A' <= b && b <= 'F')? 10 + b - 'A' : 0)));
		stream.write(&c, 1);
	}
	return 1;
}

gmexport double stream_write_stream_part(double id, double source_id, double bytes) {
	StreamProxy sourceStream = getStream(source_id);
	StreamProxy destStream = getStream(id);
	BufferProxy sourceBuffer = getBuffer(source_id);
	BufferProxy destBuffer = getBuffer(id);

	size_t size = gm_double_to_X<size_t>(bytes);

	if(size > sourceStream.getBytesLeft() || !sourceStream.streamExists() || !destStream.streamExists()) {
		return 0;
	}

	preallocateBufferSpace(id, size);
	if(sourceBuffer.bufferExists()) {
		size_t readPos = sourceBuffer.getReadPos();
		BufferFragment fragment;
		while(size > 0 && (fragment = sourceBuffer.getFragment(readPos), fragment.isValid())) {
			size_t copySize = std::min(size, fragment.getLength());
			destStream.write(fragment.getStart(), copySize);
			readPos += copySize;
			size -= copySize;
		}
		sourceBuffer.setReadPos(readPos);
		return 1;
	} else if(destBuffer.bufferExists()) {
		size_t writePos = destBuffer.getWritePos();
		// Normally we would try to allocate space here, but this was already done above
		// by preallocateBufferSpace.
		BufferFragment fragment;
		while(size > 0 && (fragment = destBuffer.getFragment(writePos), fragment.isValid())) {
			size_t copySize = std::min(size, fragment.getLength());
			sourceStream.read(fragment.getStart(), copySize);
			writePos += copySize;
			size -= copySize;
		}
		destBuffer.setWritePos(writePos);
		return 1;
	} else {
		// Copy in small blocks as a tradeoff between speed and memory use.
		uint8_t tempBuffer[256];
		while(size > 0) {
			size_t copySize = std::min(size, (size_t)256);
			sourceStream.read(tempBuffer, copySize);
			destStream.write(tempBuffer, copySize);
			size -= copySize;
		}
		return 1;
	}
}

gmexport double stream_write_stream(double id, double source_id) {
	return stream_write_stream_part(id, source_id, getStream(source_id).getBytesLeft());
}

gmexport double stream_get_bytes_left(double id) {
	return getStream(id).getBytesLeft();
}

