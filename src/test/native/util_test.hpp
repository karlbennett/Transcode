#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <metadata.hpp>

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
const int VIDEO_OGV_SIZE = 1748992;
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

// The video types that will be tested.
const std::string DIVX_VIDEO = "video/x-divx";
const std::string H264_VIDEO = "video/x-h264";
const std::string FLV1_VIDEO = "video/x-svq";
const std::string THEORA_VIDEO = "video/x-theora";

// The video resolution that will be tested.
const int VIDEO_WIDTH = 1280;
const int VIDEO_HEIGHT = 544;
const int VIDEO_FRAME_RATE = 0;
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

/**
 * Helper function for testing the subtitle metadata of the AVI file.
 *
 * @param subtitleMetaData - the collection of SubtitleMetaData structs to be tested.
 */
static void testAVISubtitles(
        std::vector<transcode::SubtitleMetaData> subtitleMetaData) {

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Helper function for testing the subtitle metadata of the MKV file.
 *
 * @param subtitleMetaData - the collection of SubtitleMetaData structs to be tested.
 */
static void testMKVSubtitles(
        std::vector<transcode::SubtitleMetaData> subtitleMetaData) {

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 10);

    BOOST_CHECK_EQUAL( subtitleMetaData[0].language, LANG_GER);
    BOOST_CHECK_EQUAL( subtitleMetaData[1].language, LANG_ENG);
    BOOST_CHECK_EQUAL( subtitleMetaData[2].language, LANG_SPA);
    BOOST_CHECK_EQUAL( subtitleMetaData[3].language, LANG_FRE);
    BOOST_CHECK_EQUAL( subtitleMetaData[4].language, LANG_ITA);
    BOOST_CHECK_EQUAL( subtitleMetaData[5].language, LANG_DUT);
    BOOST_CHECK_EQUAL( subtitleMetaData[6].language, LANG_POL);
    BOOST_CHECK_EQUAL( subtitleMetaData[7].language, LANG_POR);
    BOOST_CHECK_EQUAL( subtitleMetaData[8].language, LANG_RUS);
    BOOST_CHECK_EQUAL( subtitleMetaData[9].language, LANG_VIE);
}

/**
 * Helper function for testing the subtitle metadata of the MP4 file.
 *
 * @param subtitleMetaData - the collection of SubtitleMetaData structs to be tested.
 */
static void testMP4Subtitles(
        std::vector<transcode::SubtitleMetaData> subtitleMetaData) {

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Helper function for testing the subtitle metadata of the OGV file.
 *
 * @param subtitleMetaData - the collection of SubtitleMetaData structs to be tested.
 */
static void testOGVSubtitles(
        std::vector<transcode::SubtitleMetaData> subtitleMetaData) {

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Helper function for testing the subtitle metadata of the FLV file.
 *
 * @param subtitleMetaData - the collection of SubtitleMetaData structs to be tested.
 */
static void testFLVSubtitles(
        std::vector<transcode::SubtitleMetaData> subtitleMetaData) {

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Helper function for testing the audio metadata of the AVI file.
 *
 * @param audioMetaData - the collection of AudioMetaData structs to be tested.
 */
static void testAVIAudio(std::vector<transcode::AudioMetaData> audioMetaData) {

    BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
    BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}

/**
 * Helper function for testing the audio metadata of the MKV file.
 *
 * @param audioMetaData - the collection of AudioMetaData structs to be tested.
 */
static void testMKVAudio(std::vector<transcode::AudioMetaData> audioMetaData) {

    BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
    BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, AC3_AUDIO);
}

/**
 * Helper function for testing the audio metadata of the MP4 file.
 *
 * @param audioMetaData - the collection of AudioMetaData structs to be tested.
 */
static void testMP4Audio(std::vector<transcode::AudioMetaData> audioMetaData) {

    BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
    BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}

/**
 * Helper function for testing the audio metadata of the OGV file.
 *
 * @param audioMetaData - the collection of AudioMetaData structs to be tested.
 */
static void testOGVAudio(std::vector<transcode::AudioMetaData> audioMetaData) {

    BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
    BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, VORBIS_AUDIO);
}

/**
 * Helper function for testing the audio metadata of the FLV file.
 *
 * @param audioMetaData - the collection of AudioMetaData structs to be tested.
 */
static void testFLVAudio(std::vector<transcode::AudioMetaData> audioMetaData) {

    BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
    BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}



/**
 * Helper function for testing the video metadata of the AVI file.
 *
 * @param videoMetaData - the collection of VideoMetaData structs to be tested.
 */
static void testAVIVideo(std::vector<transcode::VideoMetaData> videoMetaData) {

    BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
    BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, DIVX_VIDEO);
    BOOST_CHECK_EQUAL( videoMetaData[0].width, VIDEO_WIDTH);
    BOOST_CHECK_EQUAL( videoMetaData[0].height, VIDEO_HEIGHT);
    BOOST_CHECK_EQUAL( videoMetaData[0].frameRate, VIDEO_FRAME_RATE);
}

/**
 * Helper function for testing the video metadata of the MKV file.
 *
 * @param videoMetaData - the collection of VideoMetaData structs to be tested.
 */
static void testMKVVideo(std::vector<transcode::VideoMetaData> videoMetaData) {

    BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
    BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, H264_VIDEO);
    BOOST_CHECK_EQUAL( videoMetaData[0].width, VIDEO_WIDTH);
    BOOST_CHECK_EQUAL( videoMetaData[0].height, VIDEO_HEIGHT);
    BOOST_CHECK_EQUAL( videoMetaData[0].frameRate, VIDEO_FRAME_RATE);
}

/**
 * Helper function for testing the video metadata of the MP4 file.
 *
 * @param videoMetaData - the collection of VideoMetaData structs to be tested.
 */
static void testMP4Video(std::vector<transcode::VideoMetaData> videoMetaData) {

    BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
    BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, H264_VIDEO);
    BOOST_CHECK_EQUAL( videoMetaData[0].width, VIDEO_WIDTH);
    BOOST_CHECK_EQUAL( videoMetaData[0].height, VIDEO_HEIGHT);
    BOOST_CHECK_EQUAL( videoMetaData[0].frameRate, VIDEO_FRAME_RATE);
}

/**
 * Helper function for testing the video metadata of the OGV file.
 *
 * @param videoMetaData - the collection of VideoMetaData structs to be tested.
 */
static void testOGVVideo(std::vector<transcode::VideoMetaData> videoMetaData) {

    BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
    BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, THEORA_VIDEO);
    BOOST_CHECK_EQUAL( videoMetaData[0].width, VIDEO_WIDTH);
    BOOST_CHECK_EQUAL( videoMetaData[0].height, VIDEO_HEIGHT);
    BOOST_CHECK_EQUAL( videoMetaData[0].frameRate, VIDEO_FRAME_RATE);
}

/**
 * Helper function for testing the video metadata of the FLV file.
 *
 * @param videoMetaData - the collection of VideoMetaData structs to be tested.
 */
static void testFLVVideo(std::vector<transcode::VideoMetaData> videoMetaData) {

    BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
    BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, FLV1_VIDEO);
    BOOST_CHECK_EQUAL( videoMetaData[0].width, VIDEO_WIDTH);
    BOOST_CHECK_EQUAL( videoMetaData[0].height, VIDEO_HEIGHT);
    BOOST_CHECK_EQUAL( videoMetaData[0].frameRate, VIDEO_FLV_FRAME_RATE);
}

/**
 * Helper function for testing the container metadata of the AVI file.
 *
 * @param containerMetaData - the ContainerMetaData struct to be tested.
 */
static void testAVIContainer(transcode::ContainerMetaData containerMetaData) {

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, AVI_CONTAINER);

    testAVISubtitles(containerMetaData.subtitleDetails);

    testAVIAudio(containerMetaData.audioDetails);

    testAVIVideo(containerMetaData.videoDetails);
}

/**
 * Helper function for testing the container metadata of the MKV file.
 *
 * @param containerMetaData - the ContainerMetaData struct to be tested.
 */
static void testMKVContainer(transcode::ContainerMetaData containerMetaData) {

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, MKV_CONTAINER);

    testMKVSubtitles(containerMetaData.subtitleDetails);

    testMKVAudio(containerMetaData.audioDetails);

    testMKVVideo(containerMetaData.videoDetails);
}

/**
 * Helper function for testing the container metadata of the MP4 file.
 *
 * @param containerMetaData - the ContainerMetaData struct to be tested.
 */
static void testMP4Container(transcode::ContainerMetaData containerMetaData) {

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, MP4_CONTAINER);

    testMP4Subtitles(containerMetaData.subtitleDetails);

    testMP4Audio(containerMetaData.audioDetails);

    testMP4Video(containerMetaData.videoDetails);
}

/**
 * Helper function for testing the container metadata of the OGV file.
 *
 * @param containerMetaData - the ContainerMetaData struct to be tested.
 */
static void testOGVContainer(transcode::ContainerMetaData containerMetaData) {

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, OGV_CONTAINER);

    testOGVSubtitles(containerMetaData.subtitleDetails);

    testOGVAudio(containerMetaData.audioDetails);

    testOGVVideo(containerMetaData.videoDetails);
}

/**
 * Helper function for testing the container metadata of the FLV file.
 *
 * @param containerMetaData - the ContainerMetaData struct to be tested.
 */
static void testFLVContainer(transcode::ContainerMetaData containerMetaData) {

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, FLV_CONTAINER);

    testFLVSubtitles(containerMetaData.subtitleDetails);

    testFLVAudio(containerMetaData.audioDetails);

    testFLVVideo(containerMetaData.videoDetails);
}

/**
 * Helper function for testing the media file metadata of the AVI file.
 *
 * @param medaFile - the MediaFileMetaData struct to be tested.
 */
static void testAVIMediaFile(transcode::MediaFileMetaData medaFile) {

    BOOST_CHECK_EQUAL( medaFile.path, VIDEO_AVI);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_AVI_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_AVI_SIZE);

    testAVIContainer(medaFile.container);
}

/**
 * Helper function for testing the media file metadata of the MKV file.
 *
 * @param medaFile - the MediaFileMetaData struct to be tested.
 */
static void testMKVMediaFile(transcode::MediaFileMetaData medaFile) {

    BOOST_CHECK_EQUAL( medaFile.path, VIDEO_MKV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_MKV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_MKV_SIZE);

    testMKVContainer(medaFile.container);
}

/**
 * Helper function for testing the media file metadata of the MP4 file.
 *
 * @param medaFile - the MediaFileMetaData struct to be tested.
 */
static void testMP4MediaFile(transcode::MediaFileMetaData medaFile) {

    BOOST_CHECK_EQUAL( medaFile.path, VIDEO_MP4);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_MP4_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_MP4_SIZE);

    testMP4Container(medaFile.container);
}

/**
 * Helper function for testing the media file metadata of the OGV file.
 *
 * @param medaFile - the MediaFileMetaData struct to be tested.
 */
static void testOGVMediaFile(transcode::MediaFileMetaData medaFile) {

    BOOST_CHECK_EQUAL( medaFile.path, VIDEO_OGV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_OGV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_OGV_SIZE);

    testOGVContainer(medaFile.container);
}

/**
 * Helper function for testing the media file metadata of the FLV file.
 *
 * @param medaFile - the MediaFileMetaData struct to be tested.
 */
static void testFLVMediaFile(transcode::MediaFileMetaData medaFile) {

    BOOST_CHECK_EQUAL( medaFile.path, VIDEO_FLV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_FLV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_FLV_SIZE);

    testFLVContainer(medaFile.container);
}

#endif /* __TEST_UTILS_H__ */
