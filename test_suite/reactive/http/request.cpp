/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/protocol.hpp>
#include <reactive/http/request.hpp>
#include <reactive/json/types.hpp>
#include <reactive/http/exception.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_request)


BOOST_AUTO_TEST_CASE(test_constructor)
{
    reactive::http::request request;

    BOOST_CHECK_EQUAL(request.getUserAgent(), REACTIVE_HTTP_REQUEST_USER_AGENT);
    BOOST_CHECK_EQUAL(request.getMethod(), "GET");
    BOOST_CHECK_EQUAL(request.getVersion(), "1.1");

    BOOST_CHECK_EQUAL(request.isGet(),      true);
    BOOST_CHECK_EQUAL(request.isHead(),     false);
    BOOST_CHECK_EQUAL(request.isPost(),     false);
    BOOST_CHECK_EQUAL(request.isPut(),      false);
    BOOST_CHECK_EQUAL(request.isDelete(),   false);
    BOOST_CHECK_EQUAL(request.isTrace(),    false);
    BOOST_CHECK_EQUAL(request.isOptions(),  false);
    BOOST_CHECK_EQUAL(request.isConnect(),  false);
    BOOST_CHECK_EQUAL(request.isPatch(),    false);
}

BOOST_AUTO_TEST_CASE(test_content)
{
    reactive::http::request request;

    request.setContent("Hello World");

    BOOST_CHECK_EQUAL(request.getContent(), "Hello World");
}

BOOST_AUTO_TEST_CASE(test_content_type)
{
    reactive::http::request request;

    request.setContentType("application/json");

    BOOST_CHECK_EQUAL(request.getContentType(), "application/json");

    request.setContentType("application/json; charset=utf-8");

    BOOST_CHECK_EQUAL(request.getContentType(), "application/json");
}

BOOST_AUTO_TEST_CASE(test_read_request)
{
    std::string data;

    data.append("GET / HTTP/1.0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: keep-alive");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    //data.append("Hello World");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getUserAgent(), "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    BOOST_CHECK_EQUAL(request.getMethod(), "GET");
    BOOST_CHECK_EQUAL(request.getVersion(), "1.0");
    BOOST_CHECK_EQUAL(request.isGet(), true);
    BOOST_CHECK_EQUAL(request.isXmlHttpRequest(), false);
    BOOST_CHECK_EQUAL(request.isUpgrade(), false);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo1"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo2"), false);

    BOOST_CHECK_EQUAL(request.getCookies().get("foo").value, "bar");

    //BOOST_CHECK_EQUAL(request.getContent(), "Hello World");
}

BOOST_AUTO_TEST_CASE(test_is_upgrade)
{
    std::string data;

    data.append("GET /demo HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: example.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Upgrade: WebSocket");
    data.append(reactive::http::protocol::CRLF);
    data.append("WebSocket-Protocol: sample");
    data.append(reactive::http::protocol::CRLF);
    data.append("Origin: http://example.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: Upgrade");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);
    BOOST_CHECK_EQUAL(request.isUpgrade(), true);
}

BOOST_AUTO_TEST_CASE(test_is_connection_keep_alive)
{
    std::string data;

    data.append("GET / HTTP/1.0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: keep-alive");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);
    BOOST_CHECK_EQUAL(request.isConnectionKeepAlive(), true);
    BOOST_CHECK_EQUAL(request.isConnectionClose(), false);
}

BOOST_AUTO_TEST_CASE(test_is_connection_close)
{
    std::string data;

    data.append("GET / HTTP/1.0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: close");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);
    BOOST_CHECK_EQUAL(request.isConnectionKeepAlive(), false);
    BOOST_CHECK_EQUAL(request.isConnectionClose(), true);
}

BOOST_AUTO_TEST_CASE(test_x_forwarded_for)
{
    std::string data;

    data.append("GET / HTTP/1.0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Forwarded-For: 90.2.244.246");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: keep-alive");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    //data.append("Hello World");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.info.ip, "90.2.244.246");
}

BOOST_AUTO_TEST_CASE(test_x_client)
{
    std::string data;

    data.append("GET / HTTP/1.0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Client: 90.2.244.246");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: keep-alive");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    //data.append("Hello World");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.info.ip, "90.2.244.246");
}

/*
BOOST_AUTO_TEST_CASE(test_read_bad_request)
{
    std::string data;

    data.append("GET /");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Encoding: gzip, deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data.append(reactive::http::protocol::CRLF);
    data.append("Connection: keep-alive");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("Hello World");

    try {
        reactive::http::request request(data);
    } catch (swift::http::exception::HttpException e) {
        BOOST_CHECK_EQUAL(e.what(), "Bad Request");
        BOOST_CHECK_EQUAL(e.getStatusCode(), 400);
    }

    std::string data1;

    data1.append("GET / HTTP/1.1");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Host: www.google.com");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Cookie: foo=bar; foo1=bar2");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.8");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Accept-Encoding: gzip, deflate");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Referer: http://www.cplusplus.com/reference/string/string/");
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Connection");
    data1.append(reactive::http::protocol::CRLF);
    data1.append(reactive::http::protocol::CRLF);
    data1.append("Hello World");

    try {
        reactive::http::request request(data1);
    } catch (swift::http::exception::HttpException e) {
        BOOST_CHECK_EQUAL(e.what(), "Bad Request");
        BOOST_CHECK_EQUAL(e.getStatusCode(), 400);
    }
}*/

BOOST_AUTO_TEST_CASE(test_make_request)
{
    std::string data;

    data.append("GET / HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Custom-Line: great");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Reactive (+https://github.com/euskadi31/reactive)");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar");
    // Connection is keep-alive by default for HTTP 1.1 but it is the server
    // setting it explicitly
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("Hello World");

    reactive::http::request request_;

    request_.setMethod("GET");
    request_.setUrl("http://www.google.com");
    request_.getCookies().add("foo", "bar");
    request_.getHeaders().add("X-Custom-Line", "great");
    request_.setContent("Hello World");

    BOOST_CHECK_EQUAL(request_.getHeaders().get("X-Custom-Line").value, "great");
    BOOST_CHECK_EQUAL(request_.toString(), data);
}

BOOST_AUTO_TEST_CASE(test_make_request_with_cookies)
{
    std::string data;

    data.append("GET / HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Reactive (+https://github.com/euskadi31/reactive)");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar1");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request;

    request.setMethod("GET");
    request.setUrl("http://www.google.com");
    request.getCookies().add("foo", "bar");
    request.getCookies().add("foo1", "bar1");

    BOOST_CHECK_EQUAL(request.toString(), data);
}

BOOST_AUTO_TEST_CASE(test_request_init)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar1");

    reactive::http::request req;

    std::string data_basic = data;
    data_basic.append(reactive::http::protocol::CRLF);
    data_basic.append(reactive::http::protocol::CRLF);

    req.parse(data_basic);
    BOOST_CHECK_EQUAL(req.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(req.getMethod(), "GET");
    BOOST_CHECK_EQUAL(req.getUrl().getPath(), "/electric");
    BOOST_CHECK_EQUAL(req.getUrl().getQuery(), "aa=123");
    BOOST_CHECK_EQUAL(req.getUserAgent(), "Reactive (+https://github.com/euskadi31/reactive)");

    std::string data_with_ua = data;
    data_with_ua.append(reactive::http::protocol::CRLF);
    data_with_ua.append("User-Agent: Super Test Web Agent");
    data_with_ua.append(reactive::http::protocol::CRLF);
    data_with_ua.append(reactive::http::protocol::CRLF);

    req.parse(data_with_ua);
    BOOST_CHECK_EQUAL(req.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(req.getMethod(), "GET");
    BOOST_CHECK_EQUAL(req.getUrl().getPath(), "/electric");
    BOOST_CHECK_EQUAL(req.getUrl().getQuery(), "aa=123");
    BOOST_CHECK_EQUAL(req.getData().size(), 0);
    BOOST_CHECK_EQUAL(req.getUserAgent(), "Super Test Web Agent");

    req.parse(data_basic);
    BOOST_CHECK_EQUAL(req.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(req.getMethod(), "GET");
    BOOST_CHECK_EQUAL(req.getUrl().getPath(), "/electric");
    BOOST_CHECK_EQUAL(req.getUrl().getQuery(), "aa=123");
    // This is bad because reset has not been called
    BOOST_CHECK_EQUAL(req.getUserAgent(), "Super Test Web Agent");

    req.reset();

    req.parse(data_basic);
    BOOST_CHECK_EQUAL(req.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(req.getMethod(), "GET");
    BOOST_CHECK_EQUAL(req.getUrl().getPath(), "/electric");
    BOOST_CHECK_EQUAL(req.getUrl().getQuery(), "aa=123");
    // This value is due to the reset
    BOOST_CHECK_EQUAL(req.getUserAgent(), "Reactive (+https://github.com/euskadi31/reactive)");
}

BOOST_AUTO_TEST_CASE(test_x_requested_with)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Requested-With: XMLHttpRequest");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.isXmlHttpRequest(), true);
}

BOOST_AUTO_TEST_CASE(test_forwarded)
{
    std::string data;

    data.append("GET /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Forwarded-For: bad_ip");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request;
    request.info.ip_version = reactive::net::ip::IPV4;
    request.info.ip = "192.168.0.12";
    request.info.port = "52459";
    request.info.local_ip = "192.168.0.15";
    request.info.local_port = "8080";

    request.parse(data);

    BOOST_CHECK_EQUAL(request.info.ip_version, reactive::net::ip::UNDEFINED);
}

BOOST_AUTO_TEST_CASE(test_post_request)
{
    std::string data;

    data.append("POST /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 11");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("foo=bar&a=b");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getUserAgent(), "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    BOOST_CHECK_EQUAL(request.getMethod(), "POST");
    BOOST_CHECK_EQUAL(request.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(request.isPost(), true);
    BOOST_CHECK_EQUAL(request.isXmlHttpRequest(), false);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo1"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo2"), false);

    BOOST_CHECK_EQUAL(request.getCookies().get("foo").value, "bar");

    BOOST_CHECK_EQUAL(request.getContent(), "foo=bar&a=b");

    BOOST_CHECK_EQUAL(request.getData().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_post_json_request)
{
    std::string data;

    data.append("POST /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/json");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cookie: foo=bar; foo1=bar1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 13");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("{\"foo\":\"bar\"}");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getUserAgent(), "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    BOOST_CHECK_EQUAL(request.getMethod(), "POST");
    BOOST_CHECK_EQUAL(request.getVersion(), "1.1");
    BOOST_CHECK_EQUAL(request.isPost(), true);
    BOOST_CHECK_EQUAL(request.isXmlHttpRequest(), false);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo1"), true);

    BOOST_CHECK_EQUAL(request.getCookies().has("foo2"), false);

    BOOST_CHECK_EQUAL(request.getCookies().get("foo").value, "bar");

    BOOST_CHECK_EQUAL(request.getContent(), "{\"foo\":\"bar\"}");

    BOOST_CHECK_EQUAL(request.getJson().is<reactive::json::object>(), true);
}

BOOST_AUTO_TEST_CASE(test_log)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);
    request.info.ip = "90.2.244.246";
    request.info.port = "52459";
    request.info.local_ip = "192.168.0.15";
    request.info.local_port = "80";

    BOOST_CHECK_EQUAL(request.toLog(), "90.2.244.246:52459<>192.168.0.15:80, GET /electric?aa=123 HTTP/1.1, Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");

    data.clear();

    data.append("GET /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request2(data);
    request2.info.ip = "90.2.244.246";
    request2.info.port = "52459";
    request2.info.local_ip = "192.168.0.15";
    request2.info.local_port = "80";

    BOOST_CHECK_EQUAL(request2.toLog(), "90.2.244.246:52459<>192.168.0.15:80, GET /electric HTTP/1.1, Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");

    data.clear();

    data.append("GET /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Forwarded-For: 90.2.244.246, 127.0.0.1");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request3;
    request3.info.ip = "192.168.0.12";
    request3.info.port = "52459";
    request3.info.local_ip = "192.168.0.15";
    request3.info.local_port = "8080";

    request3.parse(data);

    BOOST_CHECK_EQUAL(request3.info.ip, "90.2.244.246");
    BOOST_CHECK_EQUAL(request3.info.local_ip, "192.168.0.15");
    BOOST_CHECK_EQUAL(request3.toLog(), "90.2.244.246<>192.168.0.12:52459<>192.168.0.15:8080, GET /electric HTTP/1.1, Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");

    data.clear();

    data.append("GET /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("X-Forwarded-For: fe80::62eb:69ff:fe8f:1012");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request4;
    request4.info.ip = "192.168.0.12";
    request4.info.port = "52459";
    request4.info.local_ip = "192.168.0.15";
    request4.info.local_port = "8080";

    request4.parse(data);

    BOOST_CHECK_EQUAL(request4.toLog(), "fe80::62eb:69ff:fe8f:1012<>192.168.0.12:52459<>192.168.0.15:8080, GET /electric HTTP/1.1, Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
}



BOOST_AUTO_TEST_CASE(test_has_query_argument)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.hasQueryArgument("aa"), true);
    BOOST_CHECK_EQUAL(request.hasQueryArgument("bb"), false);
}

BOOST_AUTO_TEST_CASE(test_get_query_argument_exception)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_THROW(request.getQueryArgument<std::string>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getQueryArgument<int>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getQueryArgument<int64_t>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getQueryArgument<double>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getQueryArgument<bool>("bb"), reactive::http::exception);
}

BOOST_AUTO_TEST_CASE(test_get_query_argument_string)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getQueryArgument<std::string>("aa"), "123");
    BOOST_CHECK_EQUAL(request.getQueryArgument<std::string>("aa", ""), "123");
    BOOST_CHECK_EQUAL(request.getQueryArgument<std::string>("bb", ""), "");
}

BOOST_AUTO_TEST_CASE(test_get_query_argument_int)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getQueryArgument<int>("aa"), 123);
    BOOST_CHECK_EQUAL(request.getQueryArgument<int>("aa", 456), 123);
    BOOST_CHECK_EQUAL(request.getQueryArgument<int>("bb", 456), 456);
}


BOOST_AUTO_TEST_CASE(test_get_query_argument_int64)
{
    std::string data;

    data.append("GET /electric?aa=123 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getQueryArgument<int64_t>("aa"), 123);
    BOOST_CHECK_EQUAL(request.getQueryArgument<int64_t>("aa", 456), 123);
    BOOST_CHECK_EQUAL(request.getQueryArgument<int64_t>("bb", 456), 456);
}

BOOST_AUTO_TEST_CASE(test_get_query_argument_bool)
{
    std::string data;

    data.append("GET /electric?aa=1&bb=0&cc=true&dd=True&ee=TRUE HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("aa"), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("bb"), false);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("cc"), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("dd"), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("ee"), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("ff", true), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("ee", false), true);
    BOOST_CHECK_EQUAL(request.getQueryArgument<bool>("bb", true), false);
}

BOOST_AUTO_TEST_CASE(test_get_query_argument_double)
{
    std::string data;

    data.append("GET /electric?aa=1.1 HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getQueryArgument<double>("aa"), 1.1);
    BOOST_CHECK_EQUAL(request.getQueryArgument<double>("aa", 2.2), 1.1);
    BOOST_CHECK_EQUAL(request.getQueryArgument<double>("bb", 2.2), 2.2);
}

BOOST_AUTO_TEST_CASE(test_has_body_argument)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=123");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.hasBodyArgument("aa"), true);
    BOOST_CHECK_EQUAL(request.hasBodyArgument("bb"), false);
}

BOOST_AUTO_TEST_CASE(test_get_body_argument_exception)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=123");

    reactive::http::request request(data);

    BOOST_CHECK_THROW(request.getBodyArgument<std::string>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getBodyArgument<int>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getBodyArgument<double>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getBodyArgument<int64_t>("bb"), reactive::http::exception);
    BOOST_CHECK_THROW(request.getBodyArgument<bool>("bb"), reactive::http::exception);
}

BOOST_AUTO_TEST_CASE(test_get_body_argument_string)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=123");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getBodyArgument<std::string>("aa"), "123");
    BOOST_CHECK_EQUAL(request.getBodyArgument<std::string>("aa", ""), "123");
    BOOST_CHECK_EQUAL(request.getBodyArgument<std::string>("bb", ""), "");
}

BOOST_AUTO_TEST_CASE(test_get_body_argument_int)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=123");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getBodyArgument<int>("aa"), 123);
    BOOST_CHECK_EQUAL(request.getBodyArgument<int>("aa", 456), 123);
    BOOST_CHECK_EQUAL(request.getBodyArgument<int>("bb", 456), 456);
}


BOOST_AUTO_TEST_CASE(test_get_body_argument_int64)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=123");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getBodyArgument<int64_t>("aa"), 123);
    BOOST_CHECK_EQUAL(request.getBodyArgument<int64_t>("aa", 456), 123);
    BOOST_CHECK_EQUAL(request.getBodyArgument<int64_t>("bb", 456), 456);
}

BOOST_AUTO_TEST_CASE(test_get_body_argument_bool)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 33");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=1&bb=0&cc=true&dd=True&ee=TRUE");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("aa"), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("bb"), false);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("cc"), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("dd"), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("ee"), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("ff", true), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("ee", false), true);
    BOOST_CHECK_EQUAL(request.getBodyArgument<bool>("bb", true), false);
}

BOOST_AUTO_TEST_CASE(test_get_body_argument_double)
{
    std::string data;

    data.append("POST /electric HTTP/1.1");
    data.append(reactive::http::protocol::CRLF);
    data.append("Host: www.google.com");
    data.append(reactive::http::protocol::CRLF);
    data.append("Cache-Control: max-age=0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/x-www-form-urlencoded");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    data.append(reactive::http::protocol::CRLF);
    data.append("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:23.0) Gecko/20100101 Firefox/23.0 FirePHP/0.7.4");
    data.append(reactive::http::protocol::CRLF);
    data.append("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 6");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("aa=1.1");

    reactive::http::request request(data);

    BOOST_CHECK_EQUAL(request.getBodyArgument<double>("aa"), 1.1);
    BOOST_CHECK_EQUAL(request.getBodyArgument<double>("aa", 2.2), 1.1);
    BOOST_CHECK_EQUAL(request.getBodyArgument<double>("bb", 2.2), 2.2);
}

BOOST_AUTO_TEST_SUITE_END()
