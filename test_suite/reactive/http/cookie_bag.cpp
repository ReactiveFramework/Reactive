/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/cookie_bag.hpp>
#include <reactive/http/cookie.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_cookie_bag)


BOOST_AUTO_TEST_CASE(test_has)
{
    reactive::http::cookie_bag cb;
    BOOST_CHECK_EQUAL(cb.has("foo"), false);
}

BOOST_AUTO_TEST_CASE(test_add)
{
    reactive::http::cookie_bag cb;
    BOOST_CHECK_EQUAL(cb.has("foo"), false);

    cb.add("foo", "bar");
    BOOST_CHECK_EQUAL(cb.has("foo"), true);

    reactive::http::cookie_t c;
    c.name = "foo1";
    c.value = "bar1";

    cb.add(c);
    BOOST_CHECK_EQUAL(cb.has("foo1"), true);
}

BOOST_AUTO_TEST_CASE(test_get)
{
    reactive::http::cookie_bag cb;
    BOOST_CHECK_EQUAL(cb.has("foo"), false);

    BOOST_CHECK_THROW(cb.get("foo"), std::runtime_error);

    cb.add("foo", "bar");
    BOOST_CHECK_EQUAL(cb.has("foo"), true);

    reactive::http::cookie_t c;
    c = cb.get("foo");

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(c), "foo=bar");
}

BOOST_AUTO_TEST_CASE(test_del)
{
    reactive::http::cookie_bag cb;
    BOOST_CHECK_EQUAL(cb.has("foo"), false);

    cb.add("foo", "bar");
    BOOST_CHECK_EQUAL(cb.has("foo"), true);

    BOOST_CHECK_EQUAL(cb.del("foo"), true);
    BOOST_CHECK_EQUAL(cb.has("foo"), false);
}


BOOST_AUTO_TEST_SUITE_END()
