/*
 * ffmpeg.cpp
 *
 *  Created on: 10 Mar 2012
 *      Author: karl
 */

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/error.h"
}

#include <utils/ffmpeg.hpp>
#include <utils/MediaUtils.hpp>
#include <utils/StandardUtils.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <tr1/functional>

namespace helper {

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
}

namespace callback {

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
            transcode::utils::get(helper::CODEC_TO_MIMETYPE, codec->codec_id),
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
            transcode::utils::get(helper::CODEC_TO_MIMETYPE, codec->codec_id),
            language, codec->bit_rate, codec->channels);

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
            transcode::utils::get(helper::CODEC_TO_MIMETYPE, codec->codec_id),
            codec->width, codec->height, codec->frame_number);

    return videoDetail;
}

}

namespace transcode {
namespace utils {

/**
 * A singleton class that provides all the implementations for the FFMPEG
 * functions. This class has been created because FFMPEG requires
 * initialisation, so if all function calls are invoked through this
 * singleton it is guaranteed that the initialisation has been carried out
 * and will only ever be carried out once.
 *
 * The structure of this singleton was taken from here:
 * http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 */
class FfmpegSingleton
{
private:
    /**
     * The FFMPEG initialisation is carried out here, it should only ever happen
     * once.
     */
    FfmpegSingleton() {

        // Initialise the ffmpeg libav library so we can use it to inspect
        // the media file.
        avcodec_init();
        avcodec_register_all();
        av_register_all();
    }

    FfmpegSingleton(FfmpegSingleton const&); // Should not be implemented.

    void operator=(FfmpegSingleton const&); // Should not be implemented.

public:
    static FfmpegSingleton& getInstance()
    {
        static FfmpegSingleton instance;
        return instance;
    }

    std::string ffmpegErrorMessage(int errorCode);

    AVFormatContext* retrieveAVFormatContext(const std::string& filePath);

    std::vector<SubtitleDetail> extractSubtitleDetails(
            const AVFormatContext *videoFile);

    std::vector<AudioDetail> extractAudioDetails(
            const AVFormatContext *videoFile);

    std::vector<VideoDetail> extractVideoDetails(
            const AVFormatContext *videoFile);

    ContainerDetail buildContainerDetail(const AVFormatContext *videoFile);

    void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException);
};

/**
 * Check to make sure that the AVFormatContext is in a usable state.
 *
 * @param formatContext - the AVFormatContext to check.
 *
 * @throws an FFMPEGException if the AVFormatContext cannot be used.
 */
static void checkFormatContext(const AVFormatContext *formatContext) {

    if (NULL == formatContext) {
        throw FFMPEGException("The AVFormatContext cannot be NULL.");
    }
}

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

    checkFormatContext(videoFile);

    std::vector<T> details;

    AVStream *stream;
    AVCodecContext *codecContext;
    AVMediaType codecType;

    // Iterate through the streams searching for any audio streams.
    for (int i = 0; i < videoFile->nb_streams; i++) {

        stream = videoFile->streams[i];
        if (NULL == stream) {
            throw FFMPEGException("A media stream was null.");
        }

        codecContext = videoFile->streams[i]->codec;
        if (NULL == codecContext) {
            throw FFMPEGException("A codec was null.");
        }

        codecType = codecContext->codec_type;

        // If we find a stream that matches the provided type use its details to
        // construct a struct of the requested type.
        if (codecType == mediatype) {

            details.push_back(detailsCallback(*stream));
        }
    }

    return details;
}

std::string FfmpegSingleton::ffmpegErrorMessage(int errorCode) {

    size_t bufferSize = 1024;

    char buffer[bufferSize];

    int err = av_strerror(errorCode, buffer, bufferSize);

    return err == 0 ? buffer : "Unknown";
}

AVFormatContext* FfmpegSingleton::retrieveAVFormatContext(
        const std::string& filePath) {

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

std::vector<SubtitleDetail> FfmpegSingleton::extractSubtitleDetails(
        const AVFormatContext *videoFile) {

    return extractDetails<SubtitleDetail>(videoFile, AVMEDIA_TYPE_SUBTITLE,
            callback::extractSubtitleDetail);
}

std::vector<AudioDetail> FfmpegSingleton::extractAudioDetails(
        const AVFormatContext *videoFile) {

    return extractDetails<AudioDetail>(videoFile, AVMEDIA_TYPE_AUDIO,
            callback::extractAudioDetail);
}

std::vector<VideoDetail> FfmpegSingleton::extractVideoDetails(
        const AVFormatContext *videoFile) {

    return extractDetails<VideoDetail>(videoFile, AVMEDIA_TYPE_VIDEO,
            callback::extractVideoDetail);
}

ContainerDetail FfmpegSingleton::buildContainerDetail(
        const AVFormatContext *videoFile) {

    checkFormatContext(videoFile);

    AVInputFormat *inputFormat = videoFile->iformat;
    if (NULL == inputFormat) {
        throw FFMPEGException("The inputFormat is NULL.");
    }

    // Find the mime type for the media files container.
    std::string containerMimeType = get(helper::NAME_TO_MIMETYPE,
            std::string(inputFormat->name));
    // Find the description for the media files container.
    std::string description = inputFormat->long_name;

    // Extract all the audio codecs.
    std::vector<SubtitleDetail> subtitleDetails = extractSubtitleDetails(
            videoFile);
    // Extract all the audio codecs.
    std::vector<AudioDetail> audioDetails = extractAudioDetails(videoFile);
    // Extract all the video codecs.
    std::vector<VideoDetail> videoDetails = extractVideoDetails(videoFile);

    return ContainerDetail(containerMimeType, description, subtitleDetails,
            audioDetails, videoDetails);
}

void FfmpegSingleton::closeCodecs(AVFormatContext *videoFile)
        throw (FFMPEGException) {

    checkFormatContext(videoFile);

    AVCodecContext *codec;

    AVStream *stream;

    for (int i = 0; i < videoFile->nb_streams; i++) {

        stream = videoFile->streams[i];
        if (NULL == stream) {
            throw FFMPEGException("A media stream was null.");
        }

        codec = stream->codec;

        // Try and free the codec. If it fails throw an exception.
        int errorCode = avcodec_close(codec);
        if (0 > errorCode) {
            throw FFMPEGException(errorCode);
        }
    }
}

/* FfmpegSingleton End */

/**
 * Below is the implementation of the public API functions.
 */

std::string ffmpegErrorMessage(int errorCode) {

    return FfmpegSingleton::getInstance().ffmpegErrorMessage(errorCode);
}

AVFormatContext* retrieveAVFormatContext(const std::string& filePath) {

    return FfmpegSingleton::getInstance().retrieveAVFormatContext(filePath);
}

std::vector<SubtitleDetail> extractSubtitleDetails(
        const AVFormatContext *videoFile) {

    return FfmpegSingleton::getInstance().extractSubtitleDetails(videoFile);
}

std::vector<AudioDetail> extractAudioDetails(const AVFormatContext *videoFile) {

    return FfmpegSingleton::getInstance().extractAudioDetails(videoFile);
}

std::vector<VideoDetail> extractVideoDetails(const AVFormatContext *videoFile) {

    return FfmpegSingleton::getInstance().extractVideoDetails(videoFile);
}

ContainerDetail buildContainerDetail(const AVFormatContext *videoFile) {

    return FfmpegSingleton::getInstance().buildContainerDetail(videoFile);
}

void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException) {

    FfmpegSingleton::getInstance().closeCodecs(videoFile);
}

} /* namespace utils */
} /* namespace transcode */

