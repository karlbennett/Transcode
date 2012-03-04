/*
 * FFMPEGUtils.cpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/error.h"
}

#include <utils/FFMPEGUtils.hpp>
#include <utils/MediaUtils.hpp>
#include <utils/StandardUtils.hpp>
#include <utils/FileUtils.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>

std::map<CodecID, std::string> transcode::utils::initialiseCodecToMimeType() {

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

std::map<std::string, std::string> transcode::utils::initialiseNameToMimeType() {

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

std::string transcode::utils::ffmpegErrorMessage(int errorCode) {

	size_t bufferSize = 1024;

	char buffer[bufferSize];

	int err = av_strerror(errorCode, buffer, bufferSize);

	return err == 0 ? buffer : "Unknown";
}

AVFormatContext* transcode::utils::initialiseFFMPEG(const std::string& filePath)
		throw (transcode::utils::FFMPEGException) {

	// Initialise the ffmpeg libav library so we can use it to inspect
	// the media file.
	avcodec_init();
	avcodec_register_all();
	av_register_all();

	// Open the media file. This will populate the AVFormatContext
	// with all the information about this media file.
	AVFormatContext *videoFile;

	int errorCode = av_open_input_file(&videoFile, filePath.c_str(), NULL, 0,
			NULL);

	// If an error code was returned throw an appropriate exception.
	if (0 > errorCode) {

		throw FFMPEGException(errorCode);
	}

	return videoFile;
}

std::string transcode::utils::extractLanguage(const AVStream& stream) {

	// Try and find the language value by inspecting the streams metadata.
	// Supposedly this is paired to the key "language".
	AVMetadataTag *metadata = av_metadata_get(stream.metadata, "language", NULL,
			0);

	return metadata == NULL ? "" : metadata->value;
}

transcode::SubtitleDetail transcode::utils::extractSubtitleDetail(
		const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	std::string language = extractLanguage(stream);

	transcode::SubtitleDetail subtitleDetail(
			get(CODEC_TO_MIMETYPE, codec->codec_id), language);

	return subtitleDetail;
}

transcode::AudioDetail transcode::utils::extractAudioDetail(
		const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	std::string language = extractLanguage(stream);

	transcode::AudioDetail audioDetail(get(CODEC_TO_MIMETYPE, codec->codec_id),
			language, codec->bit_rate, codec->channels);

	return audioDetail;
}

transcode::VideoDetail transcode::utils::extractVideoDetail(
		const AVStream& stream) {

	AVCodecContext *codec = stream.codec;

	transcode::VideoDetail videoDetail(get(CODEC_TO_MIMETYPE, codec->codec_id),
			codec->width, codec->height, codec->frame_number);

	return videoDetail;
}

transcode::ContainerDetail transcode::utils::buildContainerDetail(
		const AVFormatContext *videoFile) {

	// Find the mime type for the media files container.
	std::string containerMimeType = get(NAME_TO_MIMETYPE,
			std::string(videoFile->iformat->name));
	// Find the description for the media files container.
	std::string description = videoFile->iformat->long_name;

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

void transcode::utils::closeCodecs(AVFormatContext *videoFile)
		throw (transcode::utils::FFMPEGException) {

	AVCodecContext *codec;

	for (int i = 0; i < videoFile->nb_streams; i++) {

		codec = videoFile->streams[i]->codec;

		int errorCode = avcodec_close(codec);

		// Try and free the codec. If it fails throw an exception.
		if (0 > errorCode) {

			throw FFMPEGException(errorCode);
		}
	}
}
