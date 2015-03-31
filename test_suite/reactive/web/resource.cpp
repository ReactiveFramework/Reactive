/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/web/resource.hpp>
#include <reactive/http/response.hpp>
#include <reactive/http/request.hpp>
#include <reactive/http/status.hpp>

class HelloResource: public reactive::web::resource
{
public:

    void preDispatch(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.getHeaders().add("Foo", "bar");
    }

    void getAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("get action");
    }

    void putAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("put action");
    }

    void postDispatch(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setContent("(" + response_.getContent() + ")");
    }
};

BOOST_AUTO_TEST_SUITE(reactive_web_resource)

// Testing that the status code is correctly by the resource in the response
BOOST_AUTO_TEST_CASE(test_headAction)
{
    // Using an empty Request object, the head action is simulated (called directly)
    reactive::http::request request;

    // The resource linking the request to the reponse
    reactive::web::resource resource;

    // The response object that should be set with the right status code
    reactive::http::response response;

    reactive::web::router::parameters parameters;

    // Directly calling the head action to set the response
    resource.headAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_HEAD);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_getAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.getAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_GET);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_postAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.postAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_POST);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_putAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.putAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_PUT);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_deleteAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.deleteAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_DELETE);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_optionsAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.optionsAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_OPTIONS);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_traceAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.traceAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_TRACE);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_connectAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.connectAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_CONNECT);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_patchAction)
{
    reactive::http::request request;
    reactive::web::resource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    resource.patchAction(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);

    request.setMethod(reactive::http::protocol::METHOD_PATCH);

    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

// Testing dispatch
BOOST_AUTO_TEST_CASE(test_dispatch)
{
    reactive::http::request request;
    HelloResource resource;
    reactive::http::response response;
    reactive::web::router::parameters parameters;

    // set method to request
    request.setMethod(reactive::http::protocol::METHOD_PUT);

    // dispatch resource with request and response object
    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getHeaders().has("Foo"), true);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(response.getContent(), "(put action)");

    // set other method for new test
    request.setMethod(reactive::http::protocol::METHOD_GET);

    // redispatch with new method
    resource.dispatch(request, parameters, response);

    BOOST_CHECK_EQUAL(response.getHeaders().has("Foo"), true);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::OK);
    BOOST_CHECK_EQUAL(response.getContent(), "(get action)");
}


BOOST_AUTO_TEST_SUITE_END()
