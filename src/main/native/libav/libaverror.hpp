/*
 * libaverror.hpp
 *
 *  Created on: 25 Jul 2012
 *      Author: karl
 */

#ifndef __LIBAVERROR_HPP__
#define __LIBAVERROR_HPP__


#include <error.hpp>


/**
 * Transcode namespace, all the top level transcode functions and classes are in
 * this namespace. So this namespace holds officially public API.
 */
namespace transcode {

/**
 * Libav namespace, all libav related functions and classes can be found within
 * this namespace.
 */
namespace libav {


const std::string UNKNOWN = "Unknown";

/**
 * A <code>PacketReadException</code> is thrown if an error
 * occurs when a packet is read from a media file.
 */
class PacketReadException: public IOException {

public:
    PacketReadException() throw () :
            IOException() {
    }

    PacketReadException(std::string message) throw () :
            IOException(message) {
    }

    ~PacketReadException() throw () {
    }
};

/**
 * A <code>CodecException</code> is thrown if a required
 * codec cannot be found or opened.
 */
class CodecException: public IllegalStateException {

public:
    CodecException() throw () :
            IllegalStateException() {
    }

    CodecException(std::string message) throw () :
            IllegalStateException(message) {
    }

    ~CodecException() throw () {
    }
};

/**
 * A <code>PacketDecodeException</code> is thrown if there
 * was an error during the decode of a packet.
 */
class PacketDecodeException: public Exception {

public:
    PacketDecodeException() throw () :
            Exception() {
    }

    PacketDecodeException(std::string message) throw () :
            Exception(message) {
    }

    ~PacketDecodeException() throw () {
    }
};

/**
 * An <code>InvalidPacketDataException</code> is thrown if a
 * decode was attempted on a packet containing invalid data.
 */
class InvalidPacketDataException: public Exception {

public:
    InvalidPacketDataException() throw () :
        Exception() {
    }

    InvalidPacketDataException(std::string message) throw () :
        Exception(message) {
    }

    ~InvalidPacketDataException() throw () {
    }
};

} /* namespace util */
} /* namespace transcode */


#endif /* __LIBAVERROR_HPP__ */
