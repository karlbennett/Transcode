#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_open_media_file )
{
    BOOST_CHECK( 2 == 1 );
}

BOOST_AUTO_TEST_CASE( test_open_null_media_file )
{
    BOOST_CHECK( 2 == 1 );
}

BOOST_AUTO_TEST_CASE( test_open_non_media_file )
{
    BOOST_CHECK( 2 == 1 );
}

BOOST_AUTO_TEST_CASE( test_media_file_properties )
{
    int i = 0;

    // reports 'error in "test2": check i == 2 failed [0 != 2]'
    BOOST_CHECK_EQUAL( i, 2 );

    BOOST_CHECK_EQUAL( i, 0 );
}