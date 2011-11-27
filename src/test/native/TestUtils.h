#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <string>

// Paths to the test media files.
const std::string VIDEO_ONE = "../../../target/test-classes/lib-test/test.avi";
const std::string VIDEO_TWO = "../../../target/test-classes/lib-test/test.mkv";
const std::string VIDEO_THREE = "../../../target/test-classes/lib-test/test.mp4";
const std::string TEXT_FILE = "../../../target/test-classes/lib-test/test.txt";

// The media containers that will be tested.
const std::string AVI_CONTAINER = "video/x-msvideo";
const std::string MKV_CONTAINER = "video/x-matroska";
const std::string MP4_CONTAINER = "video/quicktime";

// The audio types that will be tested.
const std::string MPG_AUDIO = "audio/mpeg";

// The video types that will be tested.
const std::string DIVX_VIDEO = "video/x-xvid";
const std::string H264_VIDEO = "video/x-h264";

#endif /* __TEST_UTILS_H__ */
