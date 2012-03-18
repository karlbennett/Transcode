/*
 * StandardUtils.hpp
 *
 *  Created on: 3 Mar 2012
 *      Author: karl
 */

#ifndef __UTIL_STANDARD_HPP__
#define __UTIL_STANDARD_HPP__

#include <map>

/**
 * @file util_standard.hpp
 *
 * An API that provided utility functions to help simplify some usages of the STL.
 */

/**
 * Transcode namespace, all the top level transcode functions and classes are in
 * this namespace. So this namespace holds officially public API.
 */
namespace transcode {

/**
 * Util namespace, all the utility functions and classes are found within this
 * namespace. You might find something useful here but hopefully everything in the
 * {@see transcode} namespace should provide what you need so you shouldn't have to
 * look in here.
 */
namespace util {

/**
 * Convenience template to make it easier to get the value out of a constant map.
 *
 * @param map - the map to get the value out of.
 * @param key - the key of the value in the map that we wish to get.
 *
 * @return the value from the const map that is related to the provided key if it
 * 			exists, otherwise return an empty instance of the value type.
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

	return V();
}

} /* namespace utils */
} /* namespace transcode */

#endif /* __UTIL_STANDARD_HPP__ */
