/*
 * error.hpp
 *
 *  Created on: 11 Mar 2012
 *      Author: karl
 */

#ifndef __ERROR_HPP__
#define __ERROR_HPP__

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
     */
    MediaException(std::string message) throw () :
            exception(), message_(message) {
    }

    ~MediaUtilsException() throw () {
    }

    const char* what() const throw () {
        return message_.c_str();
    }
};


#endif /* __ERROR_HPP__ */
