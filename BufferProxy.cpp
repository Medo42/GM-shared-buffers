#include "BufferProxy.hpp"
#include "core/public_core_api.hpp"

namespace shb {

StreamProxy::StreamProxy(const shb_CoreApi* coreApi, uint32_t handle) :
		coreApi(coreApi), handle(handle) {}

bool StreamProxy::streamExists() {
	return (*coreApi->findStream)(handle) != 0;
}

void StreamProxy::destroy() {
	if(streamExists()) {
		(*coreApi->destroy)(handle);
	}
}

size_t StreamProxy::read(uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->read)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

size_t StreamProxy::peek(uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->peek)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

size_t StreamProxy::skip(size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->skip)(stream->implementation, size);
	} else {
		return 0;
	}
}

size_t StreamProxy::write(const uint8_t* data, size_t size) {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->write)(stream->implementation, data, size);
	} else {
		return 0;
	}
}

size_t StreamProxy::getBytesLeft() {
	shb_Stream* stream = (*coreApi->findStream)(handle);
	if(stream) {
		return (*stream->streamInterface->getBytesLeft)(stream->implementation);
	} else {
		return 0;
	}
}

size_t StreamProxy::writeStream(AbstractStream& source, size_t size) {
	AbstractBuffer* sourceAsBuffer = dynamic_cast<AbstractBuffer*>(&source);
	if(sourceAsBuffer) {
		// If the source is actually a buffer, use the buffer copying routine for greater efficiency.
		size_t written = writeBuffer(*sourceAsBuffer, sourceAsBuffer->getReadPos(), size);
		sourceAsBuffer->skip(size);
		return written;
	} else if((*coreApi->findBuffer)(handle)) {
		// If I am actually a buffer, use the buffer's stream copying routine for greater efficiency.
		return BufferProxy(coreApi, handle).writeStream(source, size);
	} else {
		// Actual Stream->Stream copy
		size = std::min(size, source.getBytesLeft());
		uint8_t tempBuffer[256];
		size_t copied = 0;
		while(copied < size) {
			size_t copySize = std::min(size-copied, (size_t)256);
			source.read(tempBuffer, copySize);
			size_t actuallyWritten = write(tempBuffer, copySize);
			if(actuallyWritten != copySize) {
				source.skip(size-copied-copySize);
				return copied+actuallyWritten;
			}
			copied += actuallyWritten;
		}
		return copied;
	}
}

size_t StreamProxy::writeBuffer(AbstractBuffer& source, size_t startPos, size_t size) {
	size_t copied = 0;
	BufferFragment fragment;
	while(copied < size && (fragment = source.getFragment(startPos+copied), fragment.isValid())) {
		size_t copySize = std::min(size - copied, fragment.getLength());
		size_t actuallyWritten = write(fragment.getStart(), copySize);
		if(actuallyWritten != copySize) {
			return copied+actuallyWritten;
		}
		copied += actuallyWritten;
	}
	return copied;
}

BufferProxy::BufferProxy(const shb_CoreApi* coreApi, uint32_t handle):
		coreApi(coreApi), handle(handle) {}

bool BufferProxy::bufferExists() {
	return (*coreApi->findBuffer)(handle) != 0;
}

void BufferProxy::destroy() {
	if(bufferExists()) {
		(*coreApi->destroy)(handle);
	}
}

size_t BufferProxy::read(uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->read)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

size_t BufferProxy::peek(uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->peek)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

size_t BufferProxy::skip(size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->skip)(buffer->implementation, size);
	} else {
		return 0;
	}
}

size_t BufferProxy::write(const uint8_t* data, size_t size) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->write)(buffer->implementation, data, size);
	} else {
		return 0;
	}
}

size_t BufferProxy::getBytesLeft() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->streamInterface->getBytesLeft)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getReadPos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getReadPos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getWritePos() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getWritePos)(buffer->implementation);
	} else {
		return 0;
	}
}

size_t BufferProxy::getLength() {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->getLength)(buffer->implementation);
	} else {
		return 0;
	}
}

void BufferProxy::setReadPos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setReadPos)(buffer->implementation, pos);
	}
}

void BufferProxy::setWritePos(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		(*buffer->bufferInterface->setWritePos)(buffer->implementation, pos);
	}
}

bool BufferProxy::setLength(size_t length) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		return (*buffer->bufferInterface->setLength)(buffer->implementation, length);
	} else {
		return false;
	}
}

BufferFragment BufferProxy::getFragment(size_t pos) {
	shb_Buffer* buffer = (*coreApi->findBuffer)(handle);
	if(buffer) {
		shb_BufferFragment fragment;
		(*buffer->bufferInterface->getFragment)(buffer->implementation, &fragment, pos);
		return BufferFragment(fragment.start, fragment.end);
	} else {
		return BufferFragment();
	}
}

/**
 * Read size bytes from the given stream and write them to this buffer.
 * If fewer bytes are available to be read or if not everything can be written,
 * as much as possible is written. It will always be attempted to read size bytes
 * from the source stream, even if fewer bytes can be written.
 *
 * The actual number of bytes written is returned.
 */
size_t BufferProxy::writeStream(AbstractStream& source, size_t size) {
	size = std::min(size, source.getBytesLeft());
	size_t writePos = getWritePos();
	size_t copied = 0;

	if(size > getLength()-writePos && size <= std::numeric_limits<size_t>::max() - writePos) {
		setLength(writePos+size);
	}

	BufferFragment fragment;
	while(copied < size && (fragment = getFragment(writePos), fragment.isValid())) {
		size_t copySize = std::min(size - copied, fragment.getLength());
		source.read(fragment.getStart(), copySize);
		copied += copySize;
		writePos += copySize;
	}
	setWritePos(writePos);
	return copied;
}

/**
 * Read size bytes starting on position startPos in the given buffer and
 * write them to this one. The read position of the source buffer is not modified.
 * If fewer bytes are available to be read or if not everything can be written,
 * as much as possible is written.
 *
 * The actual number of bytes written is returned.
 */
size_t BufferProxy::writeBuffer(AbstractBuffer& source, size_t startPos, size_t size) {
	size_t oldReadPos = source.getReadPos();
	source.setReadPos(startPos);
	size_t written = writeStream(source, size);
	source.setReadPos(oldReadPos);
	return written;
}

}
