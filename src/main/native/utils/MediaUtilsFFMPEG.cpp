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
#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <tr1/functional>
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

	// Subtitle types.
	codecToMimeType[CODEC_ID_DVD_SUBTITLE] = "application/dvd";
	codecToMimeType[CODEC_ID_DVB_SUBTITLE] = "application/dvb";
	codecToMimeType[CODEC_ID_TEXT] = "text/plain";
	codecToMimeType[CODEC_ID_XSUB] = "application/x-subrip";
	codecToMimeType[CODEC_ID_SSA] = "application/ssa";
	codecToMimeType[CODEC_ID_MOV_TEXT] = "text/plain";
	codecToMimeType[CODEC_ID_HDMV_PGS_SUBTITLE] = "application/pgs";
	codecToMimeType[CODEC_ID_DVB_TELETEXT] = "application/dvb";

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
const std::map<CodecID, std::string> CODEC_TO_MIMETYPE =
		initialiseCodecToMimeType();

/**
 * The map that is used to look up mime types from libav container names.
 */
const std::map<std::string, std::string> NAME_TO_MIMETYPE =
		initialiseNameToMimeType();

/**
 * Extract the media details of the given type from the libav AVFormatContext struct
 * using the provided get details function.
 *
 * @param videoFile - the av format context that contains the audio details to extract.
 * @param mediaType - the type of media stream that should have it's codec inspected.
 * @param detailsCallback - the function to use to get the correct details out of the codec.
 *
 * @return a vector containing all the extracted details.
 */
template<typename T> std::vector<T> extractDetails(
		const AVFormatContext *videoFile, AVMediaType mediatype,
		std::tr1::function<T(const AVStream&)> detailsCallback) {

	// Simplify the use of the transcode api.
	using namespace transcode;

	std::vector<T> details;

	AVStream *stream;
	AVMediaType codecType;

	// Iterate through the streams searching for any audio streams.
	for (int i = 0; i < videoFile->nb_streams; i++) {

		stream = videoFile->streams[i];
		codecType = videoFile->streams[i]->codec->codec_type;

		// If we find an audio stream use its details to construct an AudioDetail
		// struct.
		if (codecType == mediatype) {

			details.push_back(detailsCallback(*stream));
		}
	}

	return details;
}

/**
 * Initialise the FFMPEG api and create an AVFormatContext to be used to expose
 * information about the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return an initialised AVFormatContext struct for the provided media file.
 */
static AVFormatContext* initialiseFFMPEG(const std::string& filePath) {

	// Initialise the ffmpeg libav library so we can use it to inspect
	// the media file.
	avcodec_init();
	avcodec_register_all();
	av_register_all();

	// Open the media file. This will populate the AVFormatContext
	// with all the information about this media file.
	AVFormatContext *videoFile;
	if (av_open_input_file(&videoFile, filePath.c_str(), NULL, 0, NULL) < 0) {

		throw transcode::MediaUtilsException("Could not open the media file");
	}

	return videoFile;
}

/**
 * Check to make sure we are working with an actual file.
 *
 * @param fp - the path to the file that will have it's details inspected.
 *
 * @return a boost filesystem path class related to the provided file.
 */
static boost::filesystem::path checkFile(const std::string& fp) {

	// Small optimisation, if no filename is provided then don't bother doing
	// anything.
	if (0 == fp.compare("")) {

		throw transcode::MediaUtilsException("No file name provided.");
	}

	// Create a path class, this provides easy access to a file paths
	// different elements.
	boost::filesystem::path filePath(fp);

	// Another small optimisation, use the boost filesystem exists function to
	// check if the provided file does not exist and if it doesn't again don't
	// bother with any further processing.
	if (!boost::filesystem::exists(filePath)) {

		throw transcode::MediaUtilsException("File " + fp + " does not exist.");
	}

	return filePath;
}

/**
 * Extract the language from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant language.
 *
 * @return the name of the streams language if one has been set.
 */
static std::string extractLanguage(const AVStream& stream) {

	// Try and find the language value by inspecting the streams metadata.
	// Supposedly this is paired to the key "language".
	AVMetadataTag *metadata = av_metadata_get(stream.metadata, "language", NULL,
			0);

	return metadata == NULL ? "" : metadata->value;
}

/**
 * Extract the subtitle detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant subtitle details.
 *
 * @return a subtitle details struct populated from values within the provided codec.
 */
static transcode::SubtitleDetail extractSubtitleDetail(const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	std::string language = extractLanguage(stream);

	transcode::SubtitleDetail subtitleDetail(
			transcode::utils::get(CODEC_TO_MIMETYPE, codec->codec_id),
			language);

	return subtitleDetail;
}

/**
 * Extract the audio detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant audio details.
 *
 * @return an audio details struct populated from values within the provided codec.
 */
static transcode::AudioDetail extractAudioDetail(const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	std::string language = extractLanguage(stream);

	transcode::AudioDetail audioDetail(
			transcode::utils::get(CODEC_TO_MIMETYPE, codec->codec_id), language,
			codec->bit_rate, codec->channels);

	return audioDetail;
}

/**
 * Extract the video detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant video details.
 *
 * @return a video details struct populated from values within the provided codec.
 */
static transcode::VideoDetail extractVideoDetail(const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	transcode::VideoDetail videoDetail(
			transcode::utils::get(CODEC_TO_MIMETYPE, codec->codec_id),
			codec->width, codec->height, codec->frame_number);

	return videoDetail;
}

/**
 * Find the details of the requested type from the provided file using the
 * provided media type.
 *
 * @param fp - the path to the file that will have it's details inspected.
 * @param mediatype - the type of details that should be found.
 * @param detailsCallback - the function to use to get the correct details.
 *
 * @return a vector containing all the extracted details.
 */
template<typename T> std::vector<T> findDetails(std::string fp,
		AVMediaType mediatype,
		std::tr1::function<T(const AVStream&)> detailsCallback) {

	checkFile(fp);

	AVFormatContext *videoFile = initialiseFFMPEG(fp);

	return extractDetails<T>(videoFile, mediatype, detailsCallback);
}

/**
 * Build a ContainerDetail struct out of the details in the provided
 * AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that contains the details to be
 * 			used to populate the ContainerDetail.
 *
 * @return a populated ContainerDetail struct.
 */
static transcode::ContainerDetail buildContainerDetail(
		const AVFormatContext *videoFile) {

	// Find the mime type for the media files container.
	std::string containerMimeType = transcode::utils::get(NAME_TO_MIMETYPE,
			std::string(videoFile->iformat->name));
	// Find the description for the media files container.
	std::string description = videoFile->iformat->long_name;

	using namespace transcode;

	// Extract all the audio codecs.
	std::vector<SubtitleDetail> subtitleDetails =
			extractDetails<SubtitleDetail>(videoFile, AVMEDIA_TYPE_SUBTITLE,
					extractSubtitleDetail);
	// Extract all the audio codecs.
	std::vector<AudioDetail> audioDetails = extractDetails<AudioDetail>(
			videoFile, AVMEDIA_TYPE_AUDIO, extractAudioDetail);
	// Extract all the video codecs.
	std::vector<VideoDetail> videoDetails = extractDetails<VideoDetail>(
			videoFile, AVMEDIA_TYPE_VIDEO, extractVideoDetail);

	return ContainerDetail(containerMimeType, description, subtitleDetails,
			audioDetails, videoDetails);
}

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
static void closeCodecs(AVFormatContext *videoFile) {

	AVCodecContext *codec;

	for (int i = 0; i < videoFile->nb_streams; i++) {

		codec = videoFile->streams[i]->codec;

		// Try and free the codec. If it fails throw an exception.
		if (0 > avcodec_close(codec)) {

			std::stringstream errorMessage;
			errorMessage << "Could not close codec " << i << "of type "
					<< transcode::utils::get(CODEC_TO_MIMETYPE, codec->codec_id)
					<< ".";

			throw transcode::MediaUtilsException(errorMessage.str());
		}
	}
}

namespace transcode {

std::vector<SubtitleDetail> findSubtitleDetails(const std::string& fp) {

	return findDetails<SubtitleDetail>(fp, AVMEDIA_TYPE_SUBTITLE,
			extractSubtitleDetail);
}

std::vector<AudioDetail> findAudioDetails(const std::string& fp) {

	return findDetails<AudioDetail>(fp, AVMEDIA_TYPE_AUDIO, extractAudioDetail);
}

std::vector<VideoDetail> findVideoDetails(const std::string& fp) {

	return findDetails<VideoDetail>(fp, AVMEDIA_TYPE_VIDEO, extractVideoDetail);
}

ContainerDetail findContainerDetails(const std::string& fp) {

	checkFile(fp);

	AVFormatContext *videoFile = initialiseFFMPEG(fp);

	return buildContainerDetail(videoFile);
}

MediaFileDetail findMediaFileDetails(const std::string& fp) {

	// Check to make sure we are working with a real file.
	// If not throw a MediaUtilsException.
	boost::filesystem::path filePath = checkFile(fp);

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
