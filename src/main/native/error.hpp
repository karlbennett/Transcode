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
class Exception: public std::exception {

private:
    std::string _message;

public:
    /**
     * Instantiate an empty MediaException object.
     */
    Exception() throw () :
            exception(), _message("") {
    }

    /**
     * Instantiate a MediaException object with the provided message.
     *
     * @param message - the message for the new exception.
     */
    Exception(const std::string& message) throw () :
            exception(), _message(message) {
    }

    ~Exception() throw () {
    }

    const char* what() const throw () {
        return _message.c_str();
    }
};

class IOException: public Exception {

public:
    IOException() throw () :
            Exception() {
    }

    IOException(const std::string& message) throw () :
            Exception(message) {
    }

    ~IOException() throw () {
    }
};

class IllegalArgumentException: public Exception {

public:
    IllegalArgumentException() throw () :
            Exception() {
    }

    IllegalArgumentException(const std::string& message) throw () :
            Exception(message) {
    }

    ~IllegalArgumentException() throw () {
    }
};

class IllegalStateException: public Exception {

public:
    IllegalStateException() throw () :
            Exception() {
    }

    IllegalStateException(const std::string& message) throw () :
            Exception(message) {
    }

    ~IllegalStateException() throw () {
    }
};

} /* transcode */

#endif /* __ERROR_HPP__ */
