/*
 * FileUtils.cpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#include <utils/FileUtils.hpp>
#include <utils/MediaUtils.hpp>

#include <string>
#include <boost/filesystem.hpp>

namespace transcode {
namespace utils {

boost::filesystem::path checkFile(const std::string& fp)
		throw (FileException) {

	// Small optimisation, if no filename is provided then don't bother doing
	// anything.
	if (0 == fp.compare("")) {

		throw FileException("No file name provided.");
	}

	// Create a path class, this provides easy access to a file paths
	// different elements.
	boost::filesystem::path filePath(fp);

	// Another small optimisation, use the boost filesystem exists function to
	// check if the provided file does not exist and if it doesn't again don't
	// bother with any further processing.
	if (!boost::filesystem::exists(filePath)) {

		throw FileException("File " + fp + " does not exist.");
	}

	return filePath;
}

} /* namespace utils */
} /* namespace transcode */
