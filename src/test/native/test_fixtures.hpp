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

struct StreamFixture {

    StreamFixture(const AVFormatContext *fc) : streams(fc->streams),
            streamNumber(fc->nb_streams) {}

    virtual ~StreamFixture() {}

    AVStream **streams;
    int streamNumber;
};

struct CodecContextFixture {

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
    CodecContextFixture(AVStream **streams, const int& size): codecNumber(size) {

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
    OpenedCodecContextFixture(AVStream **streams, const int& size) :
            CodecContextFixture(streams, size) {

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

struct PacketFixture {

    /**
     * Instantiate a new <code>PacketFixture</code> and populate it's
     * packet attribute by reading the first packet from the supplied
     * format context.
     *
     * @param fc - the format context to read the packet from.
     */
    PacketFixture(AVFormatContext *fc): packet(NULL) {

        packet = readPacket(fc);
    }

    /**
     * Instantiate a new <code>PacketFixture</code> and populate it's
     * packet attribute by reading the first packet of the supplied
     * type from the supplied format context.
     *
     * @param fc - the format context to read the typed packet from.
     */
    PacketFixture(AVFormatContext *fc, AVMediaType type): packet(NULL) {

        packet = readPacket(fc, type);
    }

    virtual ~PacketFixture() {

        av_free_packet(packet);
    }

    AVPacket *packet;

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
    static bool checkPacketType(AVFormatContext *fc, AVPacket *packet,
            AVMediaType type) {

        int streamIndex = packet->stream_index;

        if (fc->nb_streams <= streamIndex) {

            throw "Fixture packet stream index is out of bounds.";
        }

        AVStream *stream = fc->streams[streamIndex];

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
    static AVPacket* readPacket(AVFormatContext *fc) {

        AVPacket *packet = new AVPacket();

        av_init_packet(packet);

        int result = av_read_frame(fc, packet);

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
    static AVPacket* readPacket(AVFormatContext *fc, AVMediaType type) {

        AVPacket *packet = NULL;

        do {

            av_free_packet(packet);

            packet = readPacket(fc);

        } while (!checkPacketType(fc, packet, type));

        return packet;
    }
};

struct AudioPacketFixture: public PacketFixture {

    AudioPacketFixture(AVFormatContext *fc) : PacketFixture(fc, AVMEDIA_TYPE_AUDIO) {}

    virtual ~AudioPacketFixture() {}
};

struct VideoPacketFixture: public PacketFixture {

    VideoPacketFixture(AVFormatContext *fc) : PacketFixture(fc, AVMEDIA_TYPE_VIDEO) {}

    virtual ~VideoPacketFixture() {}
};

struct FrameFixture: public PacketFixture {

    FrameFixture(AVFormatContext *fc, AVMediaType tp) :
            PacketFixture(fc, tp), type(tp), frames(std::vector<AVFrame*>()) {}

    virtual ~FrameFixture() {

        for (int i = 0; i < frames.size(); i++) {

            av_free(frames[i]);
        }
    }

    static const int BUFFER_SIZE = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
    AVMediaType type;
    std::vector<AVFrame*> frames;

    std::vector<AVFrame*> retryDecodePacket(AVFormatContext *fc, AVPacket *packet, AVMediaType type) {

        std::vector<AVFrame*> frames = decodePacket(fc, packet);

        // If we haven't been able successfully decode an audio frame on the
        // first go keep trying till we do.
        while (1 > frames.size()) {

            av_free_packet(packet);

            packet = readPacket(fc, type);

            frames = decodePacket(fc, packet);
        }

        return frames;
    }

    std::vector<AVFrame*> decodePacket(AVFormatContext *fc, AVPacket *packet) {

        AVPacket packetCopy;

        uint8_t buffer[BUFFER_SIZE];

        memset(buffer, 0, BUFFER_SIZE);

        memcpy(buffer, packet->data, packet->size);

        packetCopy.data = buffer;
        packetCopy.size = packet->size;

        AVCodecContext *codec = fc->streams[packet->stream_index]->codec;

        return decodeStrategy(codec, &packetCopy);
    }

    virtual std::vector<AVFrame*> decodeStrategy(AVCodecContext *codecContext,
        AVPacket *packet) = 0;
};

struct AudioFrameFixture: public FrameFixture {

    AudioFrameFixture(AVFormatContext *fc) :
            FrameFixture(fc, AVMEDIA_TYPE_AUDIO) {

        frames = retryDecodePacket(fc, packet, type);
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

    VideoFrameFixture(AVFormatContext *fc) :
            FrameFixture(fc, AVMEDIA_TYPE_VIDEO) {

        frames = retryDecodePacket(fc, packet, type);
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

struct AVIStreamFixture: public AVIFormatContextFixture, public StreamFixture {

    AVIStreamFixture() : AVIFormatContextFixture(), StreamFixture(formatContext) {}
};

struct MKVStreamFixture: public MKVFormatContextFixture, public StreamFixture {

    MKVStreamFixture() : MKVFormatContextFixture(), StreamFixture(formatContext) {}
};

struct OGVStreamFixture: public OGVFormatContextFixture, public StreamFixture {

    OGVStreamFixture() : OGVFormatContextFixture(), StreamFixture(formatContext) {}
};

struct MP4StreamFixture: public MP4FormatContextFixture, public StreamFixture {

    MP4StreamFixture() : MP4FormatContextFixture(), StreamFixture(formatContext) {}
};

struct FLVStreamFixture: public FLVFormatContextFixture, public StreamFixture {

    FLVStreamFixture() : FLVFormatContextFixture(), StreamFixture(formatContext) {}
};


/** CODEC CONTEXT FIXTURES **/

struct AVICodecContextFixture: public AVIStreamFixture, public CodecContextFixture {

    AVICodecContextFixture() : AVIStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct AVIOpenedCodecContextFixture: public AVIStreamFixture, public OpenedCodecContextFixture {

    AVIOpenedCodecContextFixture() : AVIStreamFixture(),
            OpenedCodecContextFixture(streams, streamNumber) {}
};

struct MKVCodecContextFixture: public MKVStreamFixture, public CodecContextFixture {

    MKVCodecContextFixture() : MKVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct MKVOpenedCodecContextFixture: public MKVStreamFixture, public OpenedCodecContextFixture {

    MKVOpenedCodecContextFixture() : MKVStreamFixture(),
            OpenedCodecContextFixture(streams, streamNumber) {}
};

struct OGVCodecContextFixture: public OGVStreamFixture, public CodecContextFixture {

    OGVCodecContextFixture() : OGVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct OGVOpenedCodecContextFixture: public OGVStreamFixture, public OpenedCodecContextFixture {

    OGVOpenedCodecContextFixture() : OGVStreamFixture(),
            OpenedCodecContextFixture(streams, streamNumber) {}
};

struct MP4CodecContextFixture: public MP4StreamFixture, public CodecContextFixture {

    MP4CodecContextFixture() : MP4StreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct MP4OpenedCodecContextFixture: public MP4StreamFixture, public OpenedCodecContextFixture {

    MP4OpenedCodecContextFixture() : MP4StreamFixture(),
            OpenedCodecContextFixture(streams, streamNumber) {}
};

struct FLVCodecContextFixture: public FLVStreamFixture, public CodecContextFixture {

    FLVCodecContextFixture() : FLVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct FLVOpenedCodecContextFixture: public FLVStreamFixture, public OpenedCodecContextFixture {

    FLVOpenedCodecContextFixture() : FLVStreamFixture(),
            OpenedCodecContextFixture(streams, streamNumber) {}
};


/** PACKET CONTEXT FIXTURES **/

struct AVIPacketFixture: public AVIFormatContextFixture, public PacketFixture {

    AVIPacketFixture() : AVIFormatContextFixture(), PacketFixture(formatContext) {}
};

struct MKVPacketFixture: public MKVFormatContextFixture, public PacketFixture {

    MKVPacketFixture() : MKVFormatContextFixture(), PacketFixture(formatContext) {}
};

struct OGVPacketFixture: public OGVFormatContextFixture, public PacketFixture {

    OGVPacketFixture() : OGVFormatContextFixture(), PacketFixture(formatContext) {}
};

struct MP4PacketFixture: public MP4FormatContextFixture, public PacketFixture {

    MP4PacketFixture() : MP4FormatContextFixture(), PacketFixture(formatContext) {}
};

struct FLVPacketFixture: public FLVFormatContextFixture, public PacketFixture {

    FLVPacketFixture() : FLVFormatContextFixture(), PacketFixture(formatContext) {}
};


/** AUDIO PACKET CONTEXT FIXTURES **/

struct AVIAudioPacketFixture: public AVIOpenedCodecContextFixture, public AudioPacketFixture {

    AVIAudioPacketFixture() : AVIOpenedCodecContextFixture(), AudioPacketFixture(formatContext) {}
};

struct MKVAudioPacketFixture: public MKVOpenedCodecContextFixture, public AudioPacketFixture {

    MKVAudioPacketFixture() : MKVOpenedCodecContextFixture(), AudioPacketFixture(formatContext) {}
};

struct OGVAudioPacketFixture: public OGVOpenedCodecContextFixture, public AudioPacketFixture {

    OGVAudioPacketFixture() : OGVOpenedCodecContextFixture(), AudioPacketFixture(formatContext) {}
};

struct MP4AudioPacketFixture: public MP4OpenedCodecContextFixture, public AudioPacketFixture {

    MP4AudioPacketFixture() : MP4OpenedCodecContextFixture(), AudioPacketFixture(formatContext) {}
};

struct FLVAudioPacketFixture: public FLVOpenedCodecContextFixture, public AudioPacketFixture {

    FLVAudioPacketFixture() : FLVOpenedCodecContextFixture(), AudioPacketFixture(formatContext) {}
};


/** VIDEO PACKET CONTEXT FIXTURES **/

struct AVIVideoPacketFixture: public AVIOpenedCodecContextFixture, public VideoPacketFixture {

    AVIVideoPacketFixture() : AVIOpenedCodecContextFixture(), VideoPacketFixture(formatContext) {}
};

struct MKVVideoPacketFixture: public MKVOpenedCodecContextFixture, public VideoPacketFixture {

    MKVVideoPacketFixture() : MKVOpenedCodecContextFixture(), VideoPacketFixture(formatContext) {}
};

struct OGVVideoPacketFixture: public OGVOpenedCodecContextFixture, public VideoPacketFixture {

    OGVVideoPacketFixture() : OGVOpenedCodecContextFixture(), VideoPacketFixture(formatContext) {}
};

struct MP4VideoPacketFixture: public MP4OpenedCodecContextFixture, public VideoPacketFixture {

    MP4VideoPacketFixture() : MP4OpenedCodecContextFixture(), VideoPacketFixture(formatContext) {}
};

struct FLVVideoPacketFixture: public FLVOpenedCodecContextFixture, public VideoPacketFixture {

    FLVVideoPacketFixture() : FLVOpenedCodecContextFixture(), VideoPacketFixture(formatContext) {}
};


/** AUDIO FRAME CONTEXT FIXTURES **/

struct AVIAudioFrameFixture: public AVIOpenedCodecContextFixture, public AudioFrameFixture {

    AVIAudioFrameFixture() : AVIOpenedCodecContextFixture(), AudioFrameFixture(formatContext) {}
};

struct MKVAudioFrameFixture: public MKVOpenedCodecContextFixture, public AudioFrameFixture {

    MKVAudioFrameFixture() : MKVOpenedCodecContextFixture(), AudioFrameFixture(formatContext) {}
};

struct OGVAudioFrameFixture: public OGVOpenedCodecContextFixture, public AudioFrameFixture {

    OGVAudioFrameFixture() : OGVOpenedCodecContextFixture(), AudioFrameFixture(formatContext) {}
};

struct MP4AudioFrameFixture: public MP4OpenedCodecContextFixture, public AudioFrameFixture {

    MP4AudioFrameFixture() : MP4OpenedCodecContextFixture(), AudioFrameFixture(formatContext) {}
};

struct FLVAudioFrameFixture: public FLVOpenedCodecContextFixture, public AudioFrameFixture {

    FLVAudioFrameFixture() : FLVOpenedCodecContextFixture(), AudioFrameFixture(formatContext) {}
};


/** VIDEO FRAME CONTEXT FIXTURES **/

struct AVIVideoFrameFixture: public AVIOpenedCodecContextFixture, public VideoFrameFixture {

    AVIVideoFrameFixture() : AVIOpenedCodecContextFixture(), VideoFrameFixture(formatContext) {}
};

struct MKVVideoFrameFixture: public MKVOpenedCodecContextFixture, public VideoFrameFixture {

    MKVVideoFrameFixture() : MKVOpenedCodecContextFixture(), VideoFrameFixture(formatContext) {}
};

struct OGVVideoFrameFixture: public OGVOpenedCodecContextFixture, public VideoFrameFixture {

    OGVVideoFrameFixture() : OGVOpenedCodecContextFixture(), VideoFrameFixture(formatContext) {}
};

struct MP4VideoFrameFixture: public MP4OpenedCodecContextFixture, public VideoFrameFixture {

    MP4VideoFrameFixture() : MP4OpenedCodecContextFixture(), VideoFrameFixture(formatContext) {}
};

struct FLVVideoFrameFixture: public FLVOpenedCodecContextFixture, public VideoFrameFixture {

    FLVVideoFrameFixture() : FLVOpenedCodecContextFixture(), VideoFrameFixture(formatContext) {}
};

} /* namespace test */

#endif /* __TEST_FIXTURES_HPP__ */
