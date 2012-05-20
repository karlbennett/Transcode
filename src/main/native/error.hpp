/*
 * error.hpp
 *
 *  Created on: 11 Mar 2012
 *      Author: karl
 */

#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <exception>
#include <string>

#include <iostream>

namespace transcode {

/**
 * An exception that is thrown if something goes wrong during
 * media processing.
 */
class MediaException: public std::exception {

private:
    std::string _message;

public:
    /**
     * Instantiate an empty MediaException object.
     */
    MediaException() throw () :
            exception(), _message("") {
    }

    /**
     * Instantiate a MediaException object with the provided message.
     *
     * @param message - the message for the new exception.
     */
    MediaException(const std::string& message) throw () :
            exception(), _message(message) {
    }

    ~MediaException() throw () {
    }

    const char* what() const throw () {
        return _message.c_str();
    }
};

class IOException: public MediaException {

public:
    IOException() throw () :
            MediaException() {
    }

    IOException(const std::string& message) throw () :
            MediaException(message) {
    }

    ~IOException() throw () {
    }
};

class IllegalArgumentException: public MediaException {

public:
    IllegalArgumentException() throw () :
            MediaException() {
    }

    IllegalArgumentException(const std::string& message) throw () :
            MediaException(message) {
    }

    ~IllegalArgumentException() throw () {
    }
};

class IllegalStateException: public MediaException {

public:
    IllegalStateException() throw () :
            MediaException() {
    }

    IllegalStateException(const std::string& message) throw () :
            MediaException(message) {
    }

    ~IllegalStateException() throw () {
    }
};

} /* transcode */

#endif /* __ERROR_HPP__ */
