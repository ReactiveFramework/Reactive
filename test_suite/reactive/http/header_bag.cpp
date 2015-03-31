/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/header_bag.hpp>
#include <reactive/http/header.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_header_bag)

BOOST_AUTO_TEST_CASE(test_has)
{
    reactive::http::header_bag hb;
    BOOST_CHECK_EQUAL(hb.has("Host"), false);
}

BOOST_AUTO_TEST_CASE(test_add)
{
    reactive::http::header_bag hb;
    BOOST_CHECK_EQUAL(hb.has("Host"), false);

    hb.add("Host", "www.google.com");
    BOOST_CHECK_EQUAL(hb.has("Host"), true);
    BOOST_CHECK_EQUAL(hb.get("Host").value, "www.google.com");

    reactive::http::header_t h = {"Server", "Reactive/1.0"};

    hb.add(h);
    BOOST_CHECK_EQUAL(hb.has("Server"), true);
    BOOST_CHECK_EQUAL(hb.get("Server").value, "Reactive/1.0");
}

BOOST_AUTO_TEST_CASE(test_override)
{
    reactive::http::header_bag hb;
    BOOST_CHECK_EQUAL(hb.has("Host"), false);

    hb.add("Host", "www.google.com");
    BOOST_CHECK_EQUAL(hb.has("Host"), true);
    BOOST_CHECK_EQUAL(hb.get("Host").value, "www.google.com");

    hb.add("Host", "www.google.fr");
    BOOST_CHECK_EQUAL(hb.has("Host"), true);
    BOOST_CHECK_EQUAL(hb.get("Host").value, "www.google.fr");
}

BOOST_AUTO_TEST_CASE(test_get)
{
    reactive::http::header_bag hb;
    BOOST_CHECK_EQUAL(hb.has("Host"), false);

    BOOST_CHECK_THROW(hb.get("Host"), std::runtime_error);

    hb.add("Host", "www.google.com");

    BOOST_CHECK_EQUAL(hb.has("Host"), true);
    reactive::http::header_t h;
    h = hb.get("Host");

    BOOST_CHECK_EQUAL(h.value, "www.google.com");

    h = hb.get("Host");

    BOOST_CHECK_EQUAL(reactive::http::header::to_string(h), "Host: www.google.com");
}

BOOST_AUTO_TEST_CASE(test_del)
{
    reactive::http::header_bag hb;
    BOOST_CHECK_EQUAL(hb.has("Host"), false);

    hb.add("Host", "www.google.com");
    BOOST_CHECK_EQUAL(hb.has("Host"), true);

    BOOST_CHECK_EQUAL(hb.del("Host"), true);
    BOOST_CHECK_EQUAL(hb.has("Host"), false);
}

BOOST_AUTO_TEST_SUITE_END()
