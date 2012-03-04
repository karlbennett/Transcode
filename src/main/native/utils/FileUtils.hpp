/*
 * FileUtils.hpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#ifndef __FILEUTILS_HPP__
#define __FILEUTILS_HPP__

#include <string>
#include <boost/filesystem.hpp>

namespace transcode {
namespace utils {

/**
 * Check to make sure we are working with an actual file.
 *
 * @param fp - the path to the file that will have it's details inspected.
 *
 * @return a boost filesystem path class related to the provided file.
 */
boost::filesystem::path checkFile(const std::string& fp);

} /* namespace utils */
} /* namespace transcode */

#endif /* __FILEUTILS_HPP__ */
