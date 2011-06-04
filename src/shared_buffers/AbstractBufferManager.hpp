#pragma once

#include <stdint.h>

struct shb_CoreApi;

namespace shb {

class AbstractBuffer;
class AbstractStream;

/**
 * This is intended as the base class for all buffer-owning classes.
 *
 * You probably won't need to create your own implementation, unless
 * DefaultBufferManager does not suit your needs.
 *
 * Important: Do not destroy a buffer manager as long as there are still buffers shared
 * through it! Otherwise the library will try to call the destroy callback on a deleted
 * object - NOT a good thing!
 */
class AbstractBufferManager {
public:
	explicit AbstractBufferManager(const shb_CoreApi* coreApi);
	virtual ~AbstractBufferManager();

protected:
	const shb_CoreApi* const coreApi;

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

private:
	static __stdcall uint8_t destroyHandler(void* impl, uint32_t bufferId);
};

}

#include "impl/AbstractBufferManager.ipp"
