/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/net/port.hpp>

BOOST_AUTO_TEST_SUITE(reactive_net_port)

BOOST_AUTO_TEST_CASE(test_is_valid)
{
    std::string test_port = "8080";

    BOOST_CHECK_EQUAL(reactive::net::port::is_valid(""),          false);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid(test_port),   true);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid("80"),        true);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid("65536"),     false);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid(8080),        true);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid(80),          true);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid(65536),       false);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid("test"),      false);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid("80a"),       false);
    BOOST_CHECK_EQUAL(reactive::net::port::is_valid("655364"),    false);
}


BOOST_AUTO_TEST_SUITE_END()
