/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/cookie.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_cookie)

BOOST_AUTO_TEST_CASE(test_cookie_default)
{
    reactive::http::cookie_t user_cookie;

    BOOST_CHECK_EQUAL(user_cookie.name.empty(), true);
    BOOST_CHECK_EQUAL(user_cookie.value,        "");
    BOOST_CHECK_EQUAL(user_cookie.domain,       "");
    BOOST_CHECK_EQUAL(user_cookie.expires,      0);
    BOOST_CHECK_EQUAL(user_cookie.max_age,      0);
    BOOST_CHECK_EQUAL(user_cookie.path,         "");
    BOOST_CHECK_EQUAL(user_cookie.secure,       false);
    BOOST_CHECK_EQUAL(user_cookie.http_only,    false);
}

BOOST_AUTO_TEST_CASE(test_cookie)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name = "user";
    user_cookie.value = "euskadi31";

    BOOST_CHECK_EQUAL(user_cookie.name, "user");
    BOOST_CHECK_EQUAL(user_cookie.value, "euskadi31");
    BOOST_CHECK_EQUAL(user_cookie.max_age, 0);
    BOOST_CHECK_EQUAL(user_cookie.domain, "");
    BOOST_CHECK_EQUAL(user_cookie.secure, false);
    BOOST_CHECK_EQUAL(user_cookie.http_only, false);

    user_cookie.domain = "google.com";
    user_cookie.secure = true;

    BOOST_CHECK_EQUAL(user_cookie.domain, "google.com");
    BOOST_CHECK_EQUAL(user_cookie.secure, true);
    BOOST_CHECK_EQUAL(user_cookie.http_only, false);
}

BOOST_AUTO_TEST_CASE(test_to_string)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name = "user";
    user_cookie.value = "euskadi31";

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31");

    user_cookie.http_only = true;

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31; HttpOnly");

    // reset to default value
    user_cookie.http_only = false;

    user_cookie.name = "user";
    user_cookie.value = "";

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=deleted; Expires=Thu, 01-Jan-1970 00:00:00 GMT");
}

BOOST_AUTO_TEST_CASE(test_expires)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name = "user";
    user_cookie.value = "euskadi31";
    time(&user_cookie.expires);
    user_cookie.expires = (user_cookie.expires + 86400);

    std::string expected = "user=euskadi31; Expires=";

    struct tm * timeinfo;
    char date[35];
    timeinfo = gmtime(&user_cookie.expires);
    strftime(date, 35, "%a, %d-%b-%Y %H:%M:%S %Z", timeinfo);

    expected.append(std::string(date));

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), expected);
}

BOOST_AUTO_TEST_CASE(test_max_age)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name    = "user";
    user_cookie.value   = "euskadi31";
    user_cookie.max_age = 3600;

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31; Max-Age=3600");
}

BOOST_AUTO_TEST_CASE(test_path)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name    = "user";
    user_cookie.value   = "euskadi31";
    user_cookie.path    = "/";

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31; Path=/");
}

BOOST_AUTO_TEST_CASE(test_domain)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name    = "user";
    user_cookie.value   = "euskadi31";
    user_cookie.path    = "/";
    user_cookie.domain  = "google.com";

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31; Path=/; Domain=google.com");
}

BOOST_AUTO_TEST_CASE(test_secure)
{
    reactive::http::cookie_t user_cookie;
    user_cookie.name    = "user";
    user_cookie.value   = "euskadi31";
    user_cookie.path    = "/";
    user_cookie.secure  = true;

    BOOST_CHECK_EQUAL(reactive::http::cookie::to_string(user_cookie), "user=euskadi31; Path=/; Secure");
}

BOOST_AUTO_TEST_CASE(test_from_string)
{
    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=euskadi31");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "euskadi31");
        BOOST_CHECK_EQUAL(cookie.domain,       "");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      0);
        BOOST_CHECK_EQUAL(cookie.path,         "");
        BOOST_CHECK_EQUAL(cookie.secure,       false);
        BOOST_CHECK_EQUAL(cookie.http_only,    false);
    }

    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=euskadi31; Path=/; Secure");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "euskadi31");
        BOOST_CHECK_EQUAL(cookie.domain,       "");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      0);
        BOOST_CHECK_EQUAL(cookie.path,         "/");
        BOOST_CHECK_EQUAL(cookie.secure,       true);
        BOOST_CHECK_EQUAL(cookie.http_only,    false);
    }

    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=euskadi31; Path=/; Domain=google.com");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "euskadi31");
        BOOST_CHECK_EQUAL(cookie.domain,       "google.com");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      0);
        BOOST_CHECK_EQUAL(cookie.path,         "/");
        BOOST_CHECK_EQUAL(cookie.secure,       false);
        BOOST_CHECK_EQUAL(cookie.http_only,    false);
    }

    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=euskadi31; Max-Age=3600");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "euskadi31");
        BOOST_CHECK_EQUAL(cookie.domain,       "");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      3600);
        BOOST_CHECK_EQUAL(cookie.path,         "");
        BOOST_CHECK_EQUAL(cookie.secure,       false);
        BOOST_CHECK_EQUAL(cookie.http_only,    false);
    }

    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=deleted; Expires=Thu, 01-Jan-1970 00:00:00 UTC");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "deleted");
        BOOST_CHECK_EQUAL(cookie.domain,       "");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      0);
        BOOST_CHECK_EQUAL(cookie.path,         "");
        BOOST_CHECK_EQUAL(cookie.secure,       false);
        BOOST_CHECK_EQUAL(cookie.http_only,    false);
    }

    {
        reactive::http::cookie_t cookie;

        cookie = reactive::http::cookie::from_string("user=euskadi31; HttpOnly; Max-Age=3600");

        BOOST_CHECK_EQUAL(cookie.name,         "user");
        BOOST_CHECK_EQUAL(cookie.value,        "euskadi31");
        BOOST_CHECK_EQUAL(cookie.domain,       "");
        BOOST_CHECK_EQUAL(cookie.expires,      0);
        BOOST_CHECK_EQUAL(cookie.max_age,      3600);
        BOOST_CHECK_EQUAL(cookie.path,         "");
        BOOST_CHECK_EQUAL(cookie.secure,       false);
        BOOST_CHECK_EQUAL(cookie.http_only,    true);
    }
}

BOOST_AUTO_TEST_SUITE_END()
