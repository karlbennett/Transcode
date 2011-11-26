#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <media/io/gstreamer/GstreamerMediaFile.h>
#include <media/io/exception/MediaFileException.h>
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

/**
 * Test to make sure that the GstreamerMediaFile can open all the test media files.
 */
BOOST_AUTO_TEST_CASE( test_open_media_file )
{
	transcode::GstreamerMediaFile mediafile1(VIDEO_ONE);
	transcode::GstreamerMediaFile mediafile2(VIDEO_TWO);
	transcode::GstreamerMediaFile mediafile3(VIDEO_THREE);

	BOOST_TEST_MESSAGE( "media file creation failed." );
}

/**
 * Test to make sure that the GstreamerMediaFile throws the correct
 * exception when an empty string is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_empty_string_media_file )
{
	BOOST_REQUIRE_THROW( transcode::GstreamerMediaFile mediafile(""),
			transcode::MediaFileException );
}

/**
 * Test to make sure that the GstreamerMediaFile throws the correct
 * exception when an in valid file name is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_invalid_media_file )
{
	BOOST_REQUIRE_THROW( transcode::GstreamerMediaFile mediafile("file does not exists"),
			transcode::MediaFileException );
}

/**
 * Test to make sure that the GstreamerMediaFile throws the correct
 * exception when a non-media file is provided.
 */
BOOST_AUTO_TEST_CASE( test_open_non_media_file )
{
	transcode::GstreamerMediaFile mediafile(TEXT_FILE);
}

/**
 * Test to make sure that the GstreamerMediaFile finds the correct
 * properties for the test media files.
 */
BOOST_AUTO_TEST_CASE( test_media_file_properties )
{
	transcode::GstreamerMediaFile mediafile1(VIDEO_ONE);

	BOOST_CHECK_EQUAL( mediafile1.getFileUri(), VIDEO_ONE );

	BOOST_CHECK_EQUAL( mediafile1.getMediaContainer().getMimeType(),
			AVI_CONTAINER );

	BOOST_CHECK_EQUAL( mediafile1.getAudioStreamSize(), 1 );
	BOOST_CHECK_EQUAL( mediafile1.getAudioStream(0).getMimeType(),
			MPG_AUDIO );

	BOOST_CHECK_EQUAL( mediafile1.getVideoStreamSize(), 1);
	BOOST_CHECK_EQUAL( mediafile1.getVideoStream(0).getMimeType(),
			DIVX_VIDEO);

	transcode::GstreamerMediaFile mediafile2(VIDEO_TWO);

	BOOST_CHECK_EQUAL( mediafile2.getFileUri(), VIDEO_TWO );

	BOOST_CHECK_EQUAL( mediafile2.getMediaContainer().getMimeType(),
			MKV_CONTAINER );

	BOOST_CHECK_EQUAL( mediafile2.getAudioStreamSize(), 1 );
	BOOST_CHECK_EQUAL( mediafile2.getAudioStream(0).getMimeType(),
			MPG_AUDIO );

	BOOST_CHECK_EQUAL( mediafile2.getVideoStreamSize(), 1 );
	BOOST_CHECK_EQUAL( mediafile2.getVideoStream(0).getMimeType(),
			H264_VIDEO );

	transcode::GstreamerMediaFile mediafile3(VIDEO_THREE);

	BOOST_CHECK_EQUAL( mediafile3.getFileUri(), VIDEO_THREE );

	BOOST_CHECK_EQUAL( mediafile3.getMediaContainer().getMimeType(),
			MP4_CONTAINER );

	BOOST_CHECK_EQUAL( mediafile3.getAudioStreamSize(), 1 );
	BOOST_CHECK_EQUAL( mediafile3.getAudioStream(0).getMimeType(),
			MPG_AUDIO );

	BOOST_CHECK_EQUAL( mediafile3.getVideoStreamSize(), 1 );
	BOOST_CHECK_EQUAL( mediafile3.getVideoStream(0).getMimeType(),
			H264_VIDEO );
}
