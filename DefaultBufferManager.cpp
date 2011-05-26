#include "DefaultBufferManager.hpp"
#include "core/core.hpp"

using namespace shb;

DefaultBufferManager::DefaultBufferManager() :
		destructableBuffers(), indestructableBuffers() {}

uint8_t DefaultBufferManager::destroy(uint32_t bufferId) {
	if(destructableBuffers.count(bufferId) != 0) {
		delete destructableBuffers[bufferId];
		destructableBuffers.erase(bufferId);
		return true;
	} else {
		return false;
	}
}

uint32_t DefaultBufferManager::shareTransferOwnership(AbstractStream* stream, bool destructable) {
	AbstractBuffer* buffer = dynamic_cast<AbstractBuffer*>(stream);
	uint32_t bufferId;
	if(buffer) {
		bufferId = shareBuffer(buffer, this);
	} else {
		bufferId = shareStream(stream, this);
	}

	if(destructable) {
		destructableBuffers[bufferId] = buffer;
	} else {
		indestructableBuffers[bufferId] = buffer;
	}
	return bufferId;
}

void DefaultBufferManager::forceDestroy(uint32_t bufferId) {
	if(indestructableBuffers.count(bufferId) != 0) {
		destructableBuffers[bufferId] = indestructableBuffers[bufferId];
		indestructableBuffers.erase(bufferId);
	} else if(destructableBuffers.count(bufferId) == 0) {
		// The buffer is not managed here, do nothing
		return;
	}
	shb_destroy(bufferId);
}
