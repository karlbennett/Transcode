/*
 * StandardUtils.hpp
 *
 *  Created on: 3 Mar 2012
 *      Author: karl
 */

#ifndef __STANDARD_UTILS_HPP__
#define __STANDARD_UTILS_HPP__

#include <map>

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
		, const K& key);

#endif /* __STANDARD_UTILS_HPP__ */
