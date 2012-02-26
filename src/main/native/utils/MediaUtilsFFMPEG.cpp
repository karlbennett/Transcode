/*
 * MediaUtils.cpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __MEDIA_UTILS_CPP__
#define __MEDIA_UTILS_CPP__

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <utils/MediaUtils.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>

/**
 * Initialise the map that will be used to find the mime type for a
 * select few libav CodecID's.
 */
static std::map<CodecID, std::string> initialiseCodecToMimeType() {

	std::map<CodecID, std::string> codecToMimeType;

	// Audio codecs.
	codecToMimeType[CODEC_ID_AC3] = "audio/x-ac3";
	codecToMimeType[CODEC_ID_FLAC] = "audio/x-flac";
	codecToMimeType[CODEC_ID_AAC] = "audio/mpeg";
	codecToMimeType[CODEC_ID_MP2] = "audio/mpeg";
	codecToMimeType[CODEC_ID_MP3] = "audio/mpeg";
	codecToMimeType[CODEC_ID_VORBIS] = "audio/x-vorbis";
	codecToMimeType[CODEC_ID_WMAV1] = "audio/x-wma";
	codecToMimeType[CODEC_ID_WMAV2] = "audio/x-wma";
	codecToMimeType[CODEC_ID_WMAVOICE] = "audio/x-wma";
	codecToMimeType[CODEC_ID_WMAPRO] = "audio/x-wma";
	codecToMimeType[CODEC_ID_WMALOSSLESS] = "audio/x-wma";

	// Video codecs.
	codecToMimeType[CODEC_ID_MPEG4] = "video/x-divx";
	codecToMimeType[CODEC_ID_H264] = "video/x-h264";
	codecToMimeType[CODEC_ID_MPEG1VIDEO] = "video/mpeg";
	codecToMimeType[CODEC_ID_MPEG2VIDEO] = "video/mpeg";
	codecToMimeType[CODEC_ID_MPEG2VIDEO_XVMC] = "video/mpeg";
	codecToMimeType[CODEC_ID_THEORA] = "video/x-theora";
	codecToMimeType[CODEC_ID_WMV1] = "video/x-wmv";
	codecToMimeType[CODEC_ID_WMV2] = "video/x-wmv";
#if LIBAVCODEC_VERSION_MAJOR < 53
	codecToMimeType[CODEC_ID_XVID] = "video/x-xvid";
#endif

	return codecToMimeType;
}

/**
 * Initialise the map that will be used to find the mime type for a
 * select libav container names.
 */
static std::map<std::string, std::string> initialiseNameToMimeType() {

	std::map<std::string, std::string> nameToMimeType;

	nameToMimeType["asf"] = "video/x-ms-asf";
	nameToMimeType["avi"] = "video/x-msvideo";
	nameToMimeType["matroska"] = "video/x-matroska";
	nameToMimeType["mpeg"] = "video/mpeg";
	nameToMimeType["ogg"] = "application/ogg";
	nameToMimeType["mov,mp4,m4a,3gp,3g2,mj2"] = "video/quicktime";
	nameToMimeType["wav"] = "audio/x-wav";

	return nameToMimeType;
}

/**
 * The map that is used to look up mime types from libav CodecID's.
 */
std::map<CodecID, std::string> CODEC_TO_MIMETYPE = initialiseCodecToMimeType();

/**
 * The map that is used to look up mime types from libav container names.
 */
std::map<std::string, std::string> NAME_TO_MIMETYPE =
		initialiseNameToMimeType();


/**
 * Extract the audio details from the provided libav AVFormatContext.
 *
 * @param videoFile - the av format context that contains the audio details to extract.
 *
 * @return a vector containing all the extracted audio details.
 */
static std::vector<transcode::AudioDetail> extractAudioDetails(
		const AVFormatContext& videoFile) {

	// Simplify the use of the transcode api.
	using namespace transcode;

	std::vector<AudioDetail> audioDetails;

	AVCodecContext *codec;

	// Iterate through the streams searching for any audio streams.
	for (int i = 0; i < videoFile.nb_streams; i++) {

		codec = videoFile.streams[i]->codec;

		// If we find an audio stream use its details to construct an AudioDetail
		// struct.
		if (codec->codec_type == AVMEDIA_TYPE_AUDIO) {

			AudioDetail audioDetail(CODEC_TO_MIMETYPE[codec->codec_id],
					codec->bit_rate, codec->channels);

			audioDetails.push_back(audioDetail);
		}
	}

	return audioDetails;
}

/**
 * Extract the video details from the provided libav AVFormatContext.
 *
 * @param videoFile - the av format context that contains the video details to extract.
 *
 * @return a vector containing all the extracted video details.
 */
static std::vector<transcode::VideoDetail> extractVideoDetails(
		const AVFormatContext& videoFile) {

	// Simplify the use of the transcode api.
	using namespace transcode;

	std::vector<VideoDetail> videoDetails;

	AVCodecContext *codec;

	// Iterate through the streams searching for any video streams.
	for (int i = 0; i < videoFile.nb_streams; i++) {

		codec = videoFile.streams[i]->codec;

		// If we find a video stream use its details to construct an VideoDetail
		// struct.
		if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {

			VideoDetail videoDetail(CODEC_TO_MIMETYPE[codec->codec_id],
					codec->width, codec->height, codec->frame_number);

			videoDetails.push_back(videoDetail);
		}
	}

	return videoDetails;
}

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
static void closeCodecs(AVFormatContext& videoFile) {

	AVCodecContext *codec;

	for (int i = 0; i < videoFile.nb_streams; i++) {

		codec = videoFile.streams[i]->codec;

		// Try and free the codec. If it fails throw an exception.
		if (0 > avcodec_close(codec)) {

			std::stringstream errorMessage;
			errorMessage << "Could not close codec " << i << "of type " <<
					CODEC_TO_MIMETYPE[codec->codec_id] << ".";

			throw transcode::MediaUtilsException(errorMessage.str());
		}
	}
}

namespace transcode {

MediaFileDetail findMediaFileDetails(const std::string& fp) {

	// Small optimisation, if no filename is provided then don't bother doing
	// anything.
	if (0 == fp.compare("")) {

		throw MediaUtilsException("No file name provided.");
	}

	// Use the boost fielsystem api to find the files name and size.
	using namespace boost::filesystem;

	// Create a path class, this provides easy access to a file paths
	// different elements.
	path filePath(fp);

	// Another small optimisation, use the boost filesystem exists function to
	// check if the provided file does not exist and if it doesn't again don't
	// bother with any further processing.
	if (!exists(filePath)) {

		throw MediaUtilsException("File " + fp + " does not exist.");
	}

	// Get the files name from the file path object.
	std::string fileName = filePath.filename();
	// Use the boost file system file size function to find the size
	// of the media file.
	int fileSize = file_size(filePath);

	// Initialise the ffmpeg libav library so we can use it to inspect the media file.
	avcodec_init();
	avcodec_register_all();
	av_register_all();

	// Open the media file. This will populate the AVFormatContext
	// with all the information about this media file.
	AVFormatContext *videoFile;
	if (av_open_input_file(&videoFile, fp.c_str(), NULL, 0, NULL) < 0) {

		throw MediaUtilsException("Could not open the media file");
	}

	// Find the mime type for the media files container.
	std::string containerMimeType = NAME_TO_MIMETYPE[videoFile->iformat->name];
	// Find the description for the media files container.
	std::string description = videoFile->iformat->long_name;

	// Extract all the audio codecs.
	std::vector<AudioDetail> audioDetails = extractAudioDetails(*videoFile);
	// Extract all the video codecs.
	std::vector<VideoDetail> videoDetails = extractVideoDetails(*videoFile);

	// Construct a container struct with all the previously gathered details.
	ContainerDetail container(containerMimeType, description, audioDetails,
			videoDetails);

	// Clean up the lib av structs.
	closeCodecs(*videoFile);
	av_close_input_file(videoFile);

	// Then lastly return a fully populated MediaFileDetail struct.
	return MediaFileDetail(container, fp, fileName, fileSize);
}

}

#endif /* __MEDIA_UTILS_CPP__ */
