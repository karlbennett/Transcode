/*
 * ffmpeg.hpp
 *
 *  Created on: 10 Mar 2012
 *      Author: karl
 */

#ifndef __UTIL_FFMPEG_HPP__
#define __UTIL_FFMPEG_HPP__

extern "C" {
#include "libavutil/avutil.h"
}

#include <media.hpp>
#include <error.hpp>

#include <string>
#include <vector>
#include <map>

#include <boost/tr1/memory.hpp>

/**
 * @file util_ffmpeg.hpp
 *
 * The FFMPEG utility functions that provide a simple
 * abstraction of the libav API's.
 */

struct AVFormatContext;
struct AVPacket;

/**
 * Transcode namespace, all the top level transcode functions and classes are in
 * this namespace. So this namespace holds officially public API.
 */
namespace transcode {

struct SubtitleMetaData;
struct AudioMetaData;
struct VideoMetaData;
struct ContainerMetaData;
struct MediaFileMetaData;

/**
 * Util namespace, all the untility functions and classes are found within this
 * namespace. You might find something useful here but hopefully everything in the
 * {@see transcode} namespace should provide what you need so you shouldn't have to
 * look in here.
 */
namespace util {

const std::string UNKNOWN = "Unknown";

/**
 * Get the FFMPEG message for the provided error code.
 *
 * @param errorCode - the FFMPEG error code.
 *
 * @return the error message that relates to the provided error code.
 */
std::string ffmpegErrorMessage(int errorCode);

/**
 * Exception that is thrown if something goes wrong with the FFMPEG library.
 */
class FFMPEGException: public MediaException {

private:
    int errorCode;

public:
    /**
     * Default constructor, set message to empty string and error code to 0.
     */
    FFMPEGException() throw () :
            MediaException(), errorCode(0) {
    }

    /**
     * Instantiate a new FFMPEGException with the provided error message and
     * set the error code to 0.
     *
     * @param msg - the message for this exception.
     */
    FFMPEGException(const std::string& msg) throw () :
            MediaException(msg), errorCode(0) {
    }

    /**
     * Instantiate a new FFMPEGException with the provided error code and the
     * message set automatically from the error code.
     *
     * @param ec - the FFMPEG error code for this exception.
     */
    FFMPEGException(const int& ec) throw () :
            MediaException(ffmpegErrorMessage(ec)), errorCode(ec) {
    }

    /**
     * Instantiate an FFMPEGException with the provided message and error code.
     *
     * @param msg - the message for this exception.
     * @param ec - the FFMPEG error code for this exception.
     */
    FFMPEGException(const std::string& msg, const int& ec) throw () :
            MediaException(msg), errorCode(ec) {
    }

    ~FFMPEGException() throw () {
    }

    /**
     * Get the error code for this exception.
     *
     * @return the error code for this exception.
     */
    int getErrorCode() {
        return errorCode;
    }
};

/**
 * Retrieve the AVFormatContext for the provided media file. This can then
 * be used to extract the files meta data and media streams.
 *
 * @param filePath - the path to the media file.
 *
 * @return the AVFormatContext for the provided media file.
 */
AVFormatContext* retrieveAVFormatContext(const std::string& filePath)
        throw (FFMPEGException);

/**
 * Extract the subtitle details from the provided libav AVFormatContext.
 *
 * @param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing subtitle detail structs populated from
 *      values within the media file.
 */
std::vector<SubtitleMetaData> extractSubtitleDetails(
        const AVFormatContext *videoFile) throw (FFMPEGException);

/**
 * Extract the audio detail from the provided libav AVFormatContext.
 *
 * @param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing audio detail structs populated from values
 *      within the media file.
 */
std::vector<AudioMetaData> extractAudioDetails(const AVFormatContext *videoFile)
        throw (FFMPEGException);

/**
 * Extract the video detail from the provided libav AVFormatContext.
 *
 * @@param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing video detail structs populated from values
 *      within the media file.
 */
std::vector<VideoMetaData> extractVideoDetails(const AVFormatContext *videoFile)
        throw (FFMPEGException);

/**
 * Build a ContainerDetail struct out of the details in the provided
 * AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that contains the details to be
 *          used to populate the ContainerDetail.
 *
 * @return a populated ContainerDetail struct.
 */
ContainerMetaData buildContainerDetail(const AVFormatContext *videoFile)
        throw (FFMPEGException);

/**
 * Get the next generic packet from the AVFormatContext.
 *
 * NOTE: You must free the AVPacket with av_free_packet(AVPacket*) when your
 * are finished with it.
 *
 * @param videoFile - the AVFormatContext that will have the packet extracted
 *          from it.
 * @return the next packet or NULL if we have reached the end of the file.
 */
AVPacket* readNextPacket(AVFormatContext *videoFile) throw (FFMPEGException);

/**
 * Find the type of the provided packet. That is if it is an audio or video
 * packet.
 *
 * @param packet - the packet that is to be checked.
 * @param videoFile - the format context that the packet was read from.
 */
AVMediaType findPacketType(const AVPacket *packet,
        const AVFormatContext *videoFile) throw (FFMPEGException);

/**
 * Decode the provided packet into decoded frames.
 *
 * @param packet - the packet that contains the data to decode.
 * @param videoFile - the format context that the packet was read from.
 * @return a vector of  DeocdedFrames generated from the provided packet.
 */
//std::vector<std::tr1::shared_ptr<DecodedFrame> > decodeFrame(
//        const AVPacket *packet, const AVFormatContext *videoFile)
//                throw (FFMPEGException);

/**
 * Decode the provided packet into decoded audio frames.
 *
 * @param packet - the packet that contains the data to decode.
 * @param videoFile - the format context that the packet was read from.
 * @return a vector of DeocdedAudioFrames generated from the provided packet.
 */
//std::vector<std::tr1::shared_ptr<DecodedAudioFrame> > decodeAudioFrame(
//        const AVPacket *packet, const AVFormatContext *videoFile)
//                throw (FFMPEGException);

/**
 * Decode the provided packet into a decoded video frames.
 *
 * @param packet - the packet that contains the data to decode.
 * @param videoFile - the format context that the packet was read from.
 * @return a DeocdedVideoFrame generated from the provided packet.
 */
//std::tr1::shared_ptr<DecodedVideoFrame> decodeVideoFrame(const AVPacket *packet,
//        const AVFormatContext *videoFile) throw (FFMPEGException);

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException);

} /* namspace util */
} /* namspace transcode */

#endif /* __UTIL_FFMPEG_HPP__ */
