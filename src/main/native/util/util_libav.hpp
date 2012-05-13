/*
 * util_ffmpeg2.hpp
 *
 *  Created on: 7 May 2012
 *      Author: karl
 */

#ifndef __UTIL_LIBAV_HPP__
#define __UTIL_LIBAV_HPP__

extern "C" {
#include "libavutil/avutil.h"
}

#include <error.hpp>

#include <string>
#include <vector>

/**
 * @file util_ffmpeg.hpp
 *
 * The FFMPEG utility functions that provide a simple
 * abstraction of the libav API's.
 */

struct AVFormatContext;
struct AVStream;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;

namespace transcode {
namespace util {

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
class PacketDecodeException: public MediaException {

public:
    PacketDecodeException() throw () :
            MediaException() {
    }

    PacketDecodeException(std::string message) throw () :
            MediaException(message) {
    }

    ~PacketDecodeException() throw () {
    }
};

/**
 * An <code>InvalidPacketDataException</code> is thrown if a
 * decode was attempted on a packet containing invalid data.
 */
class InvalidPacketDataException: public MediaException {

public:
    InvalidPacketDataException() throw () :
        MediaException() {
    }

    InvalidPacketDataException(std::string message) throw () :
        MediaException(message) {
    }

    ~InvalidPacketDataException() throw () {
    }
};

std::string errorMessage(const int& errorCode);

AVFormatContext* openFormatContext(const std::string& fileName);

void closeFormatContext(AVFormatContext *formatContext);

AVPacket* readNextPacket(AVFormatContext *fomratContext);

AVMediaType findStreamType(const AVStream *stream);

AVMediaType findCodecType(const AVCodecContext *codecContext);

AVMediaType findPacketType(const AVFormatContext *formatContext,
        const AVPacket *packet);

AVCodecContext* openCodecContext(AVCodecContext *codecContext);

std::vector<AVFrame*> decodeAudioPacket(AVCodecContext *codecContext,
        const AVPacket *packet);

AVFrame* decodeVideoPacket(AVCodecContext *codecContext,
        const AVPacket *packet);

}
}

#endif /* __UTIL_LIBAV_HPP__ */
