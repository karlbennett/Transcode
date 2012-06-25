#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__


#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

// Paths to the test media files.
const std::string VIDEO_AVI = "../../../target/test-classes/lib-test/test.avi";
const std::string VIDEO_MKV = "../../../target/test-classes/lib-test/test.mkv";
const std::string VIDEO_MP4 = "../../../target/test-classes/lib-test/test.mp4";
const std::string VIDEO_OGV = "../../../target/test-classes/lib-test/test.ogv";
const std::string VIDEO_FLV = "../../../target/test-classes/lib-test/test.flv";
const std::string INVALID_FILE = "file does not exist";
const std::string TEXT_FILE = "../../../target/test-classes/lib-test/test.txt";
const std::string EMPTY_FILE = "../../../target/test-classes/lib-test/test.empty";

// Names of the test media files.
const std::string VIDEO_AVI_NAME = "test.avi";
const std::string VIDEO_MKV_NAME = "test.mkv";
const std::string VIDEO_MP4_NAME = "test.mp4";
const std::string VIDEO_OGV_NAME = "test.ogv";
const std::string VIDEO_FLV_NAME = "test.flv";
const std::string TEXT_NAME = "test.txt";

// Sizes of the test media files.
const int VIDEO_AVI_SIZE = 1398174;
const int VIDEO_MKV_SIZE = 9171456;
const int VIDEO_MP4_SIZE = 8678105;
const int VIDEO_OGV_SIZE = 1010760;
const int VIDEO_FLV_SIZE = 2133883;
const int TEXT_SIZE = 25;

// The media containers that will be tested.
const std::string AVI_CONTAINER = "video/x-msvideo";
const std::string MKV_CONTAINER = "video/x-matroska";
const std::string MP4_CONTAINER = "video/quicktime";
const std::string OGV_CONTAINER = "application/ogg";
const std::string FLV_CONTAINER = "video/x-flv";

// The audio types that will be tested.
const std::string MPG_AUDIO = "audio/mpeg";
const std::string AC3_AUDIO = "audio/x-ac3";
const std::string VORBIS_AUDIO = "audio/x-vorbis";

// The stream types that will be tested.
// 0 - AVMEDIA_TYPE_VIDEO
// 1 - AVMEDIA_TYPE_AUDIO
// 2 - AVMEDIA_TYPE_SUBTITLE
const int DIVX_STREAM_ONE = 0;
const int DIVX_STREAM_TWO = 1;
const int MKV_STREAM_ONE = 0;
const int MKV_STREAM_TWO = 1;
const int OGV_STREAM_ONE = 2;
const int OGV_STREAM_TWO = 0;
const int OGV_STREAM_THREE = 1;
const int MP4_STREAM_ONE = 0;
const int MP4_STREAM_TWO = 1;
const int FLV_STREAM_ONE = 0;
const int FLV_STREAM_TWO = 1;

// The video types that will be tested.
const std::string DIVX_VIDEO = "video/x-divx";
const std::string H264_VIDEO = "video/x-h264";
const std::string FLV1_VIDEO = "video/x-svq";
const std::string THEORA_VIDEO = "video/x-theora";

// The video resolution that will be tested.
const int VIDEO_WIDTH = 1280;
const int VIDEO_HEIGHT = 544;
const int VIDEO_FRAME_RATE = 0;
const int VIDEO_MKV_FRAME_RATE = 6;
const int VIDEO_FLV_FRAME_RATE = 1;

// The language types that will be tested.
const std::string LANG_GER = "ger";
const std::string LANG_ENG = "eng";
const std::string LANG_SPA = "spa";
const std::string LANG_FRE = "fre";
const std::string LANG_ITA = "ita";
const std::string LANG_DUT = "dut";
const std::string LANG_POL = "pol";
const std::string LANG_POR = "por";
const std::string LANG_RUS = "rus";
const std::string LANG_VIE = "vie";

#endif /* __TEST_UTILS_H__ */
