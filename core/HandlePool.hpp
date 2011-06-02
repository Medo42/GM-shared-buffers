#pragma once

#include <stdint.h>
#include <set>

namespace shb_internal {

/**
 * Generates unique integer values for use as handles.
 *
 * The value 0 is never generated as a handle. This way, people can assign
 * 0 or negative values to GML variables which can hold a handle but currently
 * do not, and this can be checked with "if(handle)". If 0 was a valid handle value,
 * this pattern would obviously fail.
 */
class HandlePool {
private:
	// Prevent accidental copying, which would not make sense for this class
	HandlePool(const HandlePool&);
	HandlePool& operator=(const HandlePool&);

	std::set<uint32_t> usedHandles;
	uint32_t nextHandle;

public:
	HandlePool() : usedHandles(), nextHandle(1) {}
	~HandlePool() {}

	uint32_t allocate() {
		while(nextHandle==0 || usedHandles.count(nextHandle) > 0) {
			nextHandle++;
		}
		usedHandles.insert(nextHandle);
		return nextHandle++;
	}

	void release(uint32_t handle) {
		usedHandles.erase(handle);
	}
};

}
