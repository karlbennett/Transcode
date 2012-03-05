/*
 * MediaUtils.cpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __MEDIA_FFMPEG_UTILS_CPP__
#define __MEDIA_FFMPEG_UTILS_CPP__

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <utils/MediaUtils.hpp>
#include <utils/StandardUtils.hpp>
#include <utils/FileUtils.hpp>
#include <utils/FFMPEGUtils.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <exception>
#include <tr1/functional>
#include <boost/filesystem.hpp>

namespace transcode {

std::vector<SubtitleDetail> findSubtitleDetails(const std::string& fp)
		throw (MediaUtilsException) {

	using namespace transcode::utils;

	return findDetails<SubtitleDetail>(fp, AVMEDIA_TYPE_SUBTITLE,
			extractSubtitleDetail);
}

std::vector<AudioDetail> findAudioDetails(const std::string& fp)
		throw (MediaUtilsException) {

	using namespace transcode::utils;

	return findDetails<AudioDetail>(fp, AVMEDIA_TYPE_AUDIO, extractAudioDetail);
}

std::vector<VideoDetail> findVideoDetails(const std::string& fp)
		throw (MediaUtilsException) {

	using namespace transcode::utils;

	return findDetails<VideoDetail>(fp, AVMEDIA_TYPE_VIDEO, extractVideoDetail);
}

ContainerDetail findContainerDetails(const std::string& fp)
		throw (MediaUtilsException) {

	using namespace transcode::utils;

	try {

		(void) checkFile(fp);

	} catch (std::exception& e) {

		throw MediaUtilsException(e.what());
	}

	AVFormatContext *videoFile = initialiseFFMPEG(fp);

	return buildContainerDetail(videoFile);
}

MediaFileDetail findMediaFileDetails(const std::string& fp)
		throw (MediaUtilsException) {

	using namespace transcode::utils;

	// Check to make sure we are working with a real file.
	// If not throw a MediaUtilsException.
	boost::filesystem::path filePath;
	try {

		filePath = checkFile(fp);

	} catch (std::exception& e) {

		throw MediaUtilsException(e.what());
	}

	// Get the files name from the file path object.
	std::string fileName = filePath.filename();
	// Use the boost file system file size function to find the size
	// of the media file.
	int fileSize = file_size(filePath);

	// Initialise FFMPEG and get the AVFormatContext for the provided file.
	// If the AVFormatContext cannot be opened a MediaUtilsException is
	// thrown.
	AVFormatContext *videoFile = initialiseFFMPEG(fp);

	// Build a container struct from the AVFormatContext.
	ContainerDetail container = buildContainerDetail(videoFile);

	// Clean up the lib av structs.
	closeCodecs(videoFile);
	av_close_input_file(videoFile);

	// Then lastly return a fully populated MediaFileDetail struct.
	return MediaFileDetail(container, fp, fileName, fileSize);
}

} /* namespace transcode */

#endif /* __MEDIA_FFMPEG_UTILS_CPP__ */
