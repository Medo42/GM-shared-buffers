#pragma once

#include "public_api.h"

typedef struct {
	void *instance;
	SharedStreamInterface* streamInterface;
} SharedStream;

typedef struct {
	void *instance;
	SharedStreamInterface* streamInterface;
	SharedBufferInterface* bufferInterface;
} SharedBuffer;
