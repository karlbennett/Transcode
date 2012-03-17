#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <util/util_media.hpp>
#include <metadata.hpp>
#include <util_test.hpp>


/**
 * Test to make sure that subtitles can be found for all the test
 * media files.
 */
BOOST_AUTO_TEST_CASE( test_find_subtitles )
{
    (void) transcode::findSubtitleDetails(VIDEO_AVI);
    (void) transcode::findSubtitleDetails(VIDEO_MKV);
    (void) transcode::findSubtitleDetails(VIDEO_MP4);
    (void) transcode::findSubtitleDetails(VIDEO_OGV);
    (void) transcode::findSubtitleDetails(VIDEO_FLV);
}

/**
 * Test to make sure that the subtitle find throws the correct
 * exception when an empty string is provided.
n*/
BOOST_AUTO_TEST_CASE( test_find_subtitles_for_empty_string )
{
    BOOST_REQUIRE_THROW( (void) transcode::findSubtitleDetails(""),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the subtitle find throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_subtitles_for_invalid_media_file )
{
    BOOST_REQUIRE_THROW(
            (void) transcode::findSubtitleDetails("file does not exist"),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the subtitle find throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_subtitles_for_non_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findSubtitleDetails(TEXT_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the subtitles of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_avi_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_AVI);

    testAVISubtitles(subtitleMetaData);
}

/**
 * Test to make sure that the subtitles of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_MKV);

    testMKVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that the subtitles of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_MP4);

    testMP4Subtitles(subtitleMetaData);
}

/**
 * Test to make sure that the subtitles of an ogv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_ogv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_OGV);

    testOGVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that the subtitles of an flv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_flv_subtitles )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_FLV);

    testFLVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that audio can be found for all the test media files.
 */
BOOST_AUTO_TEST_CASE( test_find_audio )
{
    (void) transcode::findAudioDetails(VIDEO_AVI);
    (void) transcode::findAudioDetails(VIDEO_MKV);
    (void) transcode::findAudioDetails(VIDEO_MP4);
    (void) transcode::findAudioDetails(VIDEO_OGV);
    (void) transcode::findAudioDetails(VIDEO_FLV);
}

/**
 * Test to make sure that the audio find throws the correct
 * exception when an empty string is provided.
n*/
BOOST_AUTO_TEST_CASE( test_find_audio_for_empty_string )
{
    BOOST_REQUIRE_THROW( (void) transcode::findAudioDetails(""),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the audio find throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_audio_for_invalid_media_file )
{
    BOOST_REQUIRE_THROW(
            (void) transcode::findAudioDetails("file does not exist"),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the audio find throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_audio_for_non_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findAudioDetails(TEXT_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the audio streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_avi_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_AVI);

    testAVIAudio(audioMetaData);
}

/**
 * Test to make sure that the audio streams of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_MKV);

    testMKVAudio(audioMetaData);
}

/**
 * Test to make sure that the audio streams of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_MP4);

    testMP4Audio(audioMetaData);
}

/**
 * Test to make sure that the audio streams of an ogv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_ogv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_OGV);

    testOGVAudio(audioMetaData);
}

/**
 * Test to make sure that the audio streams of an flv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_flv_audio )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_FLV);

    testFLVAudio(audioMetaData);
}

/**
 * Test to make sure that video can be found for all the test media files.
 */
BOOST_AUTO_TEST_CASE( test_find_video )
{
    (void) transcode::findVideoDetails(VIDEO_AVI);
    (void) transcode::findVideoDetails(VIDEO_MKV);
    (void) transcode::findVideoDetails(VIDEO_MP4);
    (void) transcode::findVideoDetails(VIDEO_OGV);
    (void) transcode::findVideoDetails(VIDEO_FLV);
}

/**
 * Test to make sure that the video find throws the correct
 * exception when an empty string is provided.
n*/
BOOST_AUTO_TEST_CASE( test_find_video_for_empty_string )
{
    BOOST_REQUIRE_THROW( (void) transcode::findVideoDetails(""),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the video find throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_video_for_invalid_media_file )
{
    BOOST_REQUIRE_THROW(
            (void) transcode::findVideoDetails("file does not exist"),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the video find throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_video_for_non_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findVideoDetails(TEXT_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the video streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_avi_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_AVI);

    testAVIVideo(videoMetaData);
}

/**
 * Test to make sure that the video streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_MKV);

    testMKVVideo(videoMetaData);
}

/**
 * Test to make sure that the video streams of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_MP4);

    testMP4Video(videoMetaData);
}

/**
 * Test to make sure that the video streams of an ogv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_ogv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_OGV);

    testOGVVideo(videoMetaData);
}

/**
 * Test to make sure that the video streams of an flv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_flv_video )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_FLV);

    testFLVVideo(videoMetaData);
}

/**
 * Test to make sure that the container can be found for all the test
 * media files.
 */
BOOST_AUTO_TEST_CASE( test_find_container )
{
    (void) transcode::findContainerDetails(VIDEO_AVI);
    (void) transcode::findContainerDetails(VIDEO_MKV);
    (void) transcode::findContainerDetails(VIDEO_MP4);
    (void) transcode::findContainerDetails(VIDEO_OGV);
    (void) transcode::findContainerDetails(VIDEO_FLV);
}

/**
 * Test to make sure that the container find throws the correct
 * exception when an empty string is provided.
n*/
BOOST_AUTO_TEST_CASE( test_find_container_for_empty_string )
{
    BOOST_REQUIRE_THROW( (void) transcode::findContainerDetails(""),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the container find throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_container_for_invalid_media_file )
{
    BOOST_REQUIRE_THROW(
            (void) transcode::findContainerDetails("file does not exist"),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the container find throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_find_container_for_non_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findContainerDetails(TEXT_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the container of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_avi_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_AVI);

    testAVIContainer(containerMetaData);
}

/**
 * Test to make sure that the subtitles of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_MKV);

    testMKVContainer(containerMetaData);
}

/**
 * Test to make sure that the subtitles of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_MP4);

    testMP4Container(containerMetaData);
}

/**
 * Test to make sure that the subtitles of an ogv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_ogv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_OGV);

    testOGVContainer(containerMetaData);
}

/**
 * Test to make sure that the subtitles of an flv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_flv_container )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_FLV);

    testFLVContainer(containerMetaData);
}

/**
 * Test to make sure that the all the test media files can be opened.
 */
BOOST_AUTO_TEST_CASE( test_open_media_files )
{
    (void) transcode::findMediaFileDetails(VIDEO_AVI);
    (void) transcode::findMediaFileDetails(VIDEO_MKV);
    (void) transcode::findMediaFileDetails(VIDEO_MP4);
    (void) transcode::findMediaFileDetails(VIDEO_OGV);
    (void) transcode::findMediaFileDetails(VIDEO_FLV);
}

/**
 * Test to make sure that the media file open throws the correct
 * exception when an empty string is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_empty_string )
{
    BOOST_REQUIRE_THROW( (void) transcode::findMediaFileDetails(""),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the media file open throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_invalid_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findMediaFileDetails(INVALID_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the media file open throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_non_media_file )
{
    BOOST_REQUIRE_THROW( (void) transcode::findMediaFileDetails(TEXT_FILE),
            transcode::UtilMediaException);
}

/**
 * Test to make sure that the details of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_avi_media_file_details )
{

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_AVI);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_AVI_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_AVI_SIZE);

    testAVIContainer(medaFile.container);
}

/**
 * Test to make sure that the details of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_media_file_details )
{

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_MKV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_MKV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_MKV_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, MKV_CONTAINER);

    testMKVSubtitles(medaFile.container.subtitleDetails);

    testMKVAudio(medaFile.container.audioDetails);

    testMKVVideo(medaFile.container.videoDetails);
}

/**
 * Test to make sure that the details of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_media_file_details )
{

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_MP4);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_MP4_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_MP4_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, MP4_CONTAINER);

    testMP4Subtitles(medaFile.container.subtitleDetails);

    testMP4Audio(medaFile.container.audioDetails);

    testMP4Video(medaFile.container.videoDetails);
}

/**
 * Test to make sure that the details of an ogv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_ogv_media_file_details )
{

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_OGV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_OGV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_OGV_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, OGV_CONTAINER);

    testOGVSubtitles(medaFile.container.subtitleDetails);

    testOGVAudio(medaFile.container.audioDetails);

    testOGVVideo(medaFile.container.videoDetails);
}

/**
 * Test to make sure that the details of an flv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_flv_media_file_details )
{

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_FLV);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_FLV_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_FLV_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, FLV_CONTAINER);

    testFLVSubtitles(medaFile.container.subtitleDetails);

    testFLVAudio(medaFile.container.audioDetails);

    testFLVVideo(medaFile.container.videoDetails);
}
