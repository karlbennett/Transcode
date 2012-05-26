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

namespace test {

/** BASE FIXTURES **/

struct FormatContextFixture {

    FormatContextFixture() : formatContext(NULL) {}
    FormatContextFixture(AVFormatContext *fc) : formatContext(fc) {}

    virtual ~FormatContextFixture() {
        if (NULL != formatContext) avformat_close_input(&formatContext);
    }

    AVFormatContext *formatContext;
};

struct StreamFixture {

    StreamFixture(const AVFormatContext *fc) : streams(fc->streams) {}

    virtual ~StreamFixture() {}

    AVStream **streams;
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


/** FORMAT CONTEXT FIXTURES **/

struct AVIFormatContextFixture: public FormatContextFixture {

    AVIFormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, VIDEO_AVI.c_str(), NULL,
                    NULL);
    }
};

struct MKVFormatContextFixture: public FormatContextFixture {

    MKVFormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, VIDEO_MKV.c_str(), NULL,
                    NULL);
    }
};

struct OGVFormatContextFixture: public FormatContextFixture {

    OGVFormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, VIDEO_OGV.c_str(), NULL,
                    NULL);
    }
};

struct MP4FormatContextFixture: public FormatContextFixture {

    MP4FormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, VIDEO_MP4.c_str(), NULL,
                    NULL);
    }
};

struct FLVFormatContextFixture: public FormatContextFixture {

    FLVFormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, VIDEO_FLV.c_str(), NULL,
                    NULL);
    }
};

struct TextFormatContextFixture: public FormatContextFixture {

    TextFormatContextFixture() : FormatContextFixture() {

        avformat_open_input(&formatContext, TEXT_FILE.c_str(), NULL,
                    NULL);
    }
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
            CodecContextFixture(streams, formatContext->nb_streams) {}
};

struct MKVCodecContextFixture: public MKVStreamFixture, public CodecContextFixture {

    MKVCodecContextFixture() : MKVStreamFixture(),
            CodecContextFixture(streams, formatContext->nb_streams) {}
};

struct OGVCodecContextFixture: public OGVStreamFixture, public CodecContextFixture {

    OGVCodecContextFixture() : OGVStreamFixture(),
            CodecContextFixture(streams, formatContext->nb_streams) {}
};

struct MP4CodecContextFixture: public MP4StreamFixture, public CodecContextFixture {

    MP4CodecContextFixture() : MP4StreamFixture(),
            CodecContextFixture(streams, formatContext->nb_streams) {}
};

struct FLVCodecContextFixture: public FLVStreamFixture, public CodecContextFixture {

    FLVCodecContextFixture() : FLVStreamFixture(),
            CodecContextFixture(streams, formatContext->nb_streams) {}
};

}

#endif /* __TEST_FIXTURES_HPP__ */
