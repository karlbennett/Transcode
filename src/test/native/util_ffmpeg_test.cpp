#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_ffmpeg.hpp>
#include <util_test.hpp>

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

AVFormatContext *aviFormatContext = NULL;
AVFormatContext *mkvFormatContext = NULL;
AVFormatContext *mp4FormatContext = NULL;
AVFormatContext *ogvFormatContext = NULL;
AVFormatContext *flvFormatContext = NULL;

AVFormatContext blankFormatContext;

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

    aviFormatContext =
            transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    mkvFormatContext =
            transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    mp4FormatContext =
            transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    ogvFormatContext =
            transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    flvFormatContext =
            transcode::util::retrieveAVFormatContext(VIDEO_FLV);
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
    (void) transcode::util::extractSubtitleDetails(aviFormatContext);
    (void) transcode::util::extractSubtitleDetails(mkvFormatContext);
    (void) transcode::util::extractSubtitleDetails(mp4FormatContext);
    (void) transcode::util::extractSubtitleDetails(ogvFormatContext);
    (void) transcode::util::extractSubtitleDetails(flvFormatContext);
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

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(aviFormatContext);

    testAVISubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(mkvFormatContext);

    testMKVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(mp4FormatContext);

    testMP4Subtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(ogvFormatContext);

    testOGVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(flvFormatContext);

    testFLVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_audio )
{
    (void) transcode::util::extractAudioDetails(aviFormatContext);
    (void) transcode::util::extractAudioDetails(mkvFormatContext);
    (void) transcode::util::extractAudioDetails(mp4FormatContext);
    (void) transcode::util::extractAudioDetails(ogvFormatContext);
    (void) transcode::util::extractAudioDetails(flvFormatContext);
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

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(aviFormatContext);

    testAVIAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(mkvFormatContext);

    testMKVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(mp4FormatContext);

    testMP4Audio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(ogvFormatContext);

    testOGVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(flvFormatContext);

    testFLVAudio(audioMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_video )
{
    (void) transcode::util::extractVideoDetails(aviFormatContext);
    (void) transcode::util::extractVideoDetails(mkvFormatContext);
    (void) transcode::util::extractVideoDetails(mp4FormatContext);
    (void) transcode::util::extractVideoDetails(ogvFormatContext);
    (void) transcode::util::extractVideoDetails(flvFormatContext);
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

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(aviFormatContext);

    testAVIVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(mkvFormatContext);

    testMKVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(mp4FormatContext);

    testMP4Video(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(ogvFormatContext);

    testOGVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(flvFormatContext);

    testFLVVideo(videoMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_container )
{
    (void) transcode::util::buildContainerDetail(aviFormatContext);
    (void) transcode::util::buildContainerDetail(mkvFormatContext);
    (void) transcode::util::buildContainerDetail(mp4FormatContext);
    (void) transcode::util::buildContainerDetail(ogvFormatContext);
    (void) transcode::util::buildContainerDetail(flvFormatContext);
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

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(aviFormatContext);

    testAVIContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mkv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(mkvFormatContext);

    testMKVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mp4_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(mp4FormatContext);

    testMP4Container(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_ogv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(ogvFormatContext);

    testOGVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_flv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(flvFormatContext);

    testFLVContainer(containerMetaData);
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

BOOST_AUTO_TEST_CASE( test_read_next_packet )
{

    (void) transcode::util::readNextPacket(aviFormatContext);
    (void) transcode::util::readNextPacket(mkvFormatContext);
    (void) transcode::util::readNextPacket(mp4FormatContext);
    (void) transcode::util::readNextPacket(ogvFormatContext);
    (void) transcode::util::readNextPacket(flvFormatContext);
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

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(aviFormatContext) );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_avi_packets )
{

    testReadPackets(aviFormatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mkv_packet )
{

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(mkvFormatContext) );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mkv_packets )
{

    testReadPackets(mkvFormatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mp4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mp4_packet )
{

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(mp4FormatContext) );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mp4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_mp4_packets )
{

    testReadPackets(mp4FormatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_ogv_packet )
{

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(ogvFormatContext) );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_ogv_packets )
{

    testReadPackets(ogvFormatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_flv_packet )
{

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(flvFormatContext) );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_read_flv_packets )
{

    testReadPackets(flvFormatContext);
}

/**
 * Test to make sure that populated format contexts can have their codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs )
{

    (void) transcode::util::closeCodecs(aviFormatContext);
    (void) transcode::util::closeCodecs(mkvFormatContext);
    (void) transcode::util::closeCodecs(mp4FormatContext);
    (void) transcode::util::closeCodecs(ogvFormatContext);
    (void) transcode::util::closeCodecs(flvFormatContext);
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
