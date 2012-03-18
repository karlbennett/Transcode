#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_file.hpp>

#include <util_test.hpp>

/**
 * Test that a normal file can be opened.
 */
BOOST_AUTO_TEST_CASE( test_util_file )
{

    transcode::util::File file(TEXT_FILE);

	BOOST_CHECK_EQUAL( file.getPath(), TEXT_FILE );
	BOOST_CHECK_EQUAL( file.getName(), TEXT_NAME );
	BOOST_CHECK_EQUAL( file.getSize(), 25 );
}

/**
 * Test that trying to open an invalid file throws
 * an exception.
 */
BOOST_AUTO_TEST_CASE( test_util_file_on_invalid_file )
{

    BOOST_REQUIRE_THROW(
            transcode::util::File file(INVALID_FILE),
            transcode::util::FileException );
}
