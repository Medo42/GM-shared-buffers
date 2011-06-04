// Included from AbstractBuffer.hpp.

#include <limits>
#include <cstring>

namespace shb {

inline size_t AbstractBuffer::read(uint8_t* data, size_t size) {
	size_t dataPos = 0;
	size_t readPos = getReadPos();
	BufferFragment fragment;
	while(dataPos < size && (fragment = getFragment(readPos), fragment.isValid())) {
		size_t copySize = std::min(size-dataPos, fragment.getLength());
		memmove(data+dataPos, fragment.getStart(), copySize);
		dataPos += copySize;
		readPos += copySize;
	}
	setReadPos(readPos);
	return dataPos;
}

inline size_t AbstractBuffer::peek(uint8_t* data, size_t size) {
	size_t oldReadPos = getReadPos();
	size_t copied = read(data, size);
	setReadPos(oldReadPos);
	return copied;
}

inline size_t AbstractBuffer::skip(size_t size) {
	size_t oldReadPos = getReadPos();
	setReadPos(oldReadPos+size);
	return getReadPos()-oldReadPos;
}

inline size_t AbstractBuffer::write(const uint8_t* data, size_t size) {
	size_t dataPos = 0;
	size_t writePos = getWritePos();
	if(size > getLength()-writePos && size <= std::numeric_limits<size_t>::max() - writePos) {
		// Attempt to resize, but if it doesn't work just copy what we can.
		setLength(writePos+size);
	}

	BufferFragment fragment;
	while(dataPos < size && (fragment = getFragment(writePos), fragment.isValid())) {
		size_t copySize = std::min(size-dataPos, fragment.getLength());
		memmove(fragment.getStart(), data, copySize);
		dataPos += copySize;
		writePos += copySize;
	}
	setWritePos(writePos);
	return dataPos;
}

inline size_t AbstractBuffer::getBytesLeft() {
	return getLength() - getReadPos();
}

inline size_t copyStreamToBuffer(AbstractBuffer& dest, AbstractStream& source, size_t size) {
	size = std::min(size, source.getBytesLeft());
	size_t writePos = dest.getWritePos();
	size_t copied = 0;

	if(size > dest.getLength()-writePos && size <= std::numeric_limits<size_t>::max() - writePos) {
		dest.setLength(writePos+size);
	}

	BufferFragment fragment;
	while(copied < size && (fragment = dest.getFragment(writePos), fragment.isValid())) {
		size_t copySize = std::min(size - copied, fragment.getLength());
		source.read(fragment.getStart(), copySize);
		copied += copySize;
		writePos += copySize;
	}

	if(copied < size) {
		source.skip(size - copied);
	}

	dest.setWritePos(writePos);
	return copied;
}

inline size_t copyStream(AbstractStream& dest, AbstractStream& source, size_t size) {
	AbstractBuffer* sourceAsBuffer = dynamic_cast<AbstractBuffer*>(&source);
	AbstractBuffer* destAsBuffer = dynamic_cast<AbstractBuffer*>(&dest);
	if(sourceAsBuffer) {
		size_t written = copyBuffer(dest, *sourceAsBuffer, sourceAsBuffer->getReadPos(), size);
		sourceAsBuffer->skip(size); // copyBuffer doesn't advance the source read position
		return written;
	} else if(destAsBuffer) {
		return copyStreamToBuffer(*destAsBuffer, source, size);
	} else {
		// Actual Stream->Stream copy
		size = std::min(size, source.getBytesLeft());
		uint8_t tempBuffer[256];
		size_t copied = 0;
		while(copied < size) {
			size_t copySize = std::min(size - copied, (size_t)256);
			source.read(tempBuffer, copySize);
			size_t actuallyWritten = dest.write(tempBuffer, copySize);
			if(actuallyWritten != copySize) {
				source.skip(size - (copied + copySize));
				return copied + actuallyWritten;
			}
			copied += actuallyWritten;
		}

		return copied;
	}
}

inline size_t copyBuffer(AbstractStream& dest, AbstractBuffer& source, size_t sourceStartPos, size_t size) {
	size_t copied = 0;
	BufferFragment fragment;
	while(copied < size && (fragment = source.getFragment(sourceStartPos+copied), fragment.isValid())) {
		size_t copySize = std::min(size - copied, fragment.getLength());
		size_t actuallyWritten = dest.write(fragment.getStart(), copySize);
		copied += actuallyWritten;
		if(actuallyWritten != copySize) {
			break;
		}
	}
	return copied;
}

}
