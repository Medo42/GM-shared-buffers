//Included from DefaultBufferManager.hpp

#include "../AbstractBuffer.hpp"
#include "../core/public_core_api.hpp"

namespace shb {

inline DefaultBufferManager::DefaultBufferManager(const shb_CoreApi* coreApi) :
		AbstractBufferManager(coreApi), destructableBuffers(), indestructableBuffers() {}

inline uint8_t DefaultBufferManager::destroyCallback(uint32_t bufferId) {
	if(destructableBuffers.count(bufferId) != 0) {
		delete destructableBuffers[bufferId];
		destructableBuffers.erase(bufferId);
		return true;
	} else {
		return false;
	}
}

inline uint32_t DefaultBufferManager::shareTransferOwnership(AbstractStream* stream, bool destructable) {
	AbstractBuffer* buffer = dynamic_cast<AbstractBuffer*>(stream);
	uint32_t bufferId;
	if(buffer) {
		bufferId = shareBuffer(buffer);
	} else {
		bufferId = shareStream(stream);
	}

	if(destructable) {
		destructableBuffers[bufferId] = buffer;
	} else {
		indestructableBuffers[bufferId] = buffer;
	}
	return bufferId;
}

inline void DefaultBufferManager::forceDestroy(uint32_t bufferId) {
	if(indestructableBuffers.count(bufferId) != 0) {
		destructableBuffers[bufferId] = indestructableBuffers[bufferId];
		indestructableBuffers.erase(bufferId);
	} else if(destructableBuffers.count(bufferId) == 0) {
		// The buffer is not managed here, do nothing
		return;
	}

	(*coreApi->destroy)(bufferId);
}

}