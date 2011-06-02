#pragma once

#include "../BufferProxy.hpp"

#include <math.h>
#include <limits>
#include <vector>
#include <new>

#define gmexport extern "C" __declspec (dllexport)

extern shb_CoreApi* shbCoreApi;

extern std::vector<char> gm_returnstring;
static inline char* gm_make_return_string(size_t length) {
	if(length == std::numeric_limits<size_t>::max()) throw std::bad_alloc();
	gm_returnstring.resize(length+1);
	gm_returnstring[length] = '\0';
	return &(gm_returnstring.front());
}


/**
 * Safely convert the given value to the target type, clipping to the
 * range boundaries of the target type.
 */
template<typename TargetType, typename SourceType>
static inline TargetType clipped_cast(SourceType value) {
	if(value <= std::numeric_limits<TargetType>::min()) {
		return std::numeric_limits<TargetType>::min();
	} else if(value >= std::numeric_limits<TargetType>::max()) {
		return std::numeric_limits<TargetType>::max();
	} else {
		return static_cast<TargetType>(value);
	}
}

/**
 * Convert a double from Game Maker to the target type.
 *
 * Values outside of the target type's range will be clamped
 * to the border values of the type.
 * If the target type is an integer, the result will be rounded
 * (nearest, away from zero).
 */
template<typename TargetType>
static inline TargetType gm_double_to_X(double value) {
	if(std::numeric_limits<TargetType>::is_integer) {
		return clipped_cast<TargetType>(round(value));
	} else {
		return clipped_cast<TargetType>(value);
	}
}

template<>
inline bool gm_double_to_X(double value) {
	return (value>=0.5);
}

static inline uint32_t toHandle(double id) {
	return gm_double_to_X<uint32_t>(id);
}

static inline shb::StreamProxy getStream(double id) {
	return shb::StreamProxy(shbCoreApi, toHandle(id));
}

static inline shb::BufferProxy getBuffer(double id) {
	return shb::BufferProxy(shbCoreApi, toHandle(id));
}
