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
    (void) transcode::findSubtitleDetails(VIDEO_ONE);
    (void) transcode::findSubtitleDetails(VIDEO_TWO);
    (void) transcode::findSubtitleDetails(VIDEO_THREE);
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
     BOOST_REQUIRE_THROW( (void) transcode::findSubtitleDetails("file does not exist"),
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
BOOST_AUTO_TEST_CASE( test_find_avi_subtitles ) {

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_ONE);

      BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Test to make sure that the subtitles of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_subtitles ) {

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_TWO);

      BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Test to make sure that the subtitles of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_subtitles ) {

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::findSubtitleDetails(VIDEO_THREE);

      BOOST_CHECK_EQUAL( subtitleMetaData.size(), 0);
}

/**
 * Test to make sure that audio can be found for all the test media files.
 */
BOOST_AUTO_TEST_CASE( test_find_audio )
{
     (void) transcode::findAudioDetails(VIDEO_ONE);
     (void) transcode::findAudioDetails(VIDEO_TWO);
     (void) transcode::findAudioDetails(VIDEO_THREE);
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
     BOOST_REQUIRE_THROW( (void) transcode::findAudioDetails("file does not exist"),
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
BOOST_AUTO_TEST_CASE( test_find_avi_audio ) {

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_ONE);

      BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
      BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}

/**
 * Test to make sure that the audio streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_audio ) {

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_TWO);

      BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
      BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}

/**
 * Test to make sure that the audio streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_audio ) {

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::findAudioDetails(VIDEO_THREE);

      BOOST_CHECK_EQUAL( audioMetaData.size(), 1);
      BOOST_CHECK_EQUAL( audioMetaData[0].mimeType, MPG_AUDIO);
}

/**
 * Test to make sure that video can be found for all the test media files.
 */
BOOST_AUTO_TEST_CASE( test_find_video )
{
    (void) transcode::findVideoDetails(VIDEO_ONE);
    (void) transcode::findVideoDetails(VIDEO_TWO);
    (void) transcode::findVideoDetails(VIDEO_THREE);
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
     BOOST_REQUIRE_THROW( (void) transcode::findVideoDetails("file does not exist"),
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
BOOST_AUTO_TEST_CASE( test_find_avi_video ) {

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_ONE);

      BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
      BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, DIVX_VIDEO);
}

/**
 * Test to make sure that the video streams of an avi file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_video ) {

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_TWO);

      BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
      BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, H264_VIDEO);
}

/**
 * Test to make sure that the video streams of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_video ) {

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::findVideoDetails(VIDEO_THREE);

      BOOST_CHECK_EQUAL( videoMetaData.size(), 1);
      BOOST_CHECK_EQUAL( videoMetaData[0].mimeType, H264_VIDEO);
}

/**
 * Test to make sure that the container can be found for all the test
 * media files.
 */
BOOST_AUTO_TEST_CASE( test_find_container )
{
    (void) transcode::findContainerDetails(VIDEO_ONE);
    (void) transcode::findContainerDetails(VIDEO_TWO);
    (void) transcode::findContainerDetails(VIDEO_THREE);
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
     BOOST_REQUIRE_THROW( (void) transcode::findContainerDetails("file does not exist"),
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
BOOST_AUTO_TEST_CASE( test_find_avi_container ) {

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_ONE);

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, AVI_CONTAINER);

    BOOST_CHECK_EQUAL( containerMetaData.subtitleDetails.size(), 0);

    BOOST_CHECK_EQUAL( containerMetaData.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( containerMetaData.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.videoDetails[0].mimeType, DIVX_VIDEO);
}

/**
 * Test to make sure that the subtitles of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_container ) {

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_TWO);

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, MKV_CONTAINER);

    BOOST_CHECK_EQUAL( containerMetaData.subtitleDetails.size(), 0);

    BOOST_CHECK_EQUAL( containerMetaData.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( containerMetaData.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.videoDetails[0].mimeType, H264_VIDEO);
}

/**
 * Test to make sure that the subtitles of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_container ) {

    transcode::ContainerMetaData containerMetaData =
            transcode::findContainerDetails(VIDEO_THREE);

    BOOST_CHECK_EQUAL( containerMetaData.mimeType, MP4_CONTAINER);

    BOOST_CHECK_EQUAL( containerMetaData.subtitleDetails.size(), 0);

    BOOST_CHECK_EQUAL( containerMetaData.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( containerMetaData.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( containerMetaData.videoDetails[0].mimeType, H264_VIDEO);
}

/**
 * Test to make sure that the all the test media files can be opened.
 */
BOOST_AUTO_TEST_CASE( test_open_media_files )
{
    (void) transcode::findMediaFileDetails(VIDEO_ONE);
    (void) transcode::findMediaFileDetails(VIDEO_TWO);
    (void) transcode::findMediaFileDetails(VIDEO_THREE);
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
    BOOST_REQUIRE_THROW( (void) transcode::findMediaFileDetails("file does not exist"),
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
BOOST_AUTO_TEST_CASE( test_find_avi_media_file_details ) {

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_ONE);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_ONE_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_ONE_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, AVI_CONTAINER);

    BOOST_CHECK_EQUAL( medaFile.container.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( medaFile.container.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.videoDetails[0].mimeType, DIVX_VIDEO);
}

/**
 * Test to make sure that the details of an mkv file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mkv_media_file_details ) {

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_TWO);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_TWO_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_TWO_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, MKV_CONTAINER);

    BOOST_CHECK_EQUAL( medaFile.container.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( medaFile.container.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.videoDetails[0].mimeType,
            H264_VIDEO);
}

/**
 * Test to make sure that the details of an mp4 file can be found.
 */
BOOST_AUTO_TEST_CASE( test_find_mp4_media_file_details ) {

    transcode::MediaFileMetaData medaFile = transcode::findMediaFileDetails(
            VIDEO_THREE);

    BOOST_CHECK_EQUAL( medaFile.name, VIDEO_THREE_NAME);

    BOOST_CHECK_EQUAL( medaFile.size, VIDEO_THREE_SIZE);

    BOOST_CHECK_EQUAL( medaFile.container.mimeType, MP4_CONTAINER);

    BOOST_CHECK_EQUAL( medaFile.container.audioDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.audioDetails[0].mimeType, MPG_AUDIO);

    BOOST_CHECK_EQUAL( medaFile.container.videoDetails.size(), 1);
    BOOST_CHECK_EQUAL( medaFile.container.videoDetails[0].mimeType,
            H264_VIDEO);
}
