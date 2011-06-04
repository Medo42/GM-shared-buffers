#include "gm.h"
#include <shared_buffers/DefaultBufferManager.hpp>
#include <shared_buffers/DefaultMemBuffer.hpp>

using namespace shb;
DefaultBufferManager bufferManager(shbCoreApi);

gmexport double memorybuffer_create() {
	return bufferManager.shareTransferOwnership(new DefaultMemBuffer(), true);
}
