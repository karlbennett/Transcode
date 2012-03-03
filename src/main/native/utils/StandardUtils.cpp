/*
 * StandardUtils.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: karl
 */

#ifndef __STANDARD_UTILS_CPP__
#define __STANDARD_UTILS_CPP__

#include <map>

template<typename K, typename V> V get(const std::map<K, V>& map
		, const K& key) {

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

#endif /* __STANDARD_UTILS_CPP__ */
