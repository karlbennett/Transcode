/*
 * MediaUtils.cpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <util/util_media.hpp>
#include <metadata.hpp>
#include <error.hpp>
#include <util/util_file.hpp>
#include <util/util_ffmpeg.hpp>

#include <string>
#include <vector>
#include <tr1/functional>

namespace helper {

/**
 * Function to create a new File and wrap the exception with
 * a MediaUtilsException.
 *
 * @param path - the path to the file on the file system.
 *
 * @return a new File object related to the provided path.
 */
static transcode::utils::File checkedFile(const std::string& path)
        throw (transcode::UtilMediaException) {

    using namespace transcode::utils;

    try {

        return File(path);

    } catch (std::exception& e) {

        throw transcode::UtilMediaException(e.what());
    }

    return File(); // We should never get here.
}

/**
 * A function to retrieve a AVFormatContext for the media file at the provided
 * path while also making sure the file is usable.
 *
 * @param path - the path to the file on the file system.
 *
 * @return the AVFormatContext for the provided media file.
 */
static AVFormatContext* retrieveCheckedAVFormatContext(
        const std::string& path) throw (transcode::UtilMediaException) {

    using namespace transcode::utils;

    File file = checkedFile(path);

    try {

        return retrieveAVFormatContext(file.getPath());

    } catch (transcode::MediaException& e) {

        throw transcode::UtilMediaException(e.what());
    }

    return NULL; // We should never get here.
}

/**
 * Helper template used to wrap an extract details function so that
 * it's exception can be converted to a UtilMediaException.
 *
 * @param formatContext - the format context that will be passed to
 *      the extract details function.
 * @param detailsCallback - the function that will be called with
 *      the format context.
 *
 * @return a populated construct of the requested type.
 */
template<typename T> T extractCheckedDetails(
        const AVFormatContext *formatContext,
        std::tr1::function<T(const AVFormatContext*)> detailsCallback)
                throw (transcode::UtilMediaException) {

    T details;

    try {

        details = detailsCallback(formatContext);

    } catch (std::exception& e) {

        throw transcode::UtilMediaException(e.what());
    }

    return details;
}

}

namespace transcode {

std::vector<SubtitleMetaData> findSubtitleDetails(const std::string& path)
        throw (UtilMediaException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return helper::extractCheckedDetails< std::vector<SubtitleMetaData> >(
            videoFile, transcode::utils::extractSubtitleDetails);
}

std::vector<AudioMetaData> findAudioDetails(const std::string& path)
        throw (UtilMediaException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return helper::extractCheckedDetails< std::vector<AudioMetaData> >(
            videoFile, transcode::utils::extractAudioDetails);
}

std::vector<VideoMetaData> findVideoDetails(const std::string& path)
        throw (UtilMediaException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return helper::extractCheckedDetails< std::vector<VideoMetaData> >(
            videoFile, transcode::utils::extractVideoDetails);
}

ContainerMetaData findContainerDetails(const std::string& path)
        throw (UtilMediaException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return helper::extractCheckedDetails<ContainerMetaData>(
            videoFile, transcode::utils::buildContainerDetail);
}

MediaFileMetaData findMediaFileDetails(const std::string& path)
        throw (UtilMediaException) {

    using namespace transcode::utils;

    File file = helper::checkedFile(path);

    // Get the files name from the file path object.
    std::string fileName = file.getName();
    // Use the boost file system file size function to find the size
    // of the media file.
    unsigned long fileSize = file.getSize();

    // Initialise FFMPEG and get the AVFormatContext for the provided file.
    // If the AVFormatContext cannot be opened a MediaUtilsException is
    // thrown.
    AVFormatContext *formatContext = helper::retrieveCheckedAVFormatContext(
            file.getPath());

    // Build a container struct from the AVFormatContext.
    //ContainerMetaData container = buildContainerDetail(videoFile);
    ContainerMetaData container = helper::extractCheckedDetails<
            ContainerMetaData>(formatContext, buildContainerDetail);

    // Clean up the lib av structs.
    closeCodecs(formatContext);
    av_close_input_file(formatContext);

    // Then lastly return a fully populated MediaFileDetail struct.
    return MediaFileMetaData(container, path, fileName, fileSize);
}

} /* namespace transcode */
