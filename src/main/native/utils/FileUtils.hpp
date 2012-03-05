/*
 * FileUtils.hpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#ifndef __FILEUTILS_HPP__
#define __FILEUTILS_HPP__

#include <string>
#include <exception>
#include <boost/filesystem.hpp>

namespace transcode {
namespace utils {

class FileException: public std::exception {

private:
	std::string message;

public:
	/**
	 * Default constructor, set the message to empty string.
	 */
	FileException() throw() :
			exception(), message("") {
	}

	/**
	 * Instantiate a FileException object with the provided message.
	 */
	FileException(std::string msg) throw() :
			exception(), message(msg) {
	}

	~FileException() throw() {}

	const char* what() const throw () {
		return message.c_str();
	}
};

/**
 * Check to make sure we are working with an actual file.
 *
 * @param fp - the path to the file that will have it's details inspected.
 *
 * @return a boost filesystem path class related to the provided file.
 */
boost::filesystem::path checkFile(const std::string& fp) throw(FileException);

} /* namespace utils */
} /* namespace transcode */

#endif /* __FILEUTILS_HPP__ */
