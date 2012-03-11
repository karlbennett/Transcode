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

#include <utils/MediaUtils.hpp>
#include <utils/StandardUtils.hpp>
#include <utils/FileUtils.hpp>
#include <utils/ffmpeg.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <exception>
#include <tr1/functional>
#include <boost/filesystem.hpp>

namespace helper {

/**
 * Function to create a new File and wrap the exception with
 * a MediaUtilsException.
 *
 * @param path - the path to the file on the file system.
 *
 * @return a new File object related to the provided path.
 */
static transcode::utils::File checkedFile(const std::string& path) {

    using namespace transcode::utils;

    try {

        return File(path);

    } catch (std::exception& e) {

        throw transcode::MediaUtilsException(e.what());
    }

    return File(); // We should never get here.
}

static AVFormatContext* retrieveCheckedAVFormatContext(const std::string& path) {

    using namespace transcode::utils;

    File file = checkedFile(path);

    return retrieveAVFormatContext(file.getPath());
}

}

namespace transcode {

std::vector<SubtitleDetail> findSubtitleDetails(const std::string& path)
        throw (MediaUtilsException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return transcode::utils::extractSubtitleDetails(videoFile);
}

std::vector<AudioDetail> findAudioDetails(const std::string& path)
        throw (MediaUtilsException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return transcode::utils::extractAudioDetails(videoFile);
}

std::vector<VideoDetail> findVideoDetails(const std::string& path)
        throw (MediaUtilsException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return transcode::utils::extractVideoDetails(videoFile);
}

ContainerDetail findContainerDetails(const std::string& path)
        throw (MediaUtilsException) {

    AVFormatContext *videoFile = helper::retrieveCheckedAVFormatContext(path);

    return transcode::utils::buildContainerDetail(videoFile);
}

MediaFileDetail findMediaFileDetails(const std::string& path)
        throw (MediaUtilsException) {

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
    AVFormatContext *videoFile = retrieveAVFormatContext(file.getPath());

    // Build a container struct from the AVFormatContext.
    ContainerDetail container = buildContainerDetail(videoFile);

    // Clean up the lib av structs.
    closeCodecs(videoFile);
    av_close_input_file(videoFile);

    // Then lastly return a fully populated MediaFileDetail struct.
    return MediaFileDetail(container, path, fileName, fileSize);
}

} /* namespace transcode */
