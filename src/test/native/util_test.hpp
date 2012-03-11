#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <string>

// Paths to the test media files.
const std::string VIDEO_ONE = "../../../target/test-classes/lib-test/test.avi";
const std::string VIDEO_TWO = "../../../target/test-classes/lib-test/test.mkv";
const std::string VIDEO_THREE = "../../../target/test-classes/lib-test/test.mp4";
const std::string INVALID_FILE = "file does not exist";
const std::string TEXT_FILE = "../../../target/test-classes/lib-test/test.txt";

// Names of the test media files.
const std::string VIDEO_ONE_NAME = "test.avi";
const std::string VIDEO_TWO_NAME = "test.mkv";
const std::string VIDEO_THREE_NAME = "test.mp4";

// Sizes of the test media files.
const int VIDEO_ONE_SIZE = 1054514;
const int VIDEO_TWO_SIZE = 1491907;
const int VIDEO_THREE_SIZE = 799734;

// The media containers that will be tested.
const std::string AVI_CONTAINER = "video/x-msvideo";
const std::string MKV_CONTAINER = "video/x-matroska";
const std::string MP4_CONTAINER = "video/quicktime";

// The audio types that will be tested.
const std::string MPG_AUDIO = "audio/mpeg";

// The video types that will be tested.
const std::string DIVX_VIDEO = "video/x-divx";
const std::string H264_VIDEO = "video/x-h264";

#endif /* __TEST_UTILS_H__ */
