/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/sha512.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_sha512)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(
        reactive::crypto::sha512("hello world"),
        "309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f"
    );
}

BOOST_AUTO_TEST_SUITE_END()
