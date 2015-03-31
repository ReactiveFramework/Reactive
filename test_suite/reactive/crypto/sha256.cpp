/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/sha256.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_sha256)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(
        reactive::crypto::sha256("hello world"),
        "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9"
    );
}

BOOST_AUTO_TEST_SUITE_END()
