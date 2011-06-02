#include "gm.h"
#include "../DefaultBufferManager.hpp"
#include "../DefaultMemBuffer.hpp"

using namespace shb;
DefaultBufferManager bufferManager(shbCoreApi);

gmexport double memorybuffer_create() {
	return bufferManager.shareTransferOwnership(new DefaultMemBuffer(), true);
}
