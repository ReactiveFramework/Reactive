/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/exception.hpp>
#include <reactive/http/status.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_exception)

BOOST_AUTO_TEST_CASE(test_http_status_code)
{
    reactive::http::exception ex("Bad Request", reactive::http::status::BAD_REQUEST);

    BOOST_CHECK_EQUAL(ex.what(), "Bad Request");
    BOOST_CHECK_EQUAL(ex.getHttpStatusCode(), reactive::http::status::BAD_REQUEST);
}


BOOST_AUTO_TEST_SUITE_END()
