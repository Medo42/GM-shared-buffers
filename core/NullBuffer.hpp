#pragma once

#include "core.hpp"

namespace shb_internal {

extern shb_StreamInterface nullStreamInterface;
extern shb_BufferInterface nullBufferInterface;
__stdcall uint8_t nullDestroyHandler(void*, uint32_t);

}

