#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <media.hpp>

class TestObject {};

/**
 * Test example.
 */
BOOST_AUTO_TEST_CASE( test_example )
{

    transcode::Frame frame;
    transcode::AudioFrame audioFrame;
    transcode::VideoFrame videoFrame;

	// Example test for exception.
	BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

	// Example test equality.
	BOOST_CHECK_EQUAL( 1, 1 );
}
