/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/crypto/base64.hpp>

BOOST_AUTO_TEST_SUITE(reactive_crypto_base64)

BOOST_AUTO_TEST_CASE(test_encode)
{
    std::string input_msg = "Hello World";

    BOOST_CHECK_EQUAL(reactive::crypto::base64::encode(""), "");

    BOOST_CHECK_EQUAL(reactive::crypto::base64::encode(input_msg), "SGVsbG8gV29ybGQ=");

    BOOST_CHECK_EQUAL(reactive::crypto::base64::encode("Hello World"), "SGVsbG8gV29ybGQ=");
}

BOOST_AUTO_TEST_CASE(test_decode)
{
    std::string input_msg = "SGVsbG8gV29ybGQ=";

    BOOST_CHECK_EQUAL(reactive::crypto::base64::decode(input_msg), "Hello World");

    BOOST_CHECK_EQUAL(reactive::crypto::base64::decode("SGVsbG8gV29ybGQ="), "Hello World");
}

BOOST_AUTO_TEST_SUITE_END()
