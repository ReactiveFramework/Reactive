/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/uri/url.hpp>

BOOST_AUTO_TEST_SUITE(reactive_uri_url)

BOOST_AUTO_TEST_CASE(test_url_empty)
{
    reactive::uri::url url("");

    BOOST_CHECK_EQUAL(url.isEmpty(), true);

    url.setHost("");

    BOOST_CHECK_EQUAL(url.isEmpty(), true);
}

BOOST_AUTO_TEST_CASE(test_getter)
{
    reactive::uri::url url("http://user:pass@www.google.com/search?q=test#top");

    BOOST_CHECK_EQUAL(url.getScheme(), "http");

    BOOST_CHECK_EQUAL(url.getUser(), "user");

    BOOST_CHECK_EQUAL(url.getPassword(), "pass");

    BOOST_CHECK_EQUAL(url.getHost(), "www.google.com");

    BOOST_CHECK_EQUAL(url.getPort(), 80);

    BOOST_CHECK_EQUAL(url.getPath(), "/search");

    BOOST_CHECK_EQUAL(url.getQuery(), "q=test");

    BOOST_CHECK_EQUAL(url.getFragment(), "top");

    BOOST_CHECK_EQUAL(url.toString(), "http://user:pass@www.google.com/search?q=test#top");
}

BOOST_AUTO_TEST_CASE(test_setter)
{
    reactive::uri::url url;

    url.setScheme("https");

    BOOST_CHECK_EQUAL(url.getScheme(), "https");

    url.setUser("euskadi31");

    BOOST_CHECK_EQUAL(url.getUser(), "euskadi31");

    url.setPassword("123456");

    BOOST_CHECK_EQUAL(url.getPassword(), "123456");

    url.setHost("www.dacteev.com");

    BOOST_CHECK_EQUAL(url.getHost(), "www.dacteev.com");

    url.setPort(443);

    BOOST_CHECK_EQUAL(url.getPort(), 443);

    url.setPath("test");

    BOOST_CHECK_EQUAL(url.getPath(), "/test");

    url.setPath("/");

    BOOST_CHECK_EQUAL(url.getPath(), "/");

    url.setQuery("lang=en");

    BOOST_CHECK_EQUAL(url.getQuery(), "lang=en");

    url.setFragment("top");

    BOOST_CHECK_EQUAL(url.getFragment(), "top");


    BOOST_CHECK_EQUAL(url.toString(), "https://euskadi31:123456@www.dacteev.com/?lang=en#top");
}

BOOST_AUTO_TEST_CASE(test_https_port)
{
    reactive::uri::url url;

    url.setScheme("https");

    BOOST_CHECK_EQUAL(url.getPort(), 443);

    url.setScheme("http");

    BOOST_CHECK_EQUAL(url.getPort(), 80);

    reactive::uri::url url1("https://www.google.com/");

    BOOST_CHECK_EQUAL(url1.getScheme(), "https");

    BOOST_CHECK_EQUAL(url1.getHost(), "www.google.com");

    BOOST_CHECK_EQUAL(url1.getPort(), 443);
}

BOOST_AUTO_TEST_CASE(test_invalid_port)
{
    BOOST_CHECK_THROW(reactive::uri::url("http://www.google.com:2836535"), std::invalid_argument);

    /*reactive::uri::url url("http://www.google.com:2836535");

    BOOST_CHECK_EQUAL(url.getPort(), 80);

    reactive::uri::url url1("https://www.google.com:2836535");

    BOOST_CHECK_EQUAL(url1.getPort(), 443);*/
}

BOOST_AUTO_TEST_CASE(test_getPath)
{
    reactive::uri::url url("http://www.google.com");

    BOOST_CHECK_EQUAL(url.getPath(), "/");
}


BOOST_AUTO_TEST_CASE(test_toString)
{
    reactive::uri::url url("http://user:pass@www.google.com/search?q=test#top");

    BOOST_CHECK_EQUAL(url.toString(reactive::uri::url::PATH | reactive::uri::url::QUERY), "/search?q=test");

    url.setPort(8080);

    BOOST_CHECK_EQUAL(url.toString(), "http://user:pass@www.google.com:8080/search?q=test#top");
}

BOOST_AUTO_TEST_SUITE_END()
