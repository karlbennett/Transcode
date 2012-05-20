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

struct FormatContextFixture {

    FormatContextFixture() : formatContext(NULL) {}
    FormatContextFixture(AVFormatContext *fc) : formatContext(fc) {}

    virtual ~FormatContextFixture() {
        if (NULL != formatContext) avformat_close_input(&formatContext);
    }

    AVFormatContext *formatContext;
};

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

}

#endif /* __TEST_FIXTURES_HPP__ */
