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

namespace transcode {

/**
 * An exception that is thrown if something goes wrong during
 * media processing.
 */
class MediaException: public std::exception {

private:
    std::string message_;

public:
    /**
     * Instantiate an empty MediaException object.
     */
    MediaException() throw () :
            exception(), message_("") {
    }

    /**
     * Instantiate a MediaException object with the provided message.
     *
     * @param message - the message for the new exception.
     */
    MediaException(std::string message) throw () :
            exception(), message_(message) {
    }

    ~MediaException() throw () {
    }

    const char* what() const throw () {
        return message_.c_str();
    }
};

} /* transcode */

#endif /* __ERROR_HPP__ */
