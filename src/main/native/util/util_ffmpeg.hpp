/*
 * ffmpeg.hpp
 *
 *  Created on: 10 Mar 2012
 *      Author: karl
 */

#ifndef __UTILS_FFMPEG_HPP__
#define __UTILS_FFMPEG_HPP__

#include <util/util_media.hpp>

#include <string>
#include <vector>
#include <map>

struct AVFormatContext;

struct SubtitleDetail;
struct AudioDetail;
struct VideoDetail;
struct ContainerDetail;

namespace transcode {
namespace utils {

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
class FFMPEGException: public UtilMediaException {

private:
    int errorCode;

public:
    /**
     * Default constructor, set message to empty string and error code to 0.
     */
    FFMPEGException() throw () :
            UtilMediaException(), errorCode(0) {
    }

    /**
     * Instantiate a new FFMPEGException with the provided error message and
     * set the error code to 0.
     *
     * @param msg - the message for this exception.
     */
    FFMPEGException(const std::string& msg) throw () :
            UtilMediaException(msg), errorCode(0) {
    }

    /**
     * Instantiate a new FFMPEGException with the provided error code and the
     * message set automatically from the error code.
     *
     * @param ec - the FFMPEG error code for this exception.
     */
    FFMPEGException(const int& ec) throw () :
            UtilMediaException(ffmpegErrorMessage(ec)), errorCode(ec) {
    }

    /**
     * Instantiate an FFMPEGException with the provided message and error code.
     *
     * @param msg - the message for this exception.
     * @param ec - the FFMPEG error code for this exception.
     */
    FFMPEGException(const std::string& msg, const int& ec) throw () :
            UtilMediaException(msg), errorCode(ec) {
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
std::vector<SubtitleDetail> extractSubtitleDetails(
        const AVFormatContext *videoFile) throw (FFMPEGException);

/**
 * Extract the audio detail from the provided libav AVFormatContext.
 *
 * @param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing audio detail structs populated from values
 *      within the media file.
 */
std::vector<AudioDetail> extractAudioDetails(const AVFormatContext *videoFile)
        throw (FFMPEGException);

/**
 * Extract the video detail from the provided libav AVFormatContext.
 *
 * @@param videoFile - the av context that contains the media file details.
 *
 * @return a vector containing video detail structs populated from values
 *      within the media file.
 */
std::vector<VideoDetail> extractVideoDetails(const AVFormatContext *videoFile)
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
ContainerDetail buildContainerDetail(const AVFormatContext *videoFile)
        throw (FFMPEGException);

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException);

} /* utils */
} /* transcode */

#endif /* __UTILS_FFMPEG_HPP__ */
