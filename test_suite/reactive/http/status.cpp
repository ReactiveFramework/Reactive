/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/status.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_status)

BOOST_AUTO_TEST_CASE(test_to_string)
{
    reactive::http::status_t status;

    status = reactive::http::status::CONTINUE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Continue");

    status = reactive::http::status::SWITCHING_PROTOCOLS;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Switching Protocols");

    status = reactive::http::status::PROCESSING;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Processing");

    status = reactive::http::status::OK;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "OK");

    status = reactive::http::status::CREATED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Created");

    status = reactive::http::status::ACCEPTED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Accepted");

    status = reactive::http::status::NON_AUTHORITATIVE_INFORMATION;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Non-Authoritative Information");

    status = reactive::http::status::NO_CONTENT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "No Content");

    status = reactive::http::status::RESET_CONTENT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Reset Content");

    status = reactive::http::status::PARTIAL_CONTENT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Partial Content");

    status = reactive::http::status::MULTI_STATUS;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Multi-Status");

    status = reactive::http::status::ALREADY_REPORTED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Already Reported");

    status = reactive::http::status::IM_USED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "IM Used");

    status = reactive::http::status::MULTIPLE_CHOICES;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Multiple Choices");

    status = reactive::http::status::MOVED_PERMANENTLY;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Moved Permanently");

    status = reactive::http::status::FOUND;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Found");

    status = reactive::http::status::SEE_OTHER;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "See Other");

    status = reactive::http::status::NOT_MODIFIED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Not Modified");

    status = reactive::http::status::USE_PROXY;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Use Proxy");

    status = reactive::http::status::TEMPORARY_REDIRECT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Temporary Redirect");

    status = reactive::http::status::PERMANENT_REDIRECT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Permanent Redirect");

    status = reactive::http::status::TOO_MANY_REDIRECT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Too Many Redirect");

    status = reactive::http::status::BAD_REQUEST;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Bad Request");

    status = reactive::http::status::UNAUTHORIZED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Unauthorized");

    status = reactive::http::status::PAYMENT_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Payment Required");

    status = reactive::http::status::FORBIDDEN;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Forbidden");

    status = reactive::http::status::NOT_FOUND;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Not Found");

    status = reactive::http::status::METHOD_NOT_ALLOWED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Method Not Allowed");

    status = reactive::http::status::NOT_ACCEPTABLE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Not Acceptable");

    status = reactive::http::status::PROXY_AUTHENTICATION_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Proxy Authentication Required");

    status = reactive::http::status::REQUEST_TIME_OUT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Request Time-out");

    status = reactive::http::status::CONFLICT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Conflict");

    status = reactive::http::status::GONE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Gone");

    status = reactive::http::status::LENGTH_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Length Required");

    status = reactive::http::status::PRECONDITION_FAILED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Precondition Failed");

    status = reactive::http::status::REQUEST_ENTITY_TOO_LARGE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Request Entity Too Large");

    status = reactive::http::status::REQUEST_URI_TOO_LARGE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Request-URI Too Large");

    status = reactive::http::status::UNSUPPORTED_MEDIA_TYPE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Unsupported Media Type");

    status = reactive::http::status::REQUESTED_RANGE_NOT_SATISFIABLE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Requested range not satisfiable");

    status = reactive::http::status::EXPECTATION_FAILED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Expectation Failed");

    status = reactive::http::status::IM_A_TEAPOT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "I'm a teapot");

    status = reactive::http::status::UNPROCESSABLE_ENTITY;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Unprocessable Entity");

    status = reactive::http::status::LOCKED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Locked");

    status = reactive::http::status::FAILED_DEPENDENCY;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Failed Dependency");

    status = reactive::http::status::UPGRADE_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Upgrade Required");

    status = reactive::http::status::PRECONDITION_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Precondition Required");

    status = reactive::http::status::TOO_MANY_REQUESTS;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Too Many Requests");

    status = reactive::http::status::REQUEST_HEADER_FIELDS_TOO_LARGE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Request Header Fields Too Large");

    status = reactive::http::status::INTERNAL_SERVER_ERROR;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Internal Server Error");

    status = reactive::http::status::NOT_IMPLEMENTED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Not Implemented");

    status = reactive::http::status::BAD_GATEWAY;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Bad Gateway");

    status = reactive::http::status::SERVICE_UNAVAILABLE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Service Unavailable");

    status = reactive::http::status::GATEWAY_TIME_OUT;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Gateway Time-out");

    status = reactive::http::status::HTTP_VERSION_NOT_SUPPORTED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "HTTP Version not supported");

    status = reactive::http::status::INSUFFICIENT_STORAGE;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Insufficient Storage");

    status = reactive::http::status::LOOP_DETECTED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Loop Detected");

    status = reactive::http::status::NOT_EXTENDED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Not Extended");

    status = reactive::http::status::NETWORK_AUTHENTICATION_REQUIRED;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Network Authentication Required");

    status = reactive::http::status::UNKNOWN;

    BOOST_CHECK_EQUAL(reactive::http::status::to_string(status), "Unknown error");
}

BOOST_AUTO_TEST_SUITE_END()
