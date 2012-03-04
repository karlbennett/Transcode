/*
 * StandardUtils.hpp
 *
 *  Created on: 3 Mar 2012
 *      Author: karl
 */

#ifndef __STANDARD_UTILS_HPP__
#define __STANDARD_UTILS_HPP__

#include <map>

namespace transcode {
namespace utils {

/**
 * Convenience template to make it easier to get the value out of a constant map.
 *
 * @param map - the map to get the value out of.
 * @param key - the key of the value in the map that we wish to get.
 *
 * @return the value from the const map that is related to the provided key if it
 * 			exists, otherwise return NULL.
 */
template<typename K, typename V> V get(const std::map<K, V>& map
		, const K& key) {
	// The definition of this template is in the header file because templates must
	// be declared and defined in the same file.

	// Use the map find function so that we can get the value from a const map.
	// Code from here:
	// http://stackoverflow.com/questions/687789/c-const-stdmap-reference-fails-to-compile
	typename std::map<K, V>::const_iterator it;
	it = map.find(key);
	if (it != map.end()) {

		return it->second;
	}

	return NULL;
}

} /* namespace utils */
} /* namespace transcode */

#endif /* __STANDARD_UTILS_HPP__ */
