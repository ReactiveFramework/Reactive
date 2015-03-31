/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/net/ip.hpp>

BOOST_AUTO_TEST_SUITE(reactive_net_ip)


BOOST_AUTO_TEST_CASE(test_undefined_is_valid)
{
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("1.2.3.4", reactive::net::ip::UNDEFINED),
        false
    );
}

BOOST_AUTO_TEST_CASE(test_is_v4)
{
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("1.2.3.4"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("10.0.0.1"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("255.255.255.255"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("0.0.0.256"), false);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("1.2.3.4.5"), false);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("a:b:c:d:e::1.2.3.4"), false);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v4("v1.09azAZ-._~!$&'()*+,;="), false);
}

BOOST_AUTO_TEST_CASE(test_is_v6)
{
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v6("1.2.3.4"), false);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v6("a:b:c:d:e::1.2.3.4"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_v6("v1.09azAZ-._~!$&'()*+,;="), false);
}

BOOST_AUTO_TEST_CASE(test_ipv4_is_valid)
{
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("1.2.3.4", reactive::net::ip::IPV4),
        true
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("10.0.0.1", reactive::net::ip::IPV4),
        true
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("255.255.255.255", reactive::net::ip::IPV4),
        true
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("0.0.0.256", reactive::net::ip::IPV4),
        false
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("1.2.3.4.5", reactive::net::ip::IPV4),
        false
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("a:b:c:d:e::1.2.3.4", reactive::net::ip::IPV4),
        false
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("v1.09azAZ-._~!$&'()*+,;=", reactive::net::ip::IPV4),
        false
    );
}

BOOST_AUTO_TEST_CASE(test_ipv6_is_valid)
{
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("1.2.3.4", reactive::net::ip::IPV6),
        false
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("a:b:c:d:e::1.2.3.4", reactive::net::ip::IPV6),
        true
    );
    BOOST_CHECK_EQUAL(
        reactive::net::ip::is_valid("v1.09azAZ-._~!$&'()*+,;=", reactive::net::ip::IPV6),
        false
    );
}

BOOST_AUTO_TEST_CASE(test_is_valid)
{
    BOOST_CHECK_EQUAL(reactive::net::ip::is_valid("a:b:c:d:e::1.2.3.4"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_valid("10.0.0.1"), true);
    BOOST_CHECK_EQUAL(reactive::net::ip::is_valid("v1.09azAZ-._~!$&'()*+,;="), false);
}


BOOST_AUTO_TEST_SUITE_END()
