/*
 * MediaUtils.hpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __UTIL_MEDIA_HPP__
#define __UTIL_MEDIA_HPP__

#include <error.hpp>

#include <string>
#include <exception>
#include <vector>
#include <boost/rational.hpp>

/**
 * @file util_media_ffmpeg.hpp
 *
 * A simple API for accessing the meta data and manipulation of media
 * files.
 */

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
 * An exception that is thrown from within the MediaUtils functions.
 */
class UtilMediaException: public MediaException {

public:
    /**
     * Instantiate an empty MediaException object.
     */
    UtilMediaException() throw () :
            MediaException() {
    }

    /**
     * Instantiate a MediaUtilsException object with the provided message.
     *
     * @param message - the message for the new exception.
     */
    UtilMediaException(std::string message) throw () :
            MediaException(message) {
    }

    ~UtilMediaException() throw () {
    }
};

/**
 * Find the subtitle details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the subtitle details for the provided media file if possible.
 */
std::vector<SubtitleMetaData> findSubtitleDetails(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the audio details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the audio details for the provided media file if possible.
 */
std::vector<AudioMetaData> findAudioDetails(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the video details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the video details for the provided media file if possible.
 */
std::vector<VideoMetaData> findVideoDetails(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the container details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the container details for the provided media file if possible.
 */
ContainerMetaData findContainerDetails(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the details for the provided media file if possible.
 */
MediaFileMetaData findMediaFileDetails(const std::string& filePath)
        throw (UtilMediaException);

}

#endif /* __UTIL_MEDIA_HPP__ */
