#pragma once

#include "AbstractBuffer.hpp"
#include <map>

namespace shb {

class DefaultBufferManager : public AbstractBufferManager {
	std::map<uint32_t, AbstractStream*> destructableBuffers;
	std::map<uint32_t, AbstractStream*> indestructableBuffers;

public:
	DefaultBufferManager();

	virtual uint8_t destroy(uint32_t bufferId);

	/**
	 * Register the buffer or stream in the shared buffers library and transfer its
	 * ownership to this manager. This means that the buffer object will be deleted
	 * by the manager when the buffer is destroyed. Depending on the runtime type
	 * of the provided object, it will be shared either as a buffer or a stream.
	 *
	 * If destructable is true, anyone can destroy the buffer through the shared
	 * buffer API. In that case, you should not keep a pointer to the buffer, because
	 * it can unexpectedly become invalid.
	 *
	 * If destructable is false, the buffer can only be destroyed using the destroy
	 * method of this manager instance.
	 */
	uint32_t shareTransferOwnership(AbstractStream* stream, bool destructable);

	/**
	 * Destroy the buffer/stream with the provided ID if it is managed by this
	 * manager instance, even if it was registered as indestructable. You should
	 * only call this method if you by some convention own the buffer, in order
	 * to prevent invalidating pointers to the buffer object that might still
	 * be held elsewhere.
	 */
	void forceDestroy(uint32_t bufferId);
};

}
