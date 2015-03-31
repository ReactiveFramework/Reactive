/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/web/router/route.hpp>
#include <reactive/web/router/parameters.hpp>

BOOST_AUTO_TEST_SUITE(reactive_web_router_route)

BOOST_AUTO_TEST_CASE(test_match_with_constraints)
{
    std::map<std::string, std::string> constraints;
    reactive::web::router::parameters parameters;

    reactive::web::router::route r("/");
    r.setConstraints(constraints);
    r.compile();

    BOOST_CHECK_EQUAL(r.match("/", parameters), true);
    BOOST_CHECK_EQUAL(parameters.size(), 0);

    constraints["id"] = "[0-9]+";

    reactive::web::router::route r1("/user/:id");
    r1.setConstraints(constraints);
    r1.compile();

    BOOST_CHECK_EQUAL(r1.match("/user/123", parameters), true);
    BOOST_CHECK_EQUAL(parameters.size(), 1);
    BOOST_CHECK_EQUAL(parameters["id"], "123");

    parameters.clear();

    reactive::web::router::route r2("/user/:id");
    r2.setConstraints(constraints);
    r2.compile();

    BOOST_CHECK_EQUAL(r2.match("/user/123a", parameters), false);

    reactive::web::router::route r3("/user/:id/");
    r3.setConstraints(constraints);
    r3.compile();

    BOOST_CHECK_EQUAL(r3.match("/user/123/", parameters), true);
}

BOOST_AUTO_TEST_CASE(test_match_without_constraints)
{
    std::map<std::string, std::string> constraints;
    reactive::web::router::parameters parameters;

    reactive::web::router::route r1("/user/:id");
    r1.setConstraints(constraints);

    BOOST_CHECK_EQUAL(r1.match("/user/123", parameters), true);
    BOOST_CHECK_EQUAL(parameters.size(), 1);
    BOOST_CHECK_EQUAL(parameters["id"], "123");

    parameters.clear();
}

BOOST_AUTO_TEST_CASE(test_get_route)
{
    reactive::web::router::route r1("/user/:id");

    BOOST_CHECK_EQUAL(r1.getRoute(), "/user/:id");
}

BOOST_AUTO_TEST_SUITE_END()
