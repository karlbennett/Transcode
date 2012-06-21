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
}

#include <util_test.hpp>

#include <iostream>
#include <string>

namespace test {

/** BASE FIXTURES **/

struct FormatContextFixture {

    FormatContextFixture() : formatContext(NULL) {}
    FormatContextFixture(AVFormatContext *fc) : formatContext(fc) {

        checkStreams();
    }
    FormatContextFixture(std::string fileName) : formatContext(NULL) {

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
};

struct StreamFixture {

    StreamFixture(const AVFormatContext *fc) : streams(fc->streams),
            streamNumber(fc->nb_streams) {}

    virtual ~StreamFixture() {}

    AVStream **streams;
    int streamNumber;
};

struct CodecContextFixture {

    CodecContextFixture(AVStream **streams, const int& size): codecNumber(size) {

        codecs = new AVCodecContext*[codecNumber];

        for (int i = 0; i < codecNumber; i++) {

            codecs[i] = streams[i]->codec;
        }
    }

    virtual ~CodecContextFixture() {

        delete codecs;
    }

    AVCodecContext **codecs;
    int codecNumber;
};

struct OpenedCodecContextFixture: public CodecContextFixture {

    OpenedCodecContextFixture(AVStream **streams, const int& size) :
            CodecContextFixture(streams, size) {

        AVCodecContext *codecContext = NULL;
        AVCodec *codec = NULL;

        int codecOpenResult = 0;

        for (int i = 0; i < codecNumber; i++) {

            codecContext = codecs[i];

            // Can only handle audio and video codecs at this time.
            if (AVMEDIA_TYPE_AUDIO == codecContext->codec_type
                    || AVMEDIA_TYPE_VIDEO == codecContext->codec_type) {

                codec = avcodec_find_decoder(codecContext->codec_id);

                if (NULL == codec) {

                    throw "Fixture codec context codec could not be found.";
                }

                codecOpenResult = avcodec_open2(codecContext, codec, NULL);

                if (0 != codecOpenResult)
                    throw "Fixture codec context could not be opened.";
            }
        }
    }

    virtual ~OpenedCodecContextFixture() {}
};

struct PacketFixture {

    PacketFixture(AVFormatContext *fc): packet(readPacket(fc)) {}
    PacketFixture(AVFormatContext *fc, AVMediaType type): packet(readPacket(fc)) {

        while (!checkPacketType(fc, packet, type)) {

            av_free_packet(packet);

            packet = readPacket(fc);
        }
    }

    virtual ~PacketFixture() {

        av_free_packet(packet);
    }

    AVPacket *packet;

    static AVPacket* readPacket(AVFormatContext *fc) {

        AVPacket *packet = new AVPacket();

        av_init_packet(packet);

        int error = av_read_frame(fc, packet);

        if (0 == error) return packet;

        throw "Failed to read AVPacket in test fixture.";
    }

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
};

struct AudioPacketFixture: public PacketFixture {

    AudioPacketFixture(AVFormatContext *fc) : PacketFixture(fc, AVMEDIA_TYPE_AUDIO) {}

    virtual ~AudioPacketFixture() {}
};

struct VideoPacketFixture: public PacketFixture {

    VideoPacketFixture(AVFormatContext *fc) : PacketFixture(fc, AVMEDIA_TYPE_VIDEO) {}

    virtual ~VideoPacketFixture() {}
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

} /* namespace test */

#endif /* __TEST_FIXTURES_HPP__ */
