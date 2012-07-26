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

#include <string>
#include <vector>

/**
 * @file libav.hpp
 *
 * The libav utility functions that provide a simple
 * abstraction of the libav API's.
 */

struct AVFormatContext;
struct AVStream;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;


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


/**
 * Return the error message string for the supplied error code.
 *
 * @param errorCode - the error code number to find the message for.
 * @return the error message for the supplied error code.
 */
std::string errorMessage(const int& errorCode);

/**
 * Open a libav format context for the media file that has the
 * supplied file name.
 *
 * @param fileName - the name for the file to open the format
 *      context for.
 * @retrun the newly opened format context.
 */
AVFormatContext* openFormatContext(const std::string& fileName);

/**
 * Close the supplied format context.
 *
 * @param formatContext - the format context to close.
 */
void closeFormatContext(AVFormatContext **formatContext);

/**
 * Read the next packet from the supplied format context.
 *
 * @param formatContext - the format context to close.
 */
AVPacket* readNextPacket(AVFormatContext *formatContext);

/**
 * Find the media type for the provided stream.
 *
 * @param stream - the stream to find the media type for.
 * @return the media type of the supplied stream.
 */
AVMediaType findStreamType(const AVStream *stream);

/**
 * Find the media type for the provided codec context.
 *
 * @param codecContext - the codec context to find the
 *      media type for.
 * @return the media type of the supplied codec context.
 */
AVMediaType findCodecType(const AVCodecContext *codecContext);

/**
 * Find the media type for the provided packet.
 *
 * @param packet - the packet to find the media type for.
 * @param formatContext - the formatContext to use to find
 *      the packets media type. This should be the format
 *      context that the packet was read from.
 * @return the media type of the supplied packet.
 */
AVMediaType findPacketType(const AVFormatContext *formatContext,
        const AVPacket *packet);

/**
 * Open the supplied codec context to be used for decoding.
 *
 * Note: This function opens the codec context instance
 * that it was given, it does not create a copy then open
 * that.
 *
 * @param codecContext - the codec context to open.
 * @return the newly opened codec context.
 */
AVCodecContext* openDecodeCodecContext(AVCodecContext *codecContext);

/**
 * Open the supplied codec context to be used for encoding.
 *
 * Note: This function opens the codec context instance
 * that it was given, it does not create a copy then open
 * that.
 *
 * @param codecContext - the codec context to open.
 * @return the newly opened codec context.
 */
AVCodecContext* openEncodeCodecContext(AVCodecContext *codecContext);

/**
 * Close the supplied codec context.
 *
 * Note: This function closes the codec context instance
 * that it was given, it does not create a copy then close
 * that.
 *
 * @param codecContext - the codec context to close.
 */
void closeCodecContext(AVCodecContext **codecContext);

/**
 * Decode the supplied audio packet.
 *
 * @param codecContext - the codec to use to decode the
 *      audio packet.
 * @param packet - the audio packet to be decoded.
 * @return a vector of frames that have been decoded from
 *      the supplied audio packet.
 */
std::vector<AVFrame*> decodeAudioPacket(AVCodecContext *codecContext,
        const AVPacket *packet);

/**
 * Encode the supplied audio frame.
 *
 * @param codecContext - the codec to use to encode the
 *      audio frame.
 * @param frame - the audio frame that is to be encoded.
 * @return the encoded audio packet.
 */
AVPacket* encodeAudioFrame(AVCodecContext *codecContext,
        const AVFrame *frame);

/**
 * Decode the supplied video packet.
 *
 * @param codecContext - the codec to use to decode the
 *      video packet.
 * @param packet - the video packet to be decoded.
 * @return a frame that has been decoded from the
 *      supplied video packet.
 */
AVFrame* decodeVideoPacket(AVCodecContext *codecContext,
        const AVPacket *packet);

/**
 * Encode the supplied video frame.
 *
 * @param codecContext - the codec to use to encode the
 *      video frame.
 * @param frame - the video frame that is to be encoded.
 * @return the encoded video packet.
 */
AVPacket* encodeVideoFrame(AVCodecContext *codecContext,
        const AVFrame *frame);

} /* namespace util */
} /* namespace transcode */

#endif /* __UTIL_LIBAV_HPP__ */
