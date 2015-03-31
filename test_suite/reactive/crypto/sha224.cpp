/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/sha224.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_sha224)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(
        reactive::crypto::sha224("hello world"),
        "2f05477fc24bb4faefd86517156dafdecec45b8ad3cf2522a563582b"
    );
}

BOOST_AUTO_TEST_SUITE_END()
