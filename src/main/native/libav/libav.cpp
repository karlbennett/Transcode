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

#include <libav/libav.hpp>
#include <error.hpp>
#include <sstream>

#include <iostream>

#include <tr1/functional>

using namespace std;


/**
 * @file libav.cpp
 *
 * The implementation of the libav.chpp functions.
 */


namespace transcode {
namespace libav {

/**
 * The buffer size of a decoded packet, this is hopefully big enough for any packet.
 */
const int BUFFER_SIZE = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;

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

    string errorMessage(const int& errorCode) const;

    AVFormatContext* openFormatContext(const string& fileName) const;

    void closeFormatContext(AVFormatContext **formatContext) const;

    AVPacket* readNextPacket(AVFormatContext *formatContext) const;

    AVMediaType findStreamType(const AVStream *stream) const;

    AVMediaType findCodecType(const AVCodecContext *codecContext) const;

    AVMediaType findPacketType(const AVFormatContext *formatContext,
            const AVPacket *packet) const;

    AVCodecContext* openCodecContext(AVCodecContext *codecContext) const;

    void closeCodecContext(AVCodecContext **codecContext) const;

    vector<AVFrame*> decodeAudioPacket(AVCodecContext *codecContext,
            const AVPacket *packet) const;

    AVPacket* encodeAudioFrame(AVCodecContext *codecContext,
            const AVFrame *frame) const;

    AVFrame* decodeVideoPacket(AVCodecContext *codecContext,
            const AVPacket *packet) const;

    AVPacket* encodeVideoFrame(AVCodecContext *codecContext,
            const AVFrame *frame) const;
};

LibavSingleton::LibavSingleton() {

    // Initialise the libav library so we can use it to inspect
    // the media file.
    avcodec_register_all();
    av_register_all();

    // Set the log level to fatal to stop any warnings.
    av_log_set_level(AV_LOG_INFO);
}

string LibavSingleton::errorMessage(const int& errorCode) const {

    size_t bufferSize = 1024;

    char buffer[bufferSize];

    // Make sure that the buffer is null terminated so that it
    // represents a valid C++ string.
    memset(buffer, 0, bufferSize);

    int err = av_strerror(errorCode, buffer, bufferSize);

    return err == 0 ? string(buffer) : UNKNOWN;
}

AVFormatContext* LibavSingleton::openFormatContext(
        const string& filePath) const {

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
    if (0 <= errorCode) return formatContext;

    // Other wise fail.
    throw IOException(errorMessage(errorCode));
}

void LibavSingleton::closeFormatContext(AVFormatContext **formatContext) const {

    if (NULL == (*formatContext)) {

        throw IllegalArgumentException("Cannot close a NULL AVFormatContext");
    }

    if (0 >= (*formatContext)->nb_streams) {

        throw IllegalStateException(
                "The AVFormatContext does not contain any valid streams");
    }

    AVStream *stream = NULL;

    // Close all the codec contexts within the format context to make sure they
    // are cleaned up.
    for (int i = 0; i < (*formatContext)->nb_streams; i++) {

        stream = (*formatContext)->streams[i];

        if (NULL != stream) avcodec_close(stream->codec);
    }

    avformat_close_input(formatContext);
}

AVPacket* LibavSingleton::readNextPacket(AVFormatContext *formatContext) const {

    if (NULL == formatContext) {

        throw IllegalArgumentException(
                "Cannot read a packet from a NULL AVFormatContext");
    }

    if (0 >= formatContext->nb_streams) {

        throw IllegalStateException(
                "There are no streams within the AVFormatContext to read a packet from.");
    }

    AVPacket *packet = new AVPacket();

    av_init_packet(packet);

    int error = av_read_frame(formatContext, packet);

    // If error equals 0 then we have a valid packet so return it.
    if (0 == error) return packet;

    // If we have reached the end of the file return NULL;
    if (AVERROR_EOF == error) return NULL;

    // Double check to see if we have reached the end of the file.
    // Because the AVERROR_EOF error code isn't always returned.
    AVIOContext *ioContext = formatContext->pb;
    if (NULL != ioContext && ioContext->eof_reached) return NULL;

    // Otherwise throw an exception with the error message.
    throw PacketReadException(errorMessage(error));
}

AVMediaType LibavSingleton::findStreamType(const AVStream *stream) const {

    if (NULL == stream) {

        throw IllegalArgumentException(
                "The supplied stream for findStreamType(AVStream*) cannot be null.");
    }

    return findCodecType(stream->codec);
}

AVMediaType LibavSingleton::findCodecType(
        const AVCodecContext *codecContext) const {

    if (NULL == codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context for findCodecType(AVCodecContext*) cannot be null.");
    }

    AVMediaType type = codecContext->codec_type;

    // If the type value is within the bounds of the
    // AVMediaType enum then return it.
    // This check is unfortunately tightly coupled to
    // libav version, but I think the benefit of the check
    // out ways the burden of keeping the values in sync
    // with the compiled libav version.
    if (-1 <= type && 5 >= type) return type;

    stringstream errorMessage;

    errorMessage << "Invalid AVMediaType value: "
            << type << endl;

    throw IllegalStateException(errorMessage.str());
}

AVMediaType LibavSingleton::findPacketType(const AVFormatContext *formatContext,
        const AVPacket *packet) const {

    if (NULL == formatContext) {

        throw IllegalArgumentException(
                "The supplied format context for findPacketType(AVFormatContext*,AVPacket*) cannot be null.");
    }

    if (NULL == packet) {

        throw IllegalArgumentException(
                "The supplied packet for findPacketType(AVFormatContext*,AVPacket*) cannot be null.");
    }

    int packetIndex = packet->stream_index;
    int streamNumber = formatContext->nb_streams;

    // Check to make sure the packet index isn't out of bounds.
    if (0 <= packetIndex && streamNumber < packetIndex) {

        stringstream errorMessage;

        errorMessage
        << "Packet index greater than stream number. Packet index: "
                << packetIndex << " Stream number: "
                << streamNumber << " File name: "
                << formatContext->filename << endl;

        throw IllegalStateException(errorMessage.str());
    }

    return findStreamType(formatContext->streams[packetIndex]);
}

AVCodecContext* LibavSingleton::openCodecContext(
        AVCodecContext *codecContext) const {

    if (NULL == codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context for openCodecContext(AVCodecContext*) cannot be null.");
    }

    // Find the codec for the provided codec context.
    AVCodec *codec = avcodec_find_decoder(codecContext->codec_id);

    if (NULL == codec) throw CodecException("Could not find a supported codec.");

    int codecOpenResult = avcodec_open2(codecContext, codec, NULL);

    if (0 == codecOpenResult) return codecContext;

    throw CodecException(errorMessage(codecOpenResult));
}

void LibavSingleton::closeCodecContext(AVCodecContext **codecContext) const {

    if (NULL == codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context pointer for closeCodecContext(AVCodecContext**) cannot be null.");
    }

    if (NULL == *codecContext) {

        throw IllegalArgumentException(
                "The supplied codec context for openCodecContext(AVCodecContext**) cannot be null.");
    }

    int codecCloseResult = avcodec_close(*codecContext);

    if (0 == codecCloseResult) return;

    throw CodecException(errorMessage(codecCloseResult));
}

/**
 * Template that wraps the supplied callback with default error checking and makes an
 * internal copy of the supplied packet that is then passed into the callback.
 *
 * The callback for this function should then decode the copied packet into it's
 * desired container.
 *
 * @param codecContext - the codec context that will be used to decode the callback.
 * @param packet - the packet that is to be decoded.
 * @param decodeCallback - the callback that will carry out the actual decode of the packet.
 * @param T - the return type of the supplied callback.
 * @return the result of the callback.
 */
template<typename T> T decodePacketTemplate(AVCodecContext *codecContext, const AVPacket *packet,
        std::tr1::function<T(AVCodecContext *codecContext, AVPacket *packet)> decodeCallback) {

    if (NULL == codecContext) {

        throw IllegalArgumentException("The codec context for decoding cannot be null.");
    }

    if (NULL == packet) {

        throw IllegalArgumentException("The packet for decoding cannot be null.");
    }

    // A copy of the supplied packet that will be used during the decoding so that we
    // don't mutate the supplied packet. If we mutated the supplied packet it would no
    // longer be able to be correctly deleted.
    AVPacket packetCopy;
    // Create a new large buffer array to hold the audio packet.
    uint8_t buffer[BUFFER_SIZE];
    // Set all the values within to 0 so that there is no question about the data end.
    memset(buffer, 0, BUFFER_SIZE);
    // Copy the packet data into the new buffer.
    memcpy(buffer, packet->data, packet->size);
    // Set the copy packets data and size.
    packetCopy.data = buffer;
    packetCopy.size = packet->size;

    return decodeCallback(codecContext, &packetCopy);
}

static AVPacket* encodeFrameWrapper(AVCodecContext *codecContext, const AVFrame *frame,
        std::tr1::function<int(AVCodecContext *codecContext, AVPacket *packet,
                const AVFrame *frame, int *packetEncoded)> encodeCallback) {

    if (NULL == codecContext) {

        throw IllegalArgumentException("The codec context for encoding cannot be null.");
    }

    if (NULL == frame) {

        throw IllegalArgumentException("The frame for encoding cannot be null.");
    }

    AVPacket *packet = new AVPacket();

    packet->size = 0;
    packet->data = NULL;

    av_init_packet(packet);

    int packetEncoded = 0;

    int result = encodeCallback(codecContext, packet, frame, &packetEncoded);

    if (AVERROR_INVALIDDATA == result) {

        throw InvalidPacketDataException(errorMessage(result));
    }

    if (0 > result) {

        throw PacketDecodeException(errorMessage(result));
    }

    if (0 != packetEncoded) return packet;

    return NULL;
}

/**
 * A callback function that decodes an audio packet. This should be supplied
 * to the <code>decodePacketTemplate</code> template.
 *
 * @param codecContext - the codec context that will be used to decode the callback.
 * @param packet - the packet that is to be decoded.
 * @return the decoded packet as a vector of AVFrames.
 */
static vector<AVFrame*> decodeAudioPacketCallback(AVCodecContext *codecContext,
        AVPacket *packet) {

    if (AVMEDIA_TYPE_AUDIO != findCodecType(codecContext)) {

        throw IllegalArgumentException(
                "The supplied codec context for decoding audio must have media type AVMEDIA_TYPE_AUDIO.");
    }

    // This vector will contain all the audio frames decoded from the supplied packet.
    vector<AVFrame*> frames;

    // The frame pointer that will hold each new frame before it is placed in the vector.
    AVFrame *decodedFrame = NULL;

    // This will be set to 1 if a frame has successfully been decoded with the
    // avcodec_decode_audio4() function.
    int frameDecoded = 0;

    int bytesDecoded = 0; // The number of bytes that were decoded in each iteration.

    while (0 < packet->size) {
        // Create a new frame to contain the decoded data if one is required.
        if (NULL == decodedFrame) decodedFrame = avcodec_alloc_frame();

        // Decode the packet and store it in the new frame.
        // Also record how many bytes were decoded because it might not have been all
        // of them.
        bytesDecoded = avcodec_decode_audio4(codecContext, decodedFrame,
                &frameDecoded,
                packet);

        // If there is an invalid data error throw a more specific exception.
        if (AVERROR_INVALIDDATA == bytesDecoded) {

            throw InvalidPacketDataException(errorMessage(bytesDecoded));
        }

        if (0 > bytesDecoded) {

            throw PacketDecodeException(errorMessage(bytesDecoded));
        }

        // If a frame was successfully decoded add it the vector to be returned and
        // set the pointer to null to indicate we need a new frame allocated.
        if (0 != frameDecoded) {

            frames.push_back(decodedFrame);

            decodedFrame = NULL;

        } else {
            // If we haven't successfully decoded a frame reset the decode frame
            // values to make sure it's ready for another decode attempt.
            avcodec_get_frame_defaults(decodedFrame);
        }

        // Push the data pointer down the byte array passed the last byte that we
        // decoded.
        packet->data += bytesDecoded;
        // Reduce the relative size of the data to the amount that is yet to be
        // decoded.
        packet->size -= bytesDecoded;
    }

    return frames;
}

static int encodeAudioFrameCallback(AVCodecContext *codecContext, AVPacket *packet,
                const AVFrame *frame, int *packetEncoded) {

    if (AVMEDIA_TYPE_AUDIO != findCodecType(codecContext)) {

        throw IllegalArgumentException(
                "The supplied codec context for encoding audio must have media type AVMEDIA_TYPE_AUDIO.");
    }

    return avcodec_encode_audio2(codecContext, packet, frame, packetEncoded);
}

/**
 * A callback function that decodes a video packet. This should be supplied
 * to the <code>decodePacketTemplate</code> template.
 *
 * @param codecContext - the codec context that will be used to decode the callback.
 * @param packet - the packet that is to be decoded.
 * @return the decoded packet as an AVFrame.
 */
static AVFrame* decodeVideoPacketCallBack(AVCodecContext *codecContext,
        AVPacket *packet) {

    if (AVMEDIA_TYPE_VIDEO != findCodecType(codecContext)) {

        throw IllegalArgumentException(
                "The supplied codec context for decodeVideoPacket(AVCodecContext*,AVPacket*) must have media type AVMEDIA_TYPE_VIDEO.");
    }

    AVFrame *decodedFrame = avcodec_alloc_frame();

    int bytesDecoded = 0;

    int frameDecoded = 0;

    bytesDecoded = avcodec_decode_video2(codecContext, decodedFrame,
            &frameDecoded,
            packet);

    if (AVERROR_INVALIDDATA == bytesDecoded) {

        throw InvalidPacketDataException(errorMessage(bytesDecoded));
    }

    if (0 > bytesDecoded) {

        throw PacketDecodeException(errorMessage(bytesDecoded));
    }

    if (0 != frameDecoded) return decodedFrame;

    return NULL;
}

static int encodeVideoFrameCallback(AVCodecContext *codecContext, AVPacket *packet,
                const AVFrame *frame, int *packetEncoded) {

    if (AVMEDIA_TYPE_VIDEO != findCodecType(codecContext)) {

        throw IllegalArgumentException(
                "The supplied codec context for encoding video must have media type AVMEDIA_TYPE_VIDEO.");
    }

    return avcodec_encode_video2(codecContext, packet, frame, packetEncoded);
}

vector<AVFrame*> LibavSingleton::decodeAudioPacket(
        AVCodecContext *codecContext,
        const AVPacket *packet) const {

    return decodePacketTemplate<vector<AVFrame*> >(codecContext, packet,
            decodeAudioPacketCallback);
}

AVPacket* LibavSingleton::encodeAudioFrame(AVCodecContext *codecContext,
        const AVFrame *frame) const {

    return encodeFrameWrapper(codecContext, frame, encodeAudioFrameCallback);
}

AVFrame* LibavSingleton::decodeVideoPacket(AVCodecContext *codecContext,
        const AVPacket *packet) const {

    return decodePacketTemplate<AVFrame*>(codecContext, packet,
            decodeVideoPacketCallBack);
}

AVPacket* LibavSingleton::encodeVideoFrame(AVCodecContext *codecContext,
        const AVFrame *frame) const {

    return encodeFrameWrapper(codecContext, frame, encodeVideoFrameCallback);
}


string errorMessage(const int& errorCode) {

    return LibavSingleton::getInstance().errorMessage(errorCode);
}

AVFormatContext* openFormatContext(const string& fileName) {

    return LibavSingleton::getInstance().openFormatContext(fileName);
}

void closeFormatContext(AVFormatContext **formatContext) {

    return LibavSingleton::getInstance().closeFormatContext(formatContext);
}

AVPacket* readNextPacket(AVFormatContext *formatContext) {

    return LibavSingleton::getInstance().readNextPacket(formatContext);
}

AVMediaType findStreamType(const AVStream *stream) {

    return LibavSingleton::getInstance().findStreamType(stream);
}

AVMediaType findCodecType(const AVCodecContext *codecContext) {

    return LibavSingleton::getInstance().findCodecType(codecContext);
}

AVMediaType findPacketType(const AVFormatContext *formatContext,
        const AVPacket *packet) {

    return LibavSingleton::getInstance().findPacketType(formatContext, packet);
}

AVCodecContext* openCodecContext(AVCodecContext *codecContext) {

    return LibavSingleton::getInstance().openCodecContext(codecContext);
}

void closeCodecContext(AVCodecContext **codecContext) {

    LibavSingleton::getInstance().closeCodecContext(codecContext);
}

vector<AVFrame*> decodeAudioPacket(AVCodecContext *codecContext,
        const AVPacket *packet) {

    return LibavSingleton::getInstance().decodeAudioPacket(codecContext, packet);
}

AVPacket* encodeAudioFrame(AVCodecContext *codecContext,
        const AVFrame *frame) {

    return LibavSingleton::getInstance().encodeAudioFrame(codecContext, frame);
}

AVFrame* decodeVideoPacket(AVCodecContext *codecContext,
        const AVPacket *packet) {

    return LibavSingleton::getInstance().decodeVideoPacket(codecContext, packet);
}

AVPacket* encodeVideoFrame(AVCodecContext *codecContext,
        const AVFrame *frame) {

    return LibavSingleton::getInstance().encodeVideoFrame(codecContext, frame);
}


} /* namespace util */
} /* namespace transcode */
