#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <util/util_media.hpp>
#include <metadata.hpp>
#include <TestUtils.h>

/**
* Test to make sure that the GstreamerMediaFile can open all the test media files.
*/
BOOST_AUTO_TEST_CASE( test_open_media_files )
{
	transcode::MediaFileMetaData medaFile1 = transcode::findMediaFileDetails(
				VIDEO_ONE);
	transcode::MediaFileMetaData medaFile2 = transcode::findMediaFileDetails(
					VIDEO_TWO);
	transcode::MediaFileMetaData medaFile3 = transcode::findMediaFileDetails(
					VIDEO_THREE);
}

/**
* Test to make sure that the GstreamerMediaFile throws the correct
* exception when an empty string is provided.
*/
BOOST_AUTO_TEST_CASE( test_open_empty_string )
{
	BOOST_REQUIRE_THROW( transcode::findMediaFileDetails(""),
			transcode::UtilMediaException );
}

/**
* Test to make sure that the GstreamerMediaFile throws the correct
* exception when an in valid file name is provided.
*/
BOOST_AUTO_TEST_CASE( test_open_invalid_media_file )
{
	BOOST_REQUIRE_THROW( transcode::findMediaFileDetails("file does not exist"),
				transcode::UtilMediaException );
}

/**
* Test to make sure that the GstreamerMediaFile throws the correct
* exception when a non-media file is provided.
*/
BOOST_AUTO_TEST_CASE( test_open_non_media_file )
{
	BOOST_REQUIRE_THROW( transcode::findMediaFileDetails(TEXT_FILE),
					transcode::UtilMediaException );
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
