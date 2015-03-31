/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/sha1.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_sha1)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(reactive::crypto::sha1("hello world"), "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
}

BOOST_AUTO_TEST_SUITE_END()
