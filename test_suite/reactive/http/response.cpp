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
#include <reactive/http/response.hpp>
#include <reactive/http/status.hpp>
#include <reactive/version.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_response)

BOOST_AUTO_TEST_CASE(test_response_construct)
{
    reactive::http::response res;

    BOOST_CHECK_EQUAL(res.isComplete(), true);

    BOOST_CHECK_EQUAL(res.getCharset(), "UTF-8");
    BOOST_CHECK_EQUAL(res.getContentType(), "text/html");
    BOOST_CHECK_EQUAL(res.getStatusCode(), reactive::http::status::OK);

    res.setCharset("ISO4444-1");
    res.setContentType("extraterrial/ootml");
    res.setStatusCode(reactive::http::status::NOT_FOUND);

    BOOST_CHECK_EQUAL(res.getCharset(), "ISO4444-1");
    BOOST_CHECK_EQUAL(res.getContentType(), "extraterrial/ootml");
    BOOST_CHECK_EQUAL(res.getStatusCode(), reactive::http::status::NOT_FOUND);

    res.reset();

    BOOST_CHECK_EQUAL(res.getCharset(), "UTF-8");
    BOOST_CHECK_EQUAL(res.getContentType(), "text/html");
    BOOST_CHECK_EQUAL(res.getStatusCode(), reactive::http::status::OK);

    res.setContentType("extraterrial/ootml; charset=utf-8");

    BOOST_CHECK_EQUAL(res.getContentType(), "extraterrial/ootml");
}

BOOST_AUTO_TEST_CASE(test_response_setget)
{
    reactive::http::response response;

    response.setCharset("UTF-16");
    response.setContentType("application/json");
    response.setStatusCode(reactive::http::status::NOT_FOUND);
    response.setContent("Hello World");

    BOOST_CHECK_EQUAL(response.getHeaders().size(), 1);

    response.addHeader("X-Dacteev-Server", "fr-aa-dd-aaa");

    BOOST_CHECK_EQUAL(response.getCharset(), "UTF-16");
    BOOST_CHECK_EQUAL(response.getContentType(), "application/json");
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::NOT_FOUND);
    BOOST_CHECK_EQUAL(response.getContent(), "Hello World");
    BOOST_CHECK_EQUAL(response.getHeaders().size(), 2);

    response.getHeaders().add("X-Transaction", "260fd0ee77f9f784");

    BOOST_CHECK_EQUAL(response.getHeaders().size(), 3);

    BOOST_CHECK_EQUAL(response.getCookies().size(), 0);

    response.addCookie("foo", "bar");

    BOOST_CHECK_EQUAL(response.getCookies().size(), 1);
}

BOOST_AUTO_TEST_CASE(test_response_to_string)
{
    reactive::http::response response;

    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string res = response.toString();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 0");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    BOOST_CHECK_EQUAL(res, data);
}

BOOST_AUTO_TEST_CASE(test_cookie_response_to_string)
{
    reactive::http::response response;
    response.addCookie("foo", "bar");
    response.addCookie("foo1", "bar1");

    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string res = response.toString();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 0");
    data.append(reactive::http::protocol::CRLF);
    data.append("Set-Cookie: foo=bar");
    data.append(reactive::http::protocol::CRLF);
    data.append("Set-Cookie: foo1=bar1");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    BOOST_CHECK_EQUAL(res, data);
}

BOOST_AUTO_TEST_CASE(test_content_type_response_to_string)
{
    reactive::http::response response;
    response.setContentType("application/json");

    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string res = response.toString();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: application/json; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 0");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    BOOST_CHECK_EQUAL(res, data);
}

BOOST_AUTO_TEST_CASE(test_head_request_response_to_string)
{
    reactive::http::request request;
    request.setMethod("HEAD");

    reactive::http::response response;
    response.setRequest(request);
    response.setContent("Hello World");

    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string res = response.toString();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    //data.append(reactive::http::protocol::CRLF);
    //data.append("Content-Length: 0");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    BOOST_CHECK_EQUAL(res, data);
}

BOOST_AUTO_TEST_CASE(test_deflate_encoding_response_to_string)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);
    std::string content_str = "Hello World";

    reactive::http::request request;
    request.getHeaders().add("Accept-Encoding", "deflate");

    reactive::http::response response;
    response.setRequest(request);
    response.setContent(content_str);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string res = response.toString();



    std::istringstream uncompressed(content_str);
    std::stringstream compressed;

    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(uncompressed);
    boost::iostreams::copy(out, compressed);

    std::string content = compressed.str();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Encoding: deflate");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: " + std::to_string(content.length()));
    data.append(reactive::http::protocol::CRLF);
    data.append("Vary: Accept-Encoding");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append(content);

    BOOST_CHECK_EQUAL(res, data);
}

BOOST_AUTO_TEST_CASE(test_gzip_encoding_response_to_string)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);
    std::string content_str = "Hello World";

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];


    std::istringstream uncompressed(content_str);
    std::stringstream compressed;

    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::gzip_compressor());
    out.push(uncompressed);

    boost::iostreams::copy(out, compressed);
    std::string content = compressed.str();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Encoding: gzip");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: " + std::to_string(content.length()));
    data.append(reactive::http::protocol::CRLF);
    data.append("Vary: Accept-Encoding");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append(content);

    reactive::http::request request;
    request.getHeaders().add("Accept-Encoding", "gzip");

    reactive::http::response response;
    response.setRequest(request);
    response.setContent(content_str);

    BOOST_CHECK_EQUAL(response.toString(), data);
}

BOOST_AUTO_TEST_CASE(test_bzip2_encoding_response_to_string)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);
    std::string content_str = "Hello World";

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];


    std::istringstream uncompressed(content_str);
    std::stringstream compressed;

    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::bzip2_compressor());
    out.push(uncompressed);

    boost::iostreams::copy(out, compressed);
    std::string content = compressed.str();

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Encoding: bzip2");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: " + std::to_string(content.length()));
    data.append(reactive::http::protocol::CRLF);
    data.append("Vary: Accept-Encoding");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append(content);

    reactive::http::request request;
    request.getHeaders().add("Accept-Encoding", "bzip2");

    reactive::http::response response;
    response.setRequest(request);
    response.setContent(content_str);

    BOOST_CHECK_EQUAL(response.toString(), data);
}

BOOST_AUTO_TEST_CASE(test_response_is)
{
    reactive::http::response response;

    BOOST_CHECK_EQUAL(response.isOk(),              true);
    BOOST_CHECK_EQUAL(response.isServerError(),     false);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     false);
    BOOST_CHECK_EQUAL(response.isRedirection(),     false);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      true);
    BOOST_CHECK_EQUAL(response.isNotFound(),        false);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      false);
    BOOST_CHECK_EQUAL(response.isEmpty(),           false);

    response.setStatusCode(reactive::http::status::NOT_FOUND);

    BOOST_CHECK_EQUAL(response.isOk(),              false);
    BOOST_CHECK_EQUAL(response.isServerError(),     false);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     true);
    BOOST_CHECK_EQUAL(response.isRedirection(),     false);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      false);
    BOOST_CHECK_EQUAL(response.isNotFound(),        true);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      false);
    BOOST_CHECK_EQUAL(response.isEmpty(),           false);

    response.setStatusCode(reactive::http::status::CREATED);

    BOOST_CHECK_EQUAL(response.isOk(),              false);
    BOOST_CHECK_EQUAL(response.isServerError(),     false);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     false);
    BOOST_CHECK_EQUAL(response.isRedirection(),     false);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      true);
    BOOST_CHECK_EQUAL(response.isNotFound(),        false);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      false);
    BOOST_CHECK_EQUAL(response.isEmpty(),           true);

    response.setStatusCode(reactive::http::status::BAD_REQUEST);

    BOOST_CHECK_EQUAL(response.isOk(),              false);
    BOOST_CHECK_EQUAL(response.isServerError(),     false);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     true);
    BOOST_CHECK_EQUAL(response.isRedirection(),     false);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      false);
    BOOST_CHECK_EQUAL(response.isNotFound(),        false);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      true);
    BOOST_CHECK_EQUAL(response.isEmpty(),           false);

    response.setStatusCode(reactive::http::status::MOVED_PERMANENTLY);

    BOOST_CHECK_EQUAL(response.isOk(),              false);
    BOOST_CHECK_EQUAL(response.isServerError(),     false);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     false);
    BOOST_CHECK_EQUAL(response.isRedirection(),     true);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      false);
    BOOST_CHECK_EQUAL(response.isNotFound(),        false);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      false);
    BOOST_CHECK_EQUAL(response.isEmpty(),           false);

    response.setStatusCode(reactive::http::status::INTERNAL_SERVER_ERROR);

    BOOST_CHECK_EQUAL(response.isOk(),              false);
    BOOST_CHECK_EQUAL(response.isServerError(),     true);
    BOOST_CHECK_EQUAL(response.isForbidden(),       false);
    BOOST_CHECK_EQUAL(response.isClientError(),     false);
    BOOST_CHECK_EQUAL(response.isRedirection(),     false);
    BOOST_CHECK_EQUAL(response.isInvalid(),         false);
    BOOST_CHECK_EQUAL(response.isInformational(),   false);
    BOOST_CHECK_EQUAL(response.isSuccessful(),      false);
    BOOST_CHECK_EQUAL(response.isNotFound(),        false);
    BOOST_CHECK_EQUAL(response.isBadRequest(),      false);
    BOOST_CHECK_EQUAL(response.isEmpty(),           false);

}


BOOST_AUTO_TEST_CASE(test_parse_response)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 11");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("Hello World");

    reactive::http::response response(data);

    BOOST_CHECK_EQUAL(response.getHeaders().size(), 4);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(response.getContentType(), "text/html");
    BOOST_CHECK_EQUAL(response.getCharset(), "UTF-8");
    BOOST_CHECK_EQUAL(response.getContent(), "Hello World");
}

BOOST_AUTO_TEST_CASE(test_parse_part_response)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    reactive::http::response response;

    std::string data;
    data.append("HTTP/1.1 200 OK");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 11");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);

    response.parse(data);

    BOOST_CHECK_EQUAL(response.isComplete(), false);

    data = "Hello World";

    response.parse(data);

    BOOST_CHECK_EQUAL(response.isComplete(), true);

    BOOST_CHECK_EQUAL(response.getHeaders().size(), 4);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(response.getContentType(), "text/html");
    BOOST_CHECK_EQUAL(response.getCharset(), "UTF-8");
    BOOST_CHECK_EQUAL(response.getContent(), "Hello World");
}

BOOST_AUTO_TEST_CASE(test_parse_response_not_found)
{
    std::string server(REACTIVE_NAME);
    std::string version(REACTIVE_VERSION);

    time_t rawtime;
    struct tm * timeinfo;
    char date[35];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    std::string data;
    data.append("HTTP/1.1 404 Not Found");
    data.append(reactive::http::protocol::CRLF);
    data.append("Server: " + server + "/" + version);
    data.append(reactive::http::protocol::CRLF);
    data.append("Date: " + std::string(date));
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Type: text/html; charset=UTF-8");
    data.append(reactive::http::protocol::CRLF);
    data.append("Content-Length: 9");
    data.append(reactive::http::protocol::CRLF);
    data.append(reactive::http::protocol::CRLF);
    data.append("Not Found");

    reactive::http::response response(data);

    BOOST_CHECK_EQUAL(response.getHeaders().size(), 4);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::NOT_FOUND);
    BOOST_CHECK_EQUAL(response.getContentType(), "text/html");
    BOOST_CHECK_EQUAL(response.getCharset(), "UTF-8");
    BOOST_CHECK_EQUAL(response.getContent(), "Not Found");
}

BOOST_AUTO_TEST_SUITE_END()
