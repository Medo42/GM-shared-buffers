#pragma once

#include <stdint.h>

namespace shb {

class AbstractBuffer;
class AbstractStream;
class SharedBuffersCore;

/**
 * This is intended as the base class for all buffer-owning classes.
 *
 * You probably won't need to create your own implementation, unless
 * DefaultBufferManager does not suit your needs.
 */
class AbstractBufferManager {
private:
	static __stdcall uint8_t destroyHandler(void* impl, uint32_t bufferId);

protected:
	SharedBuffersCore* core;

	/**
	 * Register a buffer/stream with the shared buffers library. If someone tries
	 * to destroy the buffer/stream, the destroyCallback in this AbstractBufferManager
	 * instance will be called with the id of the buffer.
	 *
	 * A unique handle for shared access to the buffer/stream will be generated and returned.
	 */
	uint32_t shareBuffer(AbstractBuffer* buffer);
	uint32_t shareStream(AbstractStream* stream);

	/**
	 * This method is called whenever someone tries to destroy a stream or buffer
	 * which was shared through this manager.
	 *
	 * If you return true, the buffer's handle will be invalidated so that the buffer
	 * will no longer be shared. Otherwise, the handle is kept available.
	 *
	 * You can perform appropriate cleanup actions in this method. If you choose
	 * to destroy the buffer object, you should definitely return true to prevent
	 * the shared buffers library from attempting to access the destroyed object.
	 */
	virtual uint8_t destroyCallback(uint32_t bufferId) = 0;

public:
	AbstractBufferManager(SharedBuffersCore* core);
	virtual ~AbstractBufferManager();
};

}
