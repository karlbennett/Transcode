#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_ffmpeg.hpp>
#include <util_test.hpp>

class TestObject {};

/**
 * Test to make sure that an FFMPEG error message can be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message )
{

	BOOST_REQUIRE( transcode::utils::UNKNOWN !=
	        transcode::utils::ffmpegErrorMessage(0));
}

/**
 * Test to make sure that a format context can be found for all
 * the video files.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context )
{

    (void) transcode::utils::retrieveAVFormatContext(VIDEO_ONE);
    (void) transcode::utils::retrieveAVFormatContext(VIDEO_TWO);
    (void) transcode::utils::retrieveAVFormatContext(VIDEO_THREE);
}

/**
 * Test to make sure that a format context can not be found from an
 * empty string.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_string )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(""),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that a format context can not be found from an
 * invalid file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_invalid_file )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(INVALID_FILE),
            transcode::utils::FFMPEGException);
}

/**
 * Test to make sure that a format context can not be found from a
 * text file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_non_media_file )
{

    BOOST_REQUIRE_THROW( transcode::utils::retrieveAVFormatContext(TEXT_FILE),
            transcode::utils::FFMPEGException);
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_subtitles )
{

    // Example test for exception.
    BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

    // Example test equality.
    BOOST_CHECK_EQUAL( 1, 1 );
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_audio )
{

    // Example test for exception.
    BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

    // Example test equality.
    BOOST_CHECK_EQUAL( 1, 1 );
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_extract_video )
{

    // Example test for exception.
    BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

    // Example test equality.
    BOOST_CHECK_EQUAL( 1, 1 );
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_build_container )
{

    // Example test for exception.
    BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

    // Example test equality.
    BOOST_CHECK_EQUAL( 1, 1 );
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs )
{

    // Example test for exception.
    BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

    // Example test equality.
    BOOST_CHECK_EQUAL( 1, 1 );
}
