#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

class TestObject {};

/**
 * Test example.
 */
BOOST_AUTO_TEST_CASE( test_open_media_file )
{

	// Example test for exception.
	BOOST_REQUIRE_THROW( throw TestObject(), TestObject );

	// Example test equality.
	BOOST_CHECK_EQUAL( 1, 1 );
}
