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

BOOST_AUTO_TEST_SUITE(reactive_web_router_parameters)

BOOST_AUTO_TEST_CASE(test_has)
{
    reactive::web::router::parameters parameters;

    BOOST_CHECK_EQUAL(parameters.has("id"), false);
    parameters["id"] = "123";
    BOOST_CHECK_EQUAL(parameters.has("id"), true);
}

BOOST_AUTO_TEST_CASE(test_get_exception)
{
    reactive::web::router::parameters parameters;
    BOOST_CHECK_THROW(parameters.get("id"),             std::invalid_argument);
    BOOST_CHECK_THROW(parameters.get<int>("id"),        std::invalid_argument);
    BOOST_CHECK_THROW(parameters.get<int64_t>("id"),    std::invalid_argument);
    BOOST_CHECK_THROW(parameters.get<double>("id"),     std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_get)
{
    reactive::web::router::parameters parameters;
    parameters["id"] = "123";
    BOOST_CHECK_EQUAL(parameters.get("id"), "123");
}


BOOST_AUTO_TEST_CASE(test_get_int)
{
    reactive::web::router::parameters parameters;
    parameters["id"] = "123";
    BOOST_CHECK_EQUAL(parameters.get<int>("id"), 123);
}

BOOST_AUTO_TEST_CASE(test_get_int64)
{
    reactive::web::router::parameters parameters;
    parameters["id"] = "123";
    BOOST_CHECK_EQUAL(parameters.get<int64_t>("id"), 123);
}

BOOST_AUTO_TEST_CASE(test_get_double)
{
    reactive::web::router::parameters parameters;
    parameters["id"] = "123.3";
    parameters["ratio"] = "-123.3";
    BOOST_CHECK_EQUAL(parameters.get<double>("id"),     123.3);
    BOOST_CHECK_EQUAL(parameters.get<double>("ratio"), -123.3);
}

BOOST_AUTO_TEST_SUITE_END()
