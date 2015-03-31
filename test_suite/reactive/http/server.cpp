/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/server.hpp>
#include <reactive/http/client.hpp>
#include <reactive/web/application.hpp>

BOOST_AUTO_TEST_SUITE(reactive_http_server)

BOOST_AUTO_TEST_CASE(test_set_keep_alive)
{
    reactive::web::application app;

    reactive::http::server server(app);

    BOOST_CHECK_EQUAL(server.getKeepAliveTimeout(), 15);

    server.setKeepAliveTimeout(35);

    BOOST_CHECK_EQUAL(server.getKeepAliveTimeout(), 35);
}

BOOST_AUTO_TEST_CASE(test_set_application)
{
    reactive::web::application app;

    reactive::http::server server(app);

    BOOST_CHECK_EQUAL(server.getApplication(), &app);
}

BOOST_AUTO_TEST_CASE(test_info_ip)
{
    {
        std::future<reactive::http::response> future = reactive::http::client::get(
            "http://127.0.0.1:8080/test/remote/ip"
        );

        reactive::http::response response = future.get();

        BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
        BOOST_CHECK_EQUAL(response.getContent(), "127.0.0.1");
    }
}

BOOST_AUTO_TEST_SUITE_END()
