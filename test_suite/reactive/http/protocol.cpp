/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/protocol.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_protocol)

BOOST_AUTO_TEST_CASE(test_crlf)
{
    BOOST_CHECK_EQUAL(reactive::http::protocol::CRLF, "\r\n");
}

BOOST_AUTO_TEST_CASE(test_version)
{
    BOOST_CHECK_EQUAL(reactive::http::protocol::VERSION_10, "1.0");
    BOOST_CHECK_EQUAL(reactive::http::protocol::VERSION_11, "1.1");
}

BOOST_AUTO_TEST_CASE(test_const)
{
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_HEAD,     "HEAD");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_GET,      "GET");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_POST,     "POST");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_PUT,      "PUT");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_DELETE,   "DELETE");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_OPTIONS,  "OPTIONS");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_TRACE,    "TRACE");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_CONNECT,  "CONNECT");
    BOOST_CHECK_EQUAL(reactive::http::protocol::METHOD_PATCH,    "PATCH");
}

BOOST_AUTO_TEST_SUITE_END()
