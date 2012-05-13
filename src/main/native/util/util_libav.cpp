/*
 * util_libav.cpp
 *
 *  Created on: 9 May 2012
 *      Author: karl
 */

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/error.h"
}

#include <util/util_libav.hpp>
#include <error.hpp>
#include <sstream>

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

class LibavSingleton
{

private:
    /**
     * The Libav initialisation is carried out here, it should only ever happen
     * once.
     */
    LibavSingleton();

    LibavSingleton(LibavSingleton const&); // Should not be implemented.

    void operator=(LibavSingleton const&); // Should not be implemented.

public:
    static const LibavSingleton& getInstance() {
        static LibavSingleton instance;
        return instance;
    }

    std::string errorMessage(const int& errorCode) const;

    AVFormatContext* openFormatContext(const std::string& fileName) const;

    void closeFormatContext(AVFormatContext *formatContext) const;

    AVPacket* readNextPacket(AVFormatContext *videoFile) const;

    AVMediaType findStreamType(const AVStream *stream) const;

    AVMediaType findCodecType(const AVCodecContext *codecContext) const;

    AVMediaType findPacketType(const AVFormatContext *videoFile,
            const AVPacket *packet) const;

    AVCodecContext* openCodecContext(AVCodecContext *codecContext) const;

    std::vector<AVFrame*> decodeAudioPacket(const AVCodecContext *codecContext,
            const AVPacket *packet) const;

    AVFrame decodeVideoPacket(const AVCodecContext *codecContext,
            const AVPacket *packet) const;
};

LibavSingleton::LibavSingleton() {

    // Initialise the libav library so we can use it to inspect
    // the media file.
    avcodec_register_all();
    av_register_all();

    // Set the log level to error to stop any warnings.
    av_log_set_level(AV_LOG_ERROR);
}

std::string LibavSingleton::errorMessage(const int& errorCode) const {

    size_t bufferSize = 1024;

    char buffer[bufferSize];

    int err = av_strerror(errorCode, buffer, bufferSize);

    return err == 0 ? buffer : UNKNOWN;
}

AVFormatContext* LibavSingleton::openFormatContext(
        const std::string& filePath) const {

    // Open the media file. This will populate the AVFormatContext
    // with all the information about this media file.
    AVFormatContext *formatContext = NULL;

    int errorCode = avformat_open_input(&formatContext, filePath.c_str(), NULL,
            NULL);

    // If the media file could not be opened successfully throw an exception
    // containing the errors codes message.
    if (0 != errorCode) {

        throw IOException(errorMessage(errorCode));
    }

    // Have a double try at extracting the info from the media file because
    // sometimes opening it is not enough.
    errorCode = avformat_find_stream_info(formatContext, NULL);

    // If all is successful return the newly opened format context.
    if (0 == errorCode) return formatContext;

    // Other wise fail.
    throw IOException(errorMessage(errorCode));
}

void LibavSingleton::closeFormatContext(AVFormatContext *formatContext) const {

    AVStream *stream = NULL;

    // Close all the codec contexts within the format context to make sure they
    // are cleaned up.
    for (int i = 0; i < formatContext->nb_streams; i++) {

        stream = formatContext->streams[i];

        if (NULL != stream) avcodec_close(stream->codec);
    }

    avformat_close_input(&formatContext);
}

AVPacket* LibavSingleton::readNextPacket(AVFormatContext *videoFile) const {

    AVPacket *packet = new AVPacket();

    av_init_packet(packet);

    int error = av_read_frame(videoFile, packet);

    // If error equals 0 then we have a valid packet so return it.
    if (0 == error) return packet;

    // If we have reached the end of the file return NULL;
    if (AVERROR_EOF == error) return NULL;

    // Double check to see if we have reached the end of the file.
    // Because the AVERROR_EOF error code isn't always returned.
    AVIOContext *ioContext = videoFile->pb;
    if (NULL != ioContext && ioContext->eof_reached) return NULL;

    // Otherwise throw an exception with the error message.
    throw PacketReadException(errorMessage(error));
}

AVMediaType LibavSingleton::findStreamType(const AVStream *stream) const {

    if (NULL == stream) {

        throw IllegalArgumentException(
                "The supplied stream for findStreamType(AVStream) cannot be null.");
    }

    return findCodecType(stream->codec);
}

AVMediaType LibavSingleton::findCodecType(
        const AVCodecContext *codecContext) const {

    if (NULL == codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context for findCodecType(AVCodecContext) cannot be null.");
    }

    return codecContext->codec_type;
}

AVMediaType LibavSingleton::findPacketType(const AVFormatContext *videoFile,
        const AVPacket *packet) const {

    int packetIndex = packet->stream_index;
    int streamNumber = videoFile->nb_streams;

    // Check to make sure the packet index isn't out of bounds.
    if (streamNumber < packetIndex) {

        std::stringstream errorMessage;

        errorMessage
        << "Packet index greater than stream number. Packet index: "
                << packetIndex << " Stream number: "
                << streamNumber << " File name: "
                << videoFile->filename << std::endl;

        throw IllegalStateException(errorMessage.str());
    }

    return findStreamType(videoFile->streams[packetIndex]);
}

AVCodecContext* LibavSingleton::openCodecContext(AVCodecContext *codecContext) const {

    if (NULL == codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context for openCodecContext(AVCodecContext) cannot be null.");
    }

    // Find the codec for the provided codec context.
    AVCodec *codec = avcodec_find_decoder(codecContext->codec_id);

    if (NULL == codec) throw CodecException("Could not find a supported codec.");

    int codecOpenResult = avcodec_open2(codecContext, codec, NULL);

    if (0 > codecOpenResult) {
        std::stringstream errorMessage;

        errorMessage << "Could not open " << codec->long_name
                << " codec. Error: "
                << codecOpenResult << std::endl;

        throw FFMPEGException(errorMessage.str());
    }

    return codecContext;
}

} /* namespace util */
} /* namespace transcode */
