/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/uri/query.hpp>

BOOST_AUTO_TEST_SUITE(reactive_uri_query)

BOOST_AUTO_TEST_CASE(test_query)
{
    std::string query_str = "foo=bar&q=hello&ads[test]=123&ads[test1]=456&keywords[]=key1&keywords[]=key2&empty";

    reactive::uri::query query(query_str);

    BOOST_CHECK_EQUAL(query.size(), 6);

    BOOST_CHECK_EQUAL(query.has("foo"), true);
    BOOST_CHECK_EQUAL(query.has("foo2"), false);
    BOOST_CHECK_EQUAL(query.has("empty"), true);

    BOOST_CHECK_EQUAL(query["foo"], "bar");
    BOOST_CHECK_EQUAL(query["q"], "hello");
    BOOST_CHECK_EQUAL(query["empty"], "");

    //BOOST_CHECK_THROW(query["foo2"], std::out_of_range);
    /*BOOST_CHECK_EQUAL(query["ads"].size(), 2);
    BOOST_CHECK_EQUAL(query["ads"]["test"], "123");
    BOOST_CHECK_EQUAL(query["ads"]["test1"], "456");
    BOOST_CHECK_EQUAL(query["keywords"].size(), 2);
    BOOST_CHECK_EQUAL(query["keywords"][0], "key1");
    BOOST_CHECK_EQUAL(query["keywords"][1], "key2");*/
}

BOOST_AUTO_TEST_CASE(test_to_string)
{
    std::string query_str = "foo=bar&q=hello&test%201=foo";

    reactive::uri::query query(query_str);
    BOOST_CHECK_EQUAL(query["test 1"], "foo");
    BOOST_CHECK_EQUAL(query.toString(), query_str);
}

BOOST_AUTO_TEST_SUITE_END()
