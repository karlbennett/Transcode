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
}

#include <util_test.hpp>

#include <iostream>
#include <string>

namespace test {

/** BASE FIXTURES **/

struct FormatContextFixture {

    FormatContextFixture() : formatContext(NULL) {}
    FormatContextFixture(AVFormatContext *fc) : formatContext(fc) {}
    FormatContextFixture(std::string fileName) : formatContext(NULL) {

        avformat_open_input(&formatContext, fileName.c_str(), NULL,
                            NULL);

        avformat_find_stream_info(formatContext, NULL);
    }

    virtual ~FormatContextFixture() {
        if (NULL != formatContext) avformat_close_input(&formatContext);
    }

    AVFormatContext *formatContext;
};

struct StreamFixture {

    StreamFixture(const AVFormatContext *fc) : streams(fc->streams),
            streamNumber(fc->nb_streams) {}

    virtual ~StreamFixture() {}

    AVStream **streams;
    int streamNumber;
};

struct CodecContextFixture {

    CodecContextFixture(AVStream **streams, const int& size) {

        codecs = new AVCodecContext*[size];

        for (int i = 0; i < size; i++) {

            codecs[i] = streams[i]->codec;
        }
    }

    virtual ~CodecContextFixture() {

        delete codecs;
    }

    AVCodecContext **codecs;
};

struct PacketFixture {

    PacketFixture(AVFormatContext *fc) {

        packet = new AVPacket();

        av_init_packet(packet);

        int error = av_read_frame(fc, packet);

        if (0 != error) throw "Failed to read AVPacket in test fixture.";
    }

    virtual ~PacketFixture() {

        av_free_packet(packet);
    }

    AVPacket *packet;
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

struct MKVCodecContextFixture: public MKVStreamFixture, public CodecContextFixture {

    MKVCodecContextFixture() : MKVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct OGVCodecContextFixture: public OGVStreamFixture, public CodecContextFixture {

    OGVCodecContextFixture() : OGVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct MP4CodecContextFixture: public MP4StreamFixture, public CodecContextFixture {

    MP4CodecContextFixture() : MP4StreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
};

struct FLVCodecContextFixture: public FLVStreamFixture, public CodecContextFixture {

    FLVCodecContextFixture() : FLVStreamFixture(),
            CodecContextFixture(streams, streamNumber) {}
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
}

#endif /* __TEST_FIXTURES_HPP__ */
