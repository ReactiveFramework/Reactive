/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/whirlpool.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_whirlpool)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(
        reactive::crypto::whirlpool("hello world"),
        "8d8309ca6af848095bcabaf9a53b1b6ce7f594c1434fd6e5177e7e5c20e76cd30936d8606e7f36acbef8978fea008e6400a975d51abe6ba4923178c7cf90c802"
    );
}

BOOST_AUTO_TEST_SUITE_END()
