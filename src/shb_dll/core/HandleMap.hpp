#pragma once

#include "HandlePool.hpp"

#include <stdint.h>
#include <map>

namespace shb_internal {

/**
 * Stores objects and provides access to them through unique handles.
 *
 * All HandleManagers that use the same HandlePool will hold disjoint
 * sets of handles.
 *
 * Destroying the HandleMap will release all contained handles from the pool.
 */
template<typename Handled>
class HandleMap {
private:
	// Prevent accidental copying, which would not make sense for this class
	HandleMap(const HandleMap&);
	HandleMap& operator=(const HandleMap&);

	typedef std::map<uint32_t, Handled> map_type;
	typedef typename map_type::iterator map_iter;

	HandlePool *handlePool;
	map_type content;

public:
	explicit HandleMap(HandlePool& pool) : handlePool(&pool), content() {};
	~HandleMap() {
		map_iter iter = content.begin();
		for(;iter != content.end(); ++iter) {
			handlePool->release(iter->first);
		}
	}

	/**
	 * Store a copy of the provided element and associate it with
	 * a pool-unique handle, which is returned.
	 */
	uint32_t allocate(Handled& element) {
		uint32_t handle = handlePool->allocate();
		content[handle] = element;
		return handle;
	}

	/**
	 * Return a pointer to the element associated with the given handle,
	 * or a null pointer if this Map does not hold the handle.
	 *
	 * Note that you will also get NULL as result if the given handle has
	 * previously been associated with NULL.
	 */
	Handled* find(uint32_t handle) {
		bool handleFound = content.count(handle);
		if(handleFound) {
			return &(content[handle]);
		} else {
			return 0;
		}
	}

	/**
	 * Delete the stored element and release the handle, if
	 * it is held by this map.
	 */
	void release(uint32_t handle) {
		if(content.erase(handle)) {
			handlePool->release(handle);
		}
	}
};

}
