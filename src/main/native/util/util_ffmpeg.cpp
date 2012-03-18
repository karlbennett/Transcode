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

#include <util/util_ffmpeg.hpp>
#include <metadata.hpp>
#include <util/util_standard.hpp>

#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <tr1/functional>

/**
 * @file util_ffmpeg.cpp
 *
 * The implementation for the util_ffmpeg.hpp functions.
 * The majority of the implementation is contained within the
 * {@see transcode::utils::FfmpegSingleton} class.
 */

/**
 * Helper namespace, all helper functions and classes are found here.
 */
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
    codecToMimeType[CODEC_ID_FLV1] = "video/x-svq";

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
    nameToMimeType["flv"] = "video/x-flv";

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
 * Find the audio or video mediatype for the provided codec id.
 *
 * @param codecId - the codec id that will be used to find the mimetype.
 * @return the mimetype string related to the provided codec id.
 * @throws transcode::utils::FFMPEGException if a mimetype cannot be found.
 */
static std::string findAVMimeType(const CodecID& codecId)
        throw (transcode::util::FFMPEGException) {

    std::string mimeType = transcode::util::get(helper::CODEC_TO_MIMETYPE,
            codecId);

    if (std::string() == mimeType) {

        std::stringstream errorMessage;

        errorMessage << "Could not find codec mimetype for codec ID: "
                << codecId << std::endl;

        throw transcode::util::FFMPEGException(errorMessage.str());
    }

    return mimeType;
}

/**
 * Find the container mediatype for the provided codec id.
 *
 * @param name - the simple container name to be used to find the container
 *          mimetype.
 * @return the mimetype string related to the provided container name.
 * @throws transcode::utils::FFMPEGException if a mimetype cannot be found.
 */
static std::string findContainerMimeType(const std::string& name) {

    std::string mimeType = transcode::util::get(helper::NAME_TO_MIMETYPE,
            name);

    if (std::string() == mimeType) {

        throw transcode::util::FFMPEGException(
                "Could not find container mimetype for name: " + name);
    }

    return mimeType;
}

} /* namespace helper */

/**
 * Callback namespace, all callback functions are found here.
 */
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
 * Extract the subtitle meta data from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant subtitle meta data.
 *
 * @return a subtitle meta data struct populated from values within the provided codec.
 */
static transcode::SubtitleMetaData extractSubtitleMetaData(
        const AVStream& stream) {

    AVCodecContext *codec = stream.codec;

    std::string language = extractLanguage(stream);

    transcode::SubtitleMetaData subtitleDetail(
            helper::findAVMimeType(codec->codec_id), language);

    return subtitleDetail;
}

/**
 * Extract the audio meta data from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant audio meta data.
 *
 * @return an audio meta data struct populated from values within the provided codec.
 */
static transcode::AudioMetaData extractAudioMetaData(const AVStream& stream) {

    AVCodecContext *codec = stream.codec;

    std::string language = extractLanguage(stream);

    transcode::AudioMetaData audioDetail(
            helper::findAVMimeType(codec->codec_id), language, codec->bit_rate,
            codec->channels);

    return audioDetail;
}

/**
 * Extract the video meta data from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant video meta data.
 *
 * @return a video meta data struct populated from values within the provided codec.
 */
static transcode::VideoMetaData extractVideoMetaData(const AVStream& stream) {

    AVCodecContext *codec = stream.codec;

    transcode::VideoMetaData videoDetail(
            helper::findAVMimeType(codec->codec_id), codec->width,
            codec->height, codec->frame_number);

    return videoDetail;
}

} /* namespace callback */

/**
 * Transcode namespace, all the top level transcode functions and classes are in
 * this namespace. So this namespace holds officially public API.
 */
namespace transcode {

/**
 * Util namespace, all the utility functions and classes are found within this
 * namespace. You might find something useful here but hopefully everything in the
 * {@see transcode} namespace should provide what you need so you shouldn't have to
 * look in here.
 */
namespace util {

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
    FfmpegSingleton();

    FfmpegSingleton(FfmpegSingleton const&); // Should not be implemented.

    void operator=(FfmpegSingleton const&); // Should not be implemented.

public:
    static const FfmpegSingleton& getInstance() {
        static FfmpegSingleton instance;
        return instance;
    }

    std::string ffmpegErrorMessage(int errorCode) const;

    AVFormatContext* retrieveAVFormatContext(const std::string& filePath) const;

    std::vector<SubtitleMetaData> extractSubtitleDetails(
            const AVFormatContext *videoFile) const;

    std::vector<AudioMetaData> extractAudioDetails(
            const AVFormatContext *videoFile) const;

    std::vector<VideoMetaData> extractVideoDetails(
            const AVFormatContext *videoFile) const;

    ContainerMetaData buildContainerDetail(
            const AVFormatContext *videoFile) const;

    void closeCodecs(AVFormatContext *videoFile) const throw (FFMPEGException);
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
 * Extract the media meta data of the given type from the libav AVFormatContext struct
 * using the provided get meta data function.
 *
 * @param videoFile - the av format context that contains the audio meta data to
 *          extract.
 * @param mediaType - the type of media stream that should have it's codec inspected.
 * @param metadataCallback - the function to use to get the correct meta data out of
 *          the codec.
 *
 * @return a vector containing all the extracted meta data.
 */
template<typename T> std::vector<T> extractMetaData(
        const AVFormatContext *videoFile, AVMediaType mediatype,
        std::tr1::function<T(const AVStream&)> metadataCallback) {

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

            details.push_back(metadataCallback(*stream));
        }
    }

    return details;
}

FfmpegSingleton::FfmpegSingleton() {

    // Initialise the ffmpeg libav library so we can use it to inspect
    // the media file.
    avcodec_init();
    avcodec_register_all();
    av_register_all();

    // Set the log level to quiet to stop any warnings.
    av_log_set_level(AV_LOG_QUIET);
}

std::string FfmpegSingleton::ffmpegErrorMessage(int errorCode) const {

    size_t bufferSize = 1024;

    char buffer[bufferSize];

    int err = av_strerror(errorCode, buffer, bufferSize);

    return err == 0 ? buffer : UNKNOWN;
}

AVFormatContext* FfmpegSingleton::retrieveAVFormatContext(
        const std::string& filePath) const {

    // Open the media file. This will populate the AVFormatContext
    // with all the information about this media file.
    AVFormatContext *videoFile;

    int errorCode = av_open_input_file(&videoFile, filePath.c_str(), NULL, 0,
            NULL);

    // If an error code was returned throw an appropriate exception.
    if (0 > errorCode) {
        throw FFMPEGException(errorCode);
    }

    // Have a double try at extracting the info from the media file because
    // sometimes opening it is not enough.
    errorCode = av_find_stream_info(videoFile);

    if (0 > errorCode) {
        throw FFMPEGException(errorCode);
    }

    return videoFile;
}

std::vector<SubtitleMetaData> FfmpegSingleton::extractSubtitleDetails(
        const AVFormatContext *videoFile) const {

    return extractMetaData<SubtitleMetaData>(videoFile, AVMEDIA_TYPE_SUBTITLE,
            callback::extractSubtitleMetaData);
}

std::vector<AudioMetaData> FfmpegSingleton::extractAudioDetails(
        const AVFormatContext *videoFile) const {

    return extractMetaData<AudioMetaData>(videoFile, AVMEDIA_TYPE_AUDIO,
            callback::extractAudioMetaData);
}

std::vector<VideoMetaData> FfmpegSingleton::extractVideoDetails(
        const AVFormatContext *videoFile) const {

    return extractMetaData<VideoMetaData>(videoFile, AVMEDIA_TYPE_VIDEO,
            callback::extractVideoMetaData);
}

ContainerMetaData FfmpegSingleton::buildContainerDetail(
        const AVFormatContext *videoFile) const {

    checkFormatContext(videoFile);

    AVInputFormat *inputFormat = videoFile->iformat;
    if (NULL == inputFormat) {
        throw FFMPEGException("The inputFormat is NULL.");
    }

    // Find the mime type for the media files container.
    std::string containerMimeType = helper::findContainerMimeType(
            std::string(inputFormat->name));
    // Find the description for the media files container.
    std::string description = inputFormat->long_name;

    // Extract all the audio codecs.
    std::vector<SubtitleMetaData> subtitleDetails = extractSubtitleDetails(
            videoFile);
    // Extract all the audio codecs.
    std::vector<AudioMetaData> audioDetails = extractAudioDetails(videoFile);
    // Extract all the video codecs.
    std::vector<VideoMetaData> videoDetails = extractVideoDetails(videoFile);

    return ContainerMetaData(containerMimeType, description, subtitleDetails,
            audioDetails, videoDetails);
}

void FfmpegSingleton::closeCodecs(AVFormatContext *videoFile) const
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

AVFormatContext* retrieveAVFormatContext(const std::string& filePath)
        throw (FFMPEGException) {

    return FfmpegSingleton::getInstance().retrieveAVFormatContext(filePath);
}

std::vector<SubtitleMetaData> extractSubtitleDetails(
        const AVFormatContext *videoFile) throw (FFMPEGException) {

    return FfmpegSingleton::getInstance().extractSubtitleDetails(videoFile);
}

std::vector<AudioMetaData> extractAudioDetails(const AVFormatContext *videoFile)
        throw (FFMPEGException) {

    return FfmpegSingleton::getInstance().extractAudioDetails(videoFile);
}

std::vector<VideoMetaData> extractVideoDetails(const AVFormatContext *videoFile)
        throw (FFMPEGException) {

    return FfmpegSingleton::getInstance().extractVideoDetails(videoFile);
}

ContainerMetaData buildContainerDetail(const AVFormatContext *videoFile)
        throw (FFMPEGException) {

    return FfmpegSingleton::getInstance().buildContainerDetail(videoFile);
}

void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException) {

    FfmpegSingleton::getInstance().closeCodecs(videoFile);
}

} /* namespace util */
} /* namespace transcode */

