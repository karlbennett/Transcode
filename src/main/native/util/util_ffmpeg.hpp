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
struct AVFrame;
struct AVCodecContext;

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
AVFormatContext* retrieveAVFormatContext(const std::string& filePath);

/**
 * Extract the subtitle details from the provided libav AVFormatContext.
 *
 * @param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing subtitle detail structs populated from
 *      values within the media file.
 */
std::vector<SubtitleMetaData> extractSubtitleDetails(
        const AVFormatContext *videoFile);

/**
 * Extract the audio detail from the provided libav AVFormatContext.
 *
 * @param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing audio detail structs populated from values
 *      within the media file.
 */
std::vector<AudioMetaData> extractAudioDetails(const AVFormatContext *videoFile);

/**
 * Extract the video detail from the provided libav AVFormatContext.
 *
 * @@param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing video detail structs populated from values
 *      within the media file.
 */
std::vector<VideoMetaData> extractVideoDetails(const AVFormatContext *videoFile);

/**
 * Build a ContainerDetail struct out of the details in the provided
 * AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that contains the details to be
 *          used to populate the ContainerDetail.
 *
 * @return a populated ContainerDetail struct.
 */
ContainerMetaData buildContainerDetail(const AVFormatContext *videoFile);

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
AVPacket* readNextPacket(AVFormatContext *videoFile);

/**
 * Find the type of the provided packet. That is if it is an audio or video
 * packet.
 *
 * @param packet - the packet that is to be checked.
 * @param videoFile - the format context that the packet was read from.
 * @return the type of the packet.
 */
AVMediaType findPacketType(const AVPacket *packet,
        const AVFormatContext *videoFile);

/**
 * Opens the supplied ocdec context and then returns it.
 * NOTE: This function modifies the supplied codec context, this is because
 * of the way the libav API works. It is only returned to allow a more concise
 * usage.
 *
 * @param codecContext - the codec context that is to be opened.
 * @return the newly opened codec context.
 * @throws an FFMPEGException if the supplied codec context could not be opened.
 */
AVCodecContext* openCodecContext(AVCodecContext* codecContext);

/**
 * Decode the provided packet into decoded audio frames.
 *
 * @param packet - the packet that contains the data to decode.
 * @param videoFile - the format context that the packet was read from.
 * @return a vector of AVFrames generated from the provided packet.
 */
std::vector<AVFrame*> decodeAudioFrame(const AVPacket *packet,
        const AVFormatContext *videoFile);

/**
 * Decode the provided packet into a decoded video frames.
 *
 * @param packet - the packet that contains the data to decode.
 * @param videoFile - the format context that the packet was read from.
 * @return an AVFrame generated from the provided packet.
 */
AVFrame* decodeVideoFrame(const AVPacket *packet,
        const AVFormatContext *videoFile);

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
void closeCodecs(AVFormatContext *videoFile);

} /* namspace util */
} /* namspace transcode */

#endif /* __UTIL_FFMPEG_HPP__ */
