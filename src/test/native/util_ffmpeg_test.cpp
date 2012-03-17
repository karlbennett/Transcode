#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_ffmpeg.hpp>
#include <util_test.hpp>

extern "C" {
#include "libavformat/avformat.h"
}

AVFormatContext *aviFormatContext = NULL;
AVFormatContext *mkvFormatContext = NULL;
AVFormatContext *mp4FormatContext = NULL;
AVFormatContext *ogvFormatContext = NULL;
AVFormatContext *flvFormatContext = NULL;

AVFormatContext emptyFormatContext;

/**
 * Test to make sure that an FFMPEG error message can be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message )
{

	BOOST_REQUIRE( transcode::utils::UNKNOWN !=
	        transcode::utils::ffmpegErrorMessage(0));
}

/**
 * Test to make sure that the correct message is returned if an
 * FFMPEG error message cannot be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message_not_found )
{

    BOOST_REQUIRE( transcode::utils::UNKNOWN ==
            transcode::utils::ffmpegErrorMessage(1));
}

/**
 * Test to make sure that a format context can be found for all
 * the video files.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context )
{

    aviFormatContext =
            transcode::utils::retrieveAVFormatContext(VIDEO_AVI);
    mkvFormatContext =
            transcode::utils::retrieveAVFormatContext(VIDEO_MKV);
    mp4FormatContext =
            transcode::utils::retrieveAVFormatContext(VIDEO_MP4);
    ogvFormatContext =
            transcode::utils::retrieveAVFormatContext(VIDEO_OGV);
    flvFormatContext =
            transcode::utils::retrieveAVFormatContext(VIDEO_FLV);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an empty string.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_string )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(""),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an invalid file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_invalid_file )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(INVALID_FILE),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from a text file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_non_media_file )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(TEXT_FILE),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_subtitles )
{
    (void) transcode::utils::extractSubtitleDetails(aviFormatContext);
    (void) transcode::utils::extractSubtitleDetails(mkvFormatContext);
    (void) transcode::utils::extractSubtitleDetails(mp4FormatContext);
    (void) transcode::utils::extractSubtitleDetails(ogvFormatContext);
    (void) transcode::utils::extractSubtitleDetails(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when subtitle metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_null )
{

    BOOST_REQUIRE_THROW( transcode::utils::extractSubtitleDetails(NULL),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when subtitle
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_empty_format_context )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(&emptyFormatContext);

    BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(aviFormatContext);

    testAVISubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(mkvFormatContext);

    testMKVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(mp4FormatContext);

    testMP4Subtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(ogvFormatContext);

    testOGVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::utils::extractSubtitleDetails(flvFormatContext);

    testFLVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_audio )
{
    (void) transcode::utils::extractAudioDetails(aviFormatContext);
    (void) transcode::utils::extractAudioDetails(mkvFormatContext);
    (void) transcode::utils::extractAudioDetails(mp4FormatContext);
    (void) transcode::utils::extractAudioDetails(ogvFormatContext);
    (void) transcode::utils::extractAudioDetails(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when audio metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_null )
{

    BOOST_REQUIRE_THROW( transcode::utils::extractAudioDetails(NULL),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when audio
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_empty_format_context )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(&emptyFormatContext);

    BOOST_CHECK_EQUAL( audioMetaData.size(), 0);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(aviFormatContext);

    testAVIAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(mkvFormatContext);

    testMKVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(mp4FormatContext);

    testMP4Audio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(ogvFormatContext);

    testOGVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::utils::extractAudioDetails(flvFormatContext);

    testFLVAudio(audioMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_video )
{
    (void) transcode::utils::extractVideoDetails(aviFormatContext);
    (void) transcode::utils::extractVideoDetails(mkvFormatContext);
    (void) transcode::utils::extractVideoDetails(mp4FormatContext);
    (void) transcode::utils::extractVideoDetails(ogvFormatContext);
    (void) transcode::utils::extractVideoDetails(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when video metadata
 * is requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_null )
{

    BOOST_REQUIRE_THROW( transcode::utils::extractVideoDetails(NULL),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when video
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_empty_format_context )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(&emptyFormatContext);

    BOOST_CHECK_EQUAL( videoMetaData.size(), 0);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_avi_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(aviFormatContext);

    testAVIVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mkv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(mkvFormatContext);

    testMKVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_mp4_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(mp4FormatContext);

    testMP4Video(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_ogv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(ogvFormatContext);

    testOGVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_flv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::utils::extractVideoDetails(flvFormatContext);

    testFLVVideo(videoMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_container )
{
    (void) transcode::utils::buildContainerDetail(aviFormatContext);
    (void) transcode::utils::buildContainerDetail(mkvFormatContext);
    (void) transcode::utils::buildContainerDetail(mp4FormatContext);
    (void) transcode::utils::buildContainerDetail(ogvFormatContext);
    (void) transcode::utils::buildContainerDetail(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_null )
{

    BOOST_REQUIRE_THROW( transcode::utils::buildContainerDetail(NULL),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_empty_format_context )
{

    BOOST_REQUIRE_THROW( transcode::utils::buildContainerDetail(&emptyFormatContext),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_avi_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::utils::buildContainerDetail(aviFormatContext);

    testAVIContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mkv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::utils::buildContainerDetail(mkvFormatContext);

    testMKVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_mp4_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::utils::buildContainerDetail(mp4FormatContext);

    testMP4Container(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_ogv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::utils::buildContainerDetail(ogvFormatContext);

    testOGVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_build_flv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::utils::buildContainerDetail(flvFormatContext);

    testFLVContainer(containerMetaData);
}

/**
 * Test to make sure that populated format contexts can have their codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs )
{

    (void) transcode::utils::closeCodecs(aviFormatContext);
    (void) transcode::utils::closeCodecs(mkvFormatContext);
    (void) transcode::utils::closeCodecs(mp4FormatContext);
    (void) transcode::utils::closeCodecs(ogvFormatContext);
    (void) transcode::utils::closeCodecs(flvFormatContext);
}

/**
 * Test to make sure that trying to close NULL throws an exception.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_null )
{

    BOOST_REQUIRE_THROW( transcode::utils::closeCodecs(NULL),
                transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that an empty format contexts can have its codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_empty_format_context )
{

    transcode::utils::closeCodecs(&emptyFormatContext);
}
