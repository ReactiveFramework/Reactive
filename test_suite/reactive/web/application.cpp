/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <boost/test/unit_test.hpp>
#include <reactive/http/response.hpp>
#include <reactive/http/request.hpp>
#include <reactive/http/status.hpp>
#include <reactive/web/resource.hpp>
#include <reactive/web/application.hpp>
#include <reactive/web/router/parameters.hpp>

class HelloResourceAppTest: public reactive::web::resource
{
public:

    void getAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("Get action has been called by the request");
    }

    void putAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("Put action has been called by the request");
    }

    void postAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        std::string test = "test";

        std::size_t N = 0;
        int M = 0;

        M = std::stoi(request_.getContent());
        N = static_cast<std::size_t>(M);

        std::string output(1, test.at(N));

        response_.setContent(output);
    }
};

BOOST_AUTO_TEST_SUITE(reactive_web_application)

// Testing that the status code is correctly by the resource in the response
BOOST_AUTO_TEST_CASE(test_handle_request)
{
    // Using an empty Request object, the head action is simulated (called directly)
    reactive::http::request req;

    // The response object that should be set with the right status code
    reactive::http::response resp;

    reactive::web::application app;

    HelloResourceAppTest HR;
    app.match("/", HR);

    app.match("/ads", [] (
        const reactive::http::request& request,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response
    ) -> void {

        response.setStatusCode(reactive::http::status::OK);
        response.setContent("Hello World");

    }).method("GET");

    app.match("/user/:id", [] (
        const reactive::http::request& request,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response
    ) -> void {

        response.setStatusCode(reactive::http::status::OK);
        response.setContent("Hello user id=" + parameters_.get("id"));

    }).method("GET").constrain("id", "[0-9]+");

    app.match("/user/:key", [] (
        const reactive::http::request& request,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response
    ) -> void {

        response.setStatusCode(reactive::http::status::OK);
        response.setContent("Hello user key=" + parameters_.get("key"));

    }).method("GET").constrain("key", "[a-f0-9]{32}");

    app.match("/test", [] (
        const reactive::http::request& request,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response
    ) -> void {

        std::string test = "test";

        test.at(6);
    }).method("GET");

    // Simulating standard request
    req.setUrl("http://localhost:8080/");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp.getContent(), "Get action has been called by the request");

    reactive::http::request req2;
    reactive::http::response resp2;

    req2.setMethod("PUT");
    req2.setUrl("http://localhost:8080/");
    resp2.reset(req2);
    app.handleRequest(req2, resp2);

    BOOST_CHECK_EQUAL(resp2.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp2.getContent(), "Put action has been called by the request");

    // Simulating standard request
    req.setUrl("http://localhost:8080/ads");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp.getContent(), "Hello World");

    // Simulating standard request
    req.setUrl("http://localhost:8080/user/123");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp.getContent(), "Hello user id=123");

    // Simulating standard request
    req.setUrl("http://localhost:8080/user/2855f97420cfe63a44c21126f398aab6");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp.getContent(), "Hello user key=2855f97420cfe63a44c21126f398aab6");

    // Simulating standard request
    req.setMethod(reactive::http::protocol::METHOD_POST);
    req.setUrl("http://localhost:8080/user/123");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::NOT_FOUND);

    // Simulating standard request
    req.setMethod(reactive::http::protocol::METHOD_GET);
    req.setUrl("http://localhost:8080/test");
    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::INTERNAL_SERVER_ERROR);

    // Simulating standard non-problematic POST request
    req.setMethod(reactive::http::protocol::METHOD_POST);
    req.setUrl("http://localhost:8080/");
    req.setContent("1");

    resp.reset(req);
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(resp.getContent(), "e");

    // Simulating standard non-problematic POST request
    req.setContent("7");
    app.handleRequest(req, resp);

    BOOST_CHECK_EQUAL(resp.getStatusCode(), reactive::http::status::INTERNAL_SERVER_ERROR);
}


BOOST_AUTO_TEST_SUITE_END()
