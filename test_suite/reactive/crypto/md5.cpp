/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/md5.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_md5)

BOOST_AUTO_TEST_CASE(test_hash)
{
    BOOST_CHECK_EQUAL(reactive::crypto::md5("hello world"), "5eb63bbbe01eeed093cb22bb8f5acdc3");
}

BOOST_AUTO_TEST_SUITE_END()
