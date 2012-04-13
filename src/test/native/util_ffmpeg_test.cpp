#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_ffmpeg.hpp>
#include <util_test.hpp>

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

AVFormatContext blankFormatContext;

// The media types of the first frames in the media files.
const AVMediaType FRAME_TYPE_AVI = AVMEDIA_TYPE_VIDEO;
const AVMediaType FRAME_TYPE_MKV = AVMEDIA_TYPE_VIDEO;
const AVMediaType FRAME_TYPE_MP4 = AVMEDIA_TYPE_AUDIO;
const AVMediaType FRAME_TYPE_OGV = AVMEDIA_TYPE_AUDIO;
const AVMediaType FRAME_TYPE_FLV = AVMEDIA_TYPE_AUDIO;

/**
 * Test to make sure that an FFMPEG error message can be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message )
{

	BOOST_REQUIRE( transcode::util::UNKNOWN !=
	        transcode::util::ffmpegErrorMessage(0));
}

/**
 * Test to make sure that the correct message is returned if an
 * FFMPEG error message cannot be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message_not_found )
{

    BOOST_REQUIRE( transcode::util::UNKNOWN ==
            transcode::util::ffmpegErrorMessage(1));
}

/**
 * Test to make sure that a format context can be found for all
 * the video files.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context )
{

    (void) transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::retrieveAVFormatContext(VIDEO_FLV);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an empty string.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_string )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(""),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an invalid file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_invalid_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(INVALID_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an empty file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(EMPTY_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from a text file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_non_media_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(TEXT_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_subtitles )
{
    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::extractSubtitleDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::extractSubtitleDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::extractSubtitleDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::extractSubtitleDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    (void) transcode::util::extractSubtitleDetails(formatContext);
    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that an exception is thrown when subtitle metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractSubtitleDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when subtitle
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractSubtitleDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_subtitles )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testAVISubtitles(subtitleMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_subtitles )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testMKVSubtitles(subtitleMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_subtitles )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testMP4Subtitles(subtitleMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_subtitles )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testOGVSubtitles(subtitleMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_subtitles )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testFLVSubtitles(subtitleMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_audio )
{
    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::extractAudioDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::extractAudioDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::extractAudioDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::extractAudioDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    (void) transcode::util::extractAudioDetails(formatContext);
    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that an exception is thrown when audio metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractAudioDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when audio
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractAudioDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_audio )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testAVIAudio(audioMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_audio )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testMKVAudio(audioMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_audio )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testMP4Audio(audioMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_audio )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testOGVAudio(audioMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_audio )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testFLVAudio(audioMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_video )
{
    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::extractVideoDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::extractVideoDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::extractVideoDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::extractVideoDetails(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    (void) transcode::util::extractVideoDetails(formatContext);
    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that an exception is thrown when video metadata
 * is requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractVideoDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when video
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractVideoDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_video )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testAVIVideo(videoMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_video )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testMKVVideo(videoMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_video )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testMP4Video(videoMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_video )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testOGVVideo(videoMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_video )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testFLVVideo(videoMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_container )
{
    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::buildContainerDetail(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::buildContainerDetail(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::buildContainerDetail(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::buildContainerDetail(formatContext);
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    (void) transcode::util::buildContainerDetail(formatContext);
    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::buildContainerDetail(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::buildContainerDetail(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_avi_container )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testAVIContainer(containerMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mkv_container )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testMKVContainer(containerMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mp4_container )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testMP4Container(containerMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_ogv_container )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testOGVContainer(containerMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_flv_container )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testFLVContainer(containerMetaData);

    avformat_close_input(&formatContext);
}

/**
 * Helper function for test packet reads all the way to the end of the
 * media file.
 *
 * @param formatContext - the format context that will have the packets
 *      read from it.
 */
void testReadPackets(AVFormatContext *formatContext) {

    AVPacket *packet = NULL;

    while (NULL != (packet = transcode::util::readNextPacket(formatContext))) {

        // The packet should not be empty.
        BOOST_REQUIRE( 0 < packet->size);

        av_free_packet(packet);
    }

    // Last packet should be NULL.
    BOOST_REQUIRE( NULL == packet);
}

/**
 * Test to make sure that a packet can be read from all the different container
 * types.
 */
BOOST_AUTO_TEST_CASE( test_read_next_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    av_free_packet(transcode::util::readNextPacket(formatContext));
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    av_free_packet(transcode::util::readNextPacket(formatContext));
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    av_free_packet(transcode::util::readNextPacket(formatContext));
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    av_free_packet(transcode::util::readNextPacket(formatContext));
    avformat_close_input(&formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    av_free_packet(transcode::util::readNextPacket(formatContext));
    avformat_close_input(&formatContext);
}

BOOST_AUTO_TEST_CASE( test_read_next_packet_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket(NULL),
            transcode::util::FFMPEGException);
}

BOOST_AUTO_TEST_CASE( test_read_next_packet_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_avi_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext));

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_avi_packets )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);

    testReadPackets(formatContext);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mkv_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext));

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mkv_packets )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);

    testReadPackets(formatContext);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mp4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mp4_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext));

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mp4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mp4_packets )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);

    testReadPackets(formatContext);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_ogv_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext));

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_ogv_packets )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);

    testReadPackets(formatContext);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_flv_packet )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext));

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_flv_packets )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);

    testReadPackets(formatContext);

    avformat_close_input(&formatContext);
}

/**
 * Test to make sure that a packets type can be found when read from all the
 * different container types.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    packet = transcode::util::readNextPacket(formatContext);
    (void) transcode::util::findPacketType(packet, formatContext);
    avformat_close_input(&formatContext);
    av_free_packet(packet);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    packet = transcode::util::readNextPacket(formatContext);
    (void) transcode::util::findPacketType(packet, formatContext);
    avformat_close_input(&formatContext);
    av_free_packet(packet);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    packet = transcode::util::readNextPacket(formatContext);
    (void) transcode::util::findPacketType(packet, formatContext);
    avformat_close_input(&formatContext);
    av_free_packet(packet);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    packet = transcode::util::readNextPacket(formatContext);
    (void) transcode::util::findPacketType(packet, formatContext);
    avformat_close_input(&formatContext);
    av_free_packet(packet);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    packet = transcode::util::readNextPacket(formatContext);
    (void) transcode::util::findPacketType(packet, formatContext);
    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure the right packet type is found for avi packets.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_avi_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    packet = transcode::util::readNextPacket(formatContext);

    BOOST_CHECK_EQUAL( FRAME_TYPE_AVI,
            transcode::util::findPacketType(packet, formatContext));

    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure the right packet type is found for mkv packets.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_mkv_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    packet = transcode::util::readNextPacket(formatContext);

    BOOST_CHECK_EQUAL( FRAME_TYPE_MKV,
            transcode::util::findPacketType(packet, formatContext));

    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure the right packet type is found for mp4 packets.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_mp4_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    packet = transcode::util::readNextPacket(formatContext);

    BOOST_CHECK_EQUAL( FRAME_TYPE_MP4,
            transcode::util::findPacketType(packet, formatContext));

    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure the right packet type is found for ogv packets.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_ogv_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    packet = transcode::util::readNextPacket(formatContext);

    BOOST_CHECK_EQUAL( FRAME_TYPE_OGV,
            transcode::util::findPacketType(packet, formatContext));

    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure the right packet type is found for flv packets.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_find_flv_packet_type )
{

    AVFormatContext *formatContext = NULL;
    AVPacket *packet = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    packet = transcode::util::readNextPacket(formatContext);

    BOOST_CHECK_EQUAL( FRAME_TYPE_FLV,
            transcode::util::findPacketType(packet, formatContext));

    avformat_close_input(&formatContext);
    av_free_packet(packet);
}

/**
 * Test to make sure that populated format contexts can have their codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs )
{

    AVFormatContext *formatContext = NULL;

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    (void) transcode::util::closeCodecs(formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    (void) transcode::util::closeCodecs(formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    (void) transcode::util::closeCodecs(formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    (void) transcode::util::closeCodecs(formatContext);

    formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    (void) transcode::util::closeCodecs(formatContext);
}

/**
 * Test to make sure that trying to close NULL throws an exception.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_null )
{

    BOOST_REQUIRE_THROW( transcode::util::closeCodecs(NULL),
                transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty format contexts can have its codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_empty_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::closeCodecs(&blankFormatContext),
            transcode::util::FFMPEGException);
}
