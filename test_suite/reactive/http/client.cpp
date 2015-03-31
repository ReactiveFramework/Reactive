/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/client.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_client)

BOOST_AUTO_TEST_CASE(test_send)
{
    {
        reactive::http::request request;
        request.setVersion("1.0");
        request.setUrl("http://127.0.0.1:8080/test/hello/world");

        std::future<reactive::http::response> future = reactive::http::client::send(request);

        reactive::http::response response = future.get();

        BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
        BOOST_CHECK_EQUAL(response.getContent(), "Hello World");
    }
}

BOOST_AUTO_TEST_CASE(test_get)
{
    {
        std::future<reactive::http::response> future = reactive::http::client::get(
            "http://127.0.0.1:8080/test/hello/world"
        );

        reactive::http::response response = future.get();

        BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
        BOOST_CHECK_EQUAL(response.getContent(), "Hello World");
    }
}

BOOST_AUTO_TEST_SUITE_END()
