/*
 * FileUtils.hpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#ifndef __UTIL_FILE_HPP__
#define __UTIL_FILE_HPP__

#include <string>
#include <exception>

/**
 * @file util_file.hpp
 *
 * A file API that provides a simplification of C++ file access.
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
namespace utils {

class FileException: public std::exception {

private:
    std::string message_;

public:
    /**
     * Default constructor, set the message to empty string.
     */
    FileException() throw () :
            exception(), message_("") {
    }

    /**
     * Instantiate a FileException object with the provided message.
     */
    FileException(std::string message) throw () :
            exception(), message_(message) {
    }

    ~FileException() throw () {
    }

    const char* what() const throw () {
        return message_.c_str();
    }
};

/**
 * A class that represents a file on the file system, it can be used
 * to access meta data about the file.
 */
class File {

private:
    std::string name_;
    std::string path_;
    std::string relativePath_;
    std::string absolutePath_;
    unsigned long size_;

public:

    /**
     * Instantiate an empty File object.
     */
    File() :
            name_(""),
                    path_(""),
                    relativePath_(""),
                    absolutePath_(""),
                    size_(0) {
    }

    /**
     * Instantiate a new File object that will be populated from the
     * meta data of the file at the provided path.
     *
     * @param path - the path to the file that will be inspected.
     */
    File(std::string path) throw (FileException);

    /**
     * Get the name of the related file minus the path.
     *
     * @return the name of the file.
     */
    std::string getName() const
    {
        return name_;
    }

    /**
     * Get the full path of the file that was used to create this object.
     *
     * @return the path of the file.
     */
    std::string getPath() const
    {
        return path_;
    }

    /**
     * Get the absolute path of the file.
     *
     * @return the absolute path of the file.
     */
    std::string getAbsolutePath() const
    {
        return absolutePath_;
    }

    /**
     * Get the relative path of the file to the current running directory.
     *
     * @return the relative path of the file.
     */
    std::string getRelativePath() const
    {
        return relativePath_;
    }

    /**
     * Get the size of the related file in bytes.
     *
     * @return the size of the file.
     */
    unsigned long getSize() const
    {
        return size_;
    }

};

} /* namespace utils */
} /* namespace transcode */

#endif /* __UTIL_FILE_HPP__ */
