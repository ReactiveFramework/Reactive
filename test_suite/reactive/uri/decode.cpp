/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/uri/decode.hpp>

BOOST_AUTO_TEST_SUITE(reactive_uri_decode)

BOOST_AUTO_TEST_CASE(test_decoder)
{
    BOOST_CHECK_EQUAL(reactive::uri::decode("123%20%235%20M%26M%20Street"), "123 #5 M&M Street");
}

BOOST_AUTO_TEST_SUITE_END()
