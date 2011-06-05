// Included from AbstractBuffer.hpp.

#include <limits>
#include <cstring>

namespace shb {

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

inline size_t AbstractBuffer::getBytesLeft() {
	return getLength() - getReadPos();
}

inline size_t copyStream(AbstractStream& dest, AbstractStream& source, size_t size) {
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

inline size_t copyBuffer(AbstractStream& dest, AbstractBuffer& source, size_t sourceStartPos, size_t size) {
	size_t oldReadPos = source.getReadPos();
	source.setReadPos(sourceStartPos);
	size_t copied = copyStream(dest, source, size);
	source.setReadPos(oldReadPos);
	return copied;
}

}
