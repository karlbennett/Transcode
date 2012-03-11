/*
 * FileUtils.cpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#include <utils/FileUtils.hpp>

#include <string>
#include <boost/filesystem.hpp>

namespace transcode {
namespace utils {

File::File(std::string path) throw (FileException) :
        name_(""),
                path_(""),
                relativePath_(""),
                absolutePath_(""),
                size_(0) {

    // Small optimisation, if no filename is provided then don't bother doing
    // anything.
    if (0 == path.compare("")) {

        throw FileException("No file name provided.");
    }

    path_ = path; // Now that we know the path is ok record it.

    // Create a path class, this provides easy access to a file paths
    // different elements.
    boost::filesystem::path filePath(path);

    // Another small optimisation, use the boost filesystem exists function to
    // check if the provided file does not exist and if it doesn't again don't
    // bother with any further processing.
    if (!boost::filesystem::exists(filePath)) {

        throw FileException("File " + path + " does not exist.");
    }

    // Lastly the path must point to an actual file not a directory of symlink.
    if (!boost::filesystem::is_regular_file(filePath)) {

        throw FileException("Path (" + path + ") does not point to a file.");
    }

    name_ = filePath.filename();
    absolutePath_ = filePath.canonize().string();
    relativePath_ = filePath.relative_path().string();
    size_ = boost::filesystem::file_size(filePath);
}

} /* namespace utils */
} /* namespace transcode */
