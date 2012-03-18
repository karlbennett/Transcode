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
 * Util namespace, all the untility functions and classes are found within this
 * namespace. You might find something useful here but hopefully everything in the
 * {@see transcode} namespace should provide what you need so you shouldn't have to
 * look in here.
 */
namespace util {

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
 * Find the subtitle meta data for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the subtitle details for the provided media file if possible.
 */
std::vector<SubtitleMetaData> findSubtitleMetaData(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the audio meta data for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the audio details for the provided media file if possible.
 */
std::vector<AudioMetaData> findAudioMetaData(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the video meta data for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the video details for the provided media file if possible.
 */
std::vector<VideoMetaData> findVideoMetaData(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the container meta data for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the container details for the provided media file if possible.
 */
ContainerMetaData findContainerMetaData(const std::string& filePath)
        throw (UtilMediaException);

/**
 * Find the meta data for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the details for the provided media file if possible.
 */
MediaFileMetaData findMediaFileMetaData(const std::string& filePath)
        throw (UtilMediaException);

} /* namespace util */
} /* namespace transcode */

#endif /* __UTIL_MEDIA_HPP__ */
