#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_standard.hpp>

#include <string>
#include <map>

using namespace std;
using namespace transcode::util;

const string ONE = "one";
const string TWO = "two";
const string THREE = "three";
const string FOUR = "four";
const string FIVE = "five";

static map<int, string> initialiseTestMap() {

	map<int, string> testMap;

	testMap[1] = ONE;
	testMap[2] = TWO;
	testMap[3] = THREE;
	testMap[4] = FOUR;
	testMap[5] = FIVE;

	return testMap;
}

const map<int, string> TEST_MAP = initialiseTestMap();

/**
 * Test that the standard utils get function works for valid entries.
 */
BOOST_AUTO_TEST_CASE( test_map_get_success )
{

	BOOST_CHECK_EQUAL( get(TEST_MAP, 1), ONE );
	BOOST_CHECK_EQUAL( get(TEST_MAP, 2), TWO );
	BOOST_CHECK_EQUAL( get(TEST_MAP, 3), THREE );
	BOOST_CHECK_EQUAL( get(TEST_MAP, 4), FOUR );
	BOOST_CHECK_EQUAL( get(TEST_MAP, 5), FIVE );
}

/**
 * Test that the standard utils get function works for invalid entries.
 */
BOOST_AUTO_TEST_CASE( test_map_get_failure )
{

	BOOST_CHECK_EQUAL( get(TEST_MAP, 6), string() );
}
