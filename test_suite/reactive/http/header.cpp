/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/header.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_header)

BOOST_AUTO_TEST_CASE(test_header)
{
    reactive::http::header_t header_server = {
        "Server",
        "Reactive/1.0"
    };

    BOOST_CHECK_EQUAL(header_server.name, "Server");
    BOOST_CHECK_EQUAL(header_server.value, "Reactive/1.0");
}

BOOST_AUTO_TEST_CASE(test_to_string)
{
    reactive::http::header_t header_server = {
        "Server",
        "Reactive/1.0"
    };

    BOOST_CHECK_EQUAL(reactive::http::header::to_string(header_server), "Server: Reactive/1.0");
}

BOOST_AUTO_TEST_CASE(test_from_string)
{
    reactive::http::header_t header;

    header = reactive::http::header::from_string("Server: Reactive/1.0.0\r\n");

    BOOST_CHECK_EQUAL(header.name, "Server");
    BOOST_CHECK_EQUAL(header.value, "Reactive/1.0.0");
}


BOOST_AUTO_TEST_SUITE_END()
