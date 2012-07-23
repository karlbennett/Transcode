/*
 * test_fixtures.hpp
 *
 *  Created on: 20 May 2012
 *      Author: karl
 */

#ifndef __TEST_FIXTURES_HPP__
#define __TEST_FIXTURES_HPP__

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/audioconvert.h"
}

#include <util_test.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <tr1/functional>


namespace test {

std::string error(int error) {

    size_t bufferSize = 1024;

    char buffer[bufferSize];

    // Make sure that the buffer is null terminated so that it
    // represents a valid C++ string.
    memset(buffer, 0, bufferSize);

    int err = av_strerror(error, buffer, bufferSize);

    return std::string(buffer);
}

struct LibAvRegisterable {

    LibAvRegisterable() {

        avcodec_register_all();
        av_register_all();

        av_log_set_level(AV_LOG_FATAL);
    }
};

/** BASE FIXTURES **/

struct FormatContextFixture: public LibAvRegisterable {

    FormatContextFixture() : LibAvRegisterable(), formatContext(NULL) {}

    /**
     * Instantiate a new <code>FormatContextFixture</code> with the supplied
     * format context, also check the format context to make sure it contains
     * some media streams.
     *
     * @param fc - the format context to use within this fixture.
     */
    FormatContextFixture(AVFormatContext *fc) : LibAvRegisterable(), formatContext(fc) {

        checkStreams();
    }

    /**
     * Instantiate a new <code>FormatContextFixture</code> with a format
     * context that is populated with the data from the media file with the
     * supplied file name. Also check the format context to make sure it contains
     * some media streams.
     *
     * @param fileName - the path to the file that will be used to populate
     *      the format context within this fixture.
     */
    FormatContextFixture(std::string fileName) : LibAvRegisterable(), formatContext(NULL) {

        avformat_open_input(&formatContext, fileName.c_str(), NULL,
                            NULL);

        avformat_find_stream_info(formatContext, NULL);

        checkStreams();
    }

    virtual ~FormatContextFixture() {
        if (NULL != formatContext) avformat_close_input(&formatContext);
    }

    AVFormatContext *formatContext;

    void checkStreams() {

        if (0 >= formatContext->nb_streams) {

            throw "The fixture format context does not contain any streams.";
        }
    }

    static void regesterLibAv() {


    }
};

struct StreamFixture: public FormatContextFixture {

    StreamFixture(std::string fileName) : FormatContextFixture(fileName),
            streams(formatContext->streams), streamNumber(formatContext->nb_streams) {}

    virtual ~StreamFixture() {}

    AVStream **streams;
    int streamNumber;
};

struct CodecContextFixture: public StreamFixture {

    /**
     * Instantiate a new <code>CodecContextFixture</code> and populate the
     * codecs array from by extracting the codec contexts from the supplied
     * media streams. The size of the stream array must also be supplied so
     * the fixture know how many codecs to populate.
     *
     * @param streams   - an array of pointers to AVStreams that contain the
     *      codec contexts for this fixture.
     * @param size      - the size of the streams array supplied to this
     *      fixture.
     */
    CodecContextFixture(std::string fileName): StreamFixture(fileName), codecNumber(streamNumber) {

        decodeCodecs = new AVCodecContext*[codecNumber];

        for (int i = 0; i < codecNumber; i++) {

            decodeCodecs[i] = streams[i]->codec;
        }
    }

    virtual ~CodecContextFixture() {

        for (int i = 0; i < codecNumber; i++) {

            avcodec_close(decodeCodecs[i]);
        }
    }

    AVCodecContext **decodeCodecs;
    int codecNumber;
};

struct OpenedCodecContextFixture: public CodecContextFixture {

    /**
     * Instantiate a new <code>OpenedCodecContextFixture</code> and populate
     * the codecs array from by extracting the codec contexts from the
     * supplied media streams. The codec contexts within the array are also
     * opened. The size of the stream array must also be supplied so the
     * fixture know how many codecs to populate.
     *
     * @param streams   - an array of pointers to AVStreams that contain the
     *      codec contexts for this fixture.
     * @param size      - the size of the streams array supplied to this
     *      fixture.
     */
    OpenedCodecContextFixture(std::string fileName) :
            CodecContextFixture(fileName) {

        encodeCodecs = new AVCodecContext*[codecNumber];
        // Initialise all the slots to NULL so that we know which pointers
        // actually contain an AVCodecContext.
        memset(encodeCodecs, 0, codecNumber);

        AVCodecContext *decodeCodecContext = NULL;
        AVCodecContext *encodeCodecContext = NULL;
        AVCodec *decoder = NULL;
        AVCodec *encoder = NULL;

        int codecOpenResult = 0;

        for (int i = 0; i < codecNumber; i++) {

            encodeCodecs[i] = NULL;

            decodeCodecContext = decodeCodecs[i];

            // Can only handle audio and video codecs at this time.
            if (AVMEDIA_TYPE_AUDIO == decodeCodecContext->codec_type
                    || AVMEDIA_TYPE_VIDEO == decodeCodecContext->codec_type) {

                std::string mediaType;

                if (AVMEDIA_TYPE_AUDIO == decodeCodecContext->codec_type) {

                    mediaType = "audio";

                } else {

                    mediaType = "video";
                }

                decoder = avcodec_find_decoder(decodeCodecContext->codec_id);

                if (NULL == decoder) {

                    throw "Fixture " + mediaType
                            + " decode codec for codec context could not be found.";
                }

                if (CODEC_ID_AC3 == decodeCodecContext->codec_id
                        && AV_SAMPLE_FMT_S16 == decodeCodecContext->sample_fmt) {
                    encoder = avcodec_find_encoder_by_name("ac3_fixed");
                } else if (CODEC_ID_AAC == decodeCodecContext->codec_id
                        && AV_SAMPLE_FMT_S16 == decodeCodecContext->sample_fmt) {
                    encoder = avcodec_find_encoder_by_name("libfaac");
                } else {
                    encoder = avcodec_find_encoder(decodeCodecContext->codec_id);
                }

                if (NULL == encoder) {

                    throw "Fixture " + mediaType
                            + " encode codec for codec context could not be found.";
                }

                encodeCodecs[i] = avcodec_alloc_context3(encoder);
                avcodec_copy_context(encodeCodecs[i], decodeCodecContext);
                encodeCodecContext = encodeCodecs[i];

                if (CODEC_ID_H264 == encodeCodecContext->codec_id) {
                    encodeCodecContext->me_range = 16;
                    encodeCodecContext->max_qdiff = 4;
                    encodeCodecContext->qmin = 10;
                    encodeCodecContext->qmax = 51;
                    encodeCodecContext->qcompress = 0.6;
                }

                if (CODEC_ID_FLV1 == encodeCodecContext->codec_id) {
                    encodeCodecContext->time_base.num = 1;
                    encodeCodecContext->time_base.den = 1000;
                }

                if (CODEC_ID_VORBIS == encodeCodecContext->codec_id) {
                    encodeCodecContext->channels = 2;
                    encodeCodecContext->channel_layout =
                            av_get_default_channel_layout(encodeCodecContext->channels);
                }

                if (NULL == encodeCodecContext || NULL == encodeCodecs[i]) {

                    throw "Fixture " + mediaType
                            + " encode codec context could not be allocated.";
                }

                codecOpenResult = avcodec_open2(decodeCodecContext, decoder, NULL);

                if (0 != codecOpenResult) {

                    throw "Fixture " + mediaType
                            + " decode codec context could not be opened. Error: "
                            + error(codecOpenResult);
                }

                codecOpenResult = avcodec_open2(encodeCodecContext, encoder, NULL);

                if (0 != codecOpenResult) {

                    throw "Fixture " + mediaType
                            + " encode codec context could not be opened. Error: "
                            + error(codecOpenResult);
                }
            }
        }
    }

    virtual ~OpenedCodecContextFixture() {

        for (int i = 0; i < codecNumber; i++) {

            if (NULL != encodeCodecs[i]) avcodec_close(encodeCodecs[i]);
        }
    }

    AVCodecContext **encodeCodecs;
};

struct PacketFixture: public OpenedCodecContextFixture {

    /**
     * Instantiate a new <code>PacketFixture</code> and populate it's
     * packet attribute by reading the first packet from the supplied
     * format context.
     *
     * @param fc - the format context to read the packet from.
     */
    PacketFixture(std::string fileName): OpenedCodecContextFixture(fileName), packet(NULL), type(AVMEDIA_TYPE_UNKNOWN) {

        packet = readPacket();
    }

    /**
     * Instantiate a new <code>PacketFixture</code> and populate it's
     * packet attribute by reading the first packet of the supplied
     * type from the supplied format context.
     *
     * @param fc - the format context to read the typed packet from.
     */
    PacketFixture(std::string fileName, AVMediaType tp):
        OpenedCodecContextFixture(fileName), packet(NULL), type(tp) {

        packet = readPacket(type);
    }

    virtual ~PacketFixture() {

        av_free_packet(packet);
    }

    AVPacket *packet;
    AVMediaType type;

    /**
     * Check the type of the supplied packet to make sure that it matches
     * the supplied type.
     *
     * @param fc        - the format context that contains the streams
     *      that will be used to check the packets type.
     * @param packet    - the packet that will be checked.
     * @param type      - the required type of the packet.
     * @return true if the packet is of the supplied type otherwise false.
     */
    bool checkPacketType(AVPacket *packet, AVMediaType type) {

        int streamIndex = packet->stream_index;

        if (formatContext->nb_streams <= streamIndex) {

            throw "Fixture packet stream index is out of bounds.";
        }

        AVStream *stream = formatContext->streams[streamIndex];

        AVCodecContext *codec = stream->codec;

        if (NULL == codec) {

            throw "Fixture stream codec is null.";
        }

        return type == codec->codec_type;
    }

    /**
     * Read the next packet from the supplied format context.
     *
     * @param fc - the format context to read the next packet from.
     * @return the next packet if one is available.
     */
    AVPacket* readPacket() {

        AVPacket *packet = new AVPacket();

        av_init_packet(packet);

        int result = av_read_frame(formatContext, packet);

        if (0 == result) return packet;

        throw "Failed to read AVPacket in test fixture. Error: " + error(result);
    }

    /**
     * Read the next packet of the supplied type from the supplied
     * format context.
     *
     * Note: This method will read then discard packets that don't
     * match the required type.
     *
     * @param fc    - the format context to read the next packet from.
     * @param type  - the ype of packet that should be returned.
     * @return the next packet if one is available.
     */
    AVPacket* readPacket(AVMediaType type) {

        AVPacket *packet = NULL;

        do {

            av_free_packet(packet);

            packet = readPacket();

        } while (!checkPacketType(packet, type));

        return packet;
    }

    /**
     * Keep trying to decode a video frame till success or the end of the file is reached.
     * This is done because the first few packets of most video codecs cannot be decoded
     * into a valid frame.
     *
     * @param formatContext - the format context to decode the video frame from.
     * @param packet - the initial packet to try and decode, if this packet fails each
     *      consecutive packet will be tried until a frame is successfully decoded. This
     *      packet is also used to select the codec that will be used for the decoding.
     */
    template<typename T> T retryPacketDecodeWrapper(
            std::tr1::function<T(AVCodecContext *codecContext, const AVPacket *packet)> decodeCallback) {

        AVFrame *frame = NULL;

        AVCodecContext *codec = decodeCodecs[packet->stream_index];

        // Keep reading packets till we successfully decode a video frame or there are no more
        // packets left to read from the media file.
        while (NULL != packet) {

            frame = decodeCallback(codec, packet);

            // If we get a frame return it.
            if (NULL != frame) return frame;

            av_free_packet(packet);

            packet = readPacket(type);
        }

        return frame;
    }
};

struct AudioPacketFixture: public PacketFixture {

    AudioPacketFixture(std::string fileName) : PacketFixture(fileName, AVMEDIA_TYPE_AUDIO) {}

    virtual ~AudioPacketFixture() {}
};

struct VideoPacketFixture: public PacketFixture {

    VideoPacketFixture(std::string fileName) : PacketFixture(fileName, AVMEDIA_TYPE_VIDEO) {}

    virtual ~VideoPacketFixture() {}
};

struct FrameFixture: public PacketFixture {

    FrameFixture(std::string fileName, AVMediaType tp) :
            PacketFixture(fileName, tp), type(tp), frames(std::vector<AVFrame*>()) {}

    virtual ~FrameFixture() {

        for (int i = 0; i < frames.size(); i++) {

            av_free(frames[i]);
        }
    }

    static const int BUFFER_SIZE = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
    AVMediaType type;
    std::vector<AVFrame*> frames;

    AVPacket* retryEncodeFrameWrapper(
            std::tr1::function<AVPacket*(AVCodecContext *codecContext, AVFrame *frame)> encodeCallback) {

        AVPacket *encodedPacket = NULL;

        while (NULL == encodedPacket) {

            for (int i = 0; i < frames.size(); i++) {

                encodedPacket = encodeCallback(encodeCodecs[packet->stream_index], frames[i]);

                if (NULL != encodedPacket) return encodedPacket;
            }

            frames = retryDecodePacket(type);
        }

        return encodedPacket;
    }

    std::vector<AVFrame*> retryDecodePacket(AVMediaType type) {

        std::vector<AVFrame*> frames = decodePacket(packet);

        // If we haven't been able successfully decode an audio frame on the
        // first go keep trying till we do.
        while (1 > frames.size()) {

            av_free_packet(packet);

            packet = readPacket(type);

            frames = decodePacket(packet);
        }

        return frames;
    }

    std::vector<AVFrame*> decodePacket(AVPacket *packet) {

        AVPacket packetCopy;

        uint8_t buffer[BUFFER_SIZE];

        memset(buffer, 0, BUFFER_SIZE);

        memcpy(buffer, packet->data, packet->size);

        packetCopy.data = buffer;
        packetCopy.size = packet->size;
        packetCopy.side_data_elems = packet->side_data_elems;
        packetCopy.side_data = packet->side_data;

        AVCodecContext *codec = decodeCodecs[packet->stream_index];

        return decodeStrategy(codec, &packetCopy);
    }

    virtual std::vector<AVFrame*> decodeStrategy(AVCodecContext *codecContext,
        AVPacket *packet) = 0;
};

struct AudioFrameFixture: public FrameFixture {

    AudioFrameFixture(std::string fileName) :
            FrameFixture(fileName, AVMEDIA_TYPE_AUDIO) {

        frames = retryDecodePacket(type);
    }

    virtual ~AudioFrameFixture() {}

    std::vector<AVFrame*> decodeStrategy(AVCodecContext *codecContext,
            AVPacket *packet) {

        std::vector<AVFrame*> frames;

        AVFrame *decodedFrame = NULL;

        int frameDecoded = 0;

        int bytesDecoded = 0; // The number of bytes that were decoded in each iteration.

        while (0 < packet->size) {

            if (NULL == decodedFrame) decodedFrame = avcodec_alloc_frame();

            bytesDecoded = avcodec_decode_audio4(codecContext, decodedFrame,
                    &frameDecoded,
                    packet);

            if (AVERROR_INVALIDDATA == bytesDecoded) {

                throw "Invalid data for audio packet decode in test fixture. Error: "
                        + error(bytesDecoded);
            }

            if (0 > bytesDecoded) {

                throw "Error decoding audio packet in test fixture. Error: "
                        + error(bytesDecoded);
            }

            if (0 != frameDecoded) {

                frames.push_back(decodedFrame);

                decodedFrame = NULL;

            } else {

                avcodec_get_frame_defaults(decodedFrame);
            }

            packet->data += bytesDecoded;
            packet->size -= bytesDecoded;
        }

        return frames;
    }
};

struct VideoFrameFixture: public FrameFixture {

    VideoFrameFixture(std::string fileName) :
            FrameFixture(fileName, AVMEDIA_TYPE_VIDEO) {

        frames = retryDecodePacket(type);
    }

    virtual ~VideoFrameFixture() {}

    std::vector<AVFrame*> decodeStrategy(AVCodecContext *codecContext,
            AVPacket *packet) {

        std::vector<AVFrame*> frames;

        AVFrame *decodedFrame = avcodec_alloc_frame();

        int bytesDecoded = 0;

        int frameDecoded = 0;

        bytesDecoded = avcodec_decode_video2(codecContext, decodedFrame,
                &frameDecoded,
                packet);

        if (AVERROR_INVALIDDATA == bytesDecoded) {

            throw "Invalid data for video packet decode in test fixture. Error: "
                    + error(bytesDecoded);
        }

        if (0 > bytesDecoded) {

            throw "Error decoding video packet in test fixture. Error: "
                    + error(bytesDecoded);
        }

        if (0 != frameDecoded) frames.push_back(decodedFrame);

        return frames;
    }
};

/** FORMAT CONTEXT FIXTURES **/

struct AVIFormatContextFixture: public FormatContextFixture {

    AVIFormatContextFixture() : FormatContextFixture(VIDEO_AVI) {}
};

struct MKVFormatContextFixture: public FormatContextFixture {

    MKVFormatContextFixture() : FormatContextFixture(VIDEO_MKV) {}
};

struct OGVFormatContextFixture: public FormatContextFixture {

    OGVFormatContextFixture() : FormatContextFixture(VIDEO_OGV) {}
};

struct MP4FormatContextFixture: public FormatContextFixture {

    MP4FormatContextFixture() : FormatContextFixture(VIDEO_MP4) {}
};

struct FLVFormatContextFixture: public FormatContextFixture {

    FLVFormatContextFixture() : FormatContextFixture(VIDEO_FLV) {}
};

struct TextFormatContextFixture: public FormatContextFixture {

    TextFormatContextFixture() : FormatContextFixture(TEXT_FILE) {}
};


/** STREAM FIXTURES **/

struct AVIStreamFixture: public StreamFixture {

    AVIStreamFixture() : StreamFixture(VIDEO_AVI) {}
};

struct MKVStreamFixture: public StreamFixture {

    MKVStreamFixture() : StreamFixture(VIDEO_MKV) {}
};

struct OGVStreamFixture: public StreamFixture {

    OGVStreamFixture() : StreamFixture(VIDEO_OGV) {}
};

struct MP4StreamFixture: public StreamFixture {

    MP4StreamFixture() : StreamFixture(VIDEO_MP4) {}
};

struct FLVStreamFixture: public StreamFixture {

    FLVStreamFixture() : StreamFixture(VIDEO_FLV) {}
};


/** CODEC CONTEXT FIXTURES **/

struct AVICodecContextFixture: public CodecContextFixture {

    AVICodecContextFixture() : CodecContextFixture(VIDEO_AVI) {}
};

struct AVIOpenedCodecContextFixture: public OpenedCodecContextFixture {

    AVIOpenedCodecContextFixture() : OpenedCodecContextFixture(VIDEO_AVI) {}
};

struct MKVCodecContextFixture: public CodecContextFixture {

    MKVCodecContextFixture() : CodecContextFixture(VIDEO_MKV) {}
};

struct MKVOpenedCodecContextFixture: public OpenedCodecContextFixture {

    MKVOpenedCodecContextFixture() : OpenedCodecContextFixture(VIDEO_MKV) {}
};

struct OGVCodecContextFixture: public CodecContextFixture {

    OGVCodecContextFixture() : CodecContextFixture(VIDEO_OGV) {}
};

struct OGVOpenedCodecContextFixture: public OpenedCodecContextFixture {

    OGVOpenedCodecContextFixture() : OpenedCodecContextFixture(VIDEO_OGV) {}
};

struct MP4CodecContextFixture: public CodecContextFixture {

    MP4CodecContextFixture() : CodecContextFixture(VIDEO_MP4) {}
};

struct MP4OpenedCodecContextFixture: public OpenedCodecContextFixture {

    MP4OpenedCodecContextFixture() : OpenedCodecContextFixture(VIDEO_MP4) {}
};

struct FLVCodecContextFixture: public CodecContextFixture {

    FLVCodecContextFixture() : CodecContextFixture(VIDEO_FLV) {}
};

struct FLVOpenedCodecContextFixture: public OpenedCodecContextFixture {

    FLVOpenedCodecContextFixture() : OpenedCodecContextFixture(VIDEO_FLV) {}
};


/** PACKET CONTEXT FIXTURES **/

struct AVIPacketFixture: public PacketFixture {

    AVIPacketFixture() : PacketFixture(VIDEO_AVI) {}
};

struct MKVPacketFixture: public PacketFixture {

    MKVPacketFixture() : PacketFixture(VIDEO_MKV) {}
};

struct OGVPacketFixture: public PacketFixture {

    OGVPacketFixture() : PacketFixture(VIDEO_OGV) {}
};

struct MP4PacketFixture: public PacketFixture {

    MP4PacketFixture() : PacketFixture(VIDEO_MP4) {}
};

struct FLVPacketFixture: public PacketFixture {

    FLVPacketFixture() : PacketFixture(VIDEO_FLV) {}
};


/** AUDIO PACKET CONTEXT FIXTURES **/

struct AVIAudioPacketFixture: public AudioPacketFixture {

    AVIAudioPacketFixture() : AudioPacketFixture(VIDEO_AVI) {}
};

struct MKVAudioPacketFixture: public AudioPacketFixture {

    MKVAudioPacketFixture() : AudioPacketFixture(VIDEO_MKV) {}
};

struct OGVAudioPacketFixture: public AudioPacketFixture {

    OGVAudioPacketFixture() : AudioPacketFixture(VIDEO_OGV) {}
};

struct MP4AudioPacketFixture: public AudioPacketFixture {

    MP4AudioPacketFixture() : AudioPacketFixture(VIDEO_MP4) {}
};

struct FLVAudioPacketFixture: public AudioPacketFixture {

    FLVAudioPacketFixture() : AudioPacketFixture(VIDEO_FLV) {}
};


/** VIDEO PACKET CONTEXT FIXTURES **/

struct AVIVideoPacketFixture: public VideoPacketFixture {

    AVIVideoPacketFixture() : VideoPacketFixture(VIDEO_AVI) {}
};

struct MKVVideoPacketFixture: public VideoPacketFixture {

    MKVVideoPacketFixture() : VideoPacketFixture(VIDEO_MKV) {}
};

struct OGVVideoPacketFixture: public VideoPacketFixture {

    OGVVideoPacketFixture() : VideoPacketFixture(VIDEO_OGV) {}
};

struct MP4VideoPacketFixture: public VideoPacketFixture {

    MP4VideoPacketFixture() : VideoPacketFixture(VIDEO_MP4) {}
};

struct FLVVideoPacketFixture: public VideoPacketFixture {

    FLVVideoPacketFixture() : VideoPacketFixture(VIDEO_FLV) {}
};


/** AUDIO FRAME CONTEXT FIXTURES **/

struct AVIAudioFrameFixture: public AudioFrameFixture {

    AVIAudioFrameFixture() : AudioFrameFixture(VIDEO_AVI) {}
};

struct MKVAudioFrameFixture: public AudioFrameFixture {

    MKVAudioFrameFixture() : AudioFrameFixture(VIDEO_MKV) {}
};

struct OGVAudioFrameFixture: public AudioFrameFixture {

    OGVAudioFrameFixture() : AudioFrameFixture(VIDEO_OGV) {}
};

struct MP4AudioFrameFixture: public AudioFrameFixture {

    MP4AudioFrameFixture() : AudioFrameFixture(VIDEO_MP4) {}
};

struct FLVAudioFrameFixture: public AudioFrameFixture {

    FLVAudioFrameFixture() : AudioFrameFixture(VIDEO_FLV) {}
};


/** VIDEO FRAME CONTEXT FIXTURES **/

struct AVIVideoFrameFixture: public VideoFrameFixture {

    AVIVideoFrameFixture() : VideoFrameFixture(VIDEO_AVI) {}
};

struct MKVVideoFrameFixture: public VideoFrameFixture {

    MKVVideoFrameFixture() : VideoFrameFixture(VIDEO_MKV) {}
};

struct OGVVideoFrameFixture: public VideoFrameFixture {

    OGVVideoFrameFixture() : VideoFrameFixture(VIDEO_OGV) {}
};

struct MP4VideoFrameFixture: public VideoFrameFixture {

    MP4VideoFrameFixture() : VideoFrameFixture(VIDEO_MP4) {}
};

struct FLVVideoFrameFixture: public VideoFrameFixture {

    FLVVideoFrameFixture() : VideoFrameFixture(VIDEO_FLV) {}
};

} /* namespace test */

#endif /* __TEST_FIXTURES_HPP__ */
