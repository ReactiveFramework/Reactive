/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/web/router/parameters.hpp>
#include <boost/test/unit_test.hpp>
#include <reactive/web/router/route.hpp>
#include <reactive/web/router/context.hpp>
#include <reactive/web/resource.hpp>
#include <reactive/http/request.hpp>

class exception_test_resource: public reactive::web::resource
{
public:
    void getAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("get action");

        throw reactive::http::exception("Bad Request", reactive::http::status::BAD_REQUEST);
    }
};

BOOST_AUTO_TEST_SUITE(reactive_web_router_context)

BOOST_AUTO_TEST_CASE(test_context)
{
    reactive::web::router::route r("/");
    reactive::web::router::context context(r);
    context.setMethod("GET|POST");

    BOOST_CHECK_EQUAL(context.hasMethod("GET"), true);
    BOOST_CHECK_EQUAL(context.hasMethod("POST"), true);
    BOOST_CHECK_EQUAL(context.hasMethod("PUT"), false);
}

// Testing match
BOOST_AUTO_TEST_CASE(test_match)
{
    // request object
    reactive::http::request request;
    // resource object
    reactive::web::resource resource;

    reactive::web::router::parameters parameters;

    // route object
    reactive::web::router::route r("/");
    // context object
    reactive::web::router::context context(r);

    // set resource in context object
    context.setResource(resource);
    // set method in context object
    context.setMethod("GET|POST");

    // check if context type is equal to reactive::web::router::TYPE_OBJECT
    BOOST_CHECK_EQUAL(context.getType(), reactive::web::router::TYPE_OBJECT);

    // set url in request object
    request.setUrl("http://dacteev.com/");
    // set method in request object
    request.setMethod(reactive::http::protocol::METHOD_PUT);

    // check if context match with request
    BOOST_CHECK_EQUAL(context.match(request, parameters), false);

    // reset new method in request object
    request.setMethod(reactive::http::protocol::METHOD_GET);

    // recheck if context match with request object
    BOOST_CHECK_EQUAL(context.match(request, parameters), true);

    // check the path matching against the context route
    request.setUrl("http://dacteev.com/ads");

    BOOST_CHECK_EQUAL(context.match(request, parameters), false);
}

BOOST_AUTO_TEST_CASE(test_handle_request)
{
    reactive::http::request request;
    reactive::http::response response;
    reactive::web::resource resource;
    reactive::web::router::route route("/");
    reactive::web::router::context context(route);
    reactive::web::router::parameters parameters;

    context.setResource(resource);
    context.setMethod("GET|POST");

    // set url in request object
    request.setUrl("http://dacteev.com/");
    // set method in request object
    request.setMethod(reactive::http::protocol::METHOD_GET);

    BOOST_CHECK_EQUAL(context.handleRequest(request, parameters, response), true);

    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::METHOD_NOT_ALLOWED);
}

BOOST_AUTO_TEST_CASE(test_handle_request_with_exception)
{
    // request object
    reactive::http::request request;
    request.setUrl("http://dacteev.com/");

    reactive::http::response response;

    // resource object
    exception_test_resource resource;

    reactive::web::router::parameters parameters;

    // route object
    reactive::web::router::route r("/");
    // context object
    reactive::web::router::context context(r);

    // set resource in context object
    context.setResource(resource);
    // set method in context object
    context.setMethod("GET");

    BOOST_CHECK_EQUAL(context.handleRequest(request, parameters, response), false);
    BOOST_CHECK_EQUAL(response.getStatusCode(), reactive::http::status::BAD_REQUEST);
}

BOOST_AUTO_TEST_CASE(test_to_log_exception_class)
{
    // request object
    reactive::http::request request;
    request.setUrl("http://dacteev.com/");

    reactive::http::response response;

    // resource object
    exception_test_resource resource;

    reactive::web::router::parameters parameters;

    // route object
    reactive::web::router::route r("/");
    // context object
    reactive::web::router::context context(r);

    // set resource in context object
    context.setResource(resource);
    // set method in context object
    context.setMethod("GET");

    reactive::http::exception ex("Bad Request", reactive::http::status::BAD_REQUEST);

    BOOST_CHECK_EQUAL(
        context.toLogException(ex, request),
        "exception_test_resource::getAction(...) => reactive::http::exception : Bad Request"
    );
}

BOOST_AUTO_TEST_CASE(test_to_log_exception_func)
{
    // request object
    reactive::http::request request;
    request.setUrl("http://dacteev.com/");

    reactive::http::response response;

    reactive::web::router::parameters parameters;

    // route object
    reactive::web::router::route r("/");
    // context object
    reactive::web::router::context context(r);

    // set resource in context object
    context.setResource([](
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) -> void
    {
        response_.setStatusCode(reactive::http::status::OK);
        response_.setContent("get action");

        throw reactive::http::exception("Bad Request", reactive::http::status::BAD_REQUEST);
    });
    // set method in context object
    context.setMethod("GET");

    reactive::http::exception ex("Bad Request", reactive::http::status::BAD_REQUEST);

    BOOST_CHECK_EQUAL(
        context.toLogException(ex, request),
        "app.match(\"/\", ...).method(\"GET\") => reactive::http::exception : Bad Request"
    );

    context.setMethod("GET|POST");

    BOOST_CHECK_EQUAL(
        context.toLogException(ex, request),
        "app.match(\"/\", ...).method(\"GET|POST\") => reactive::http::exception : Bad Request"
    );
}

BOOST_AUTO_TEST_SUITE_END()
