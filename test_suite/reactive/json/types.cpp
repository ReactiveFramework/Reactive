/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/json/types.hpp>

BOOST_AUTO_TEST_SUITE(reactive_json_types)

BOOST_AUTO_TEST_CASE(test_constructor)
{
    {
        reactive::json::value test = "foo";
        BOOST_CHECK_EQUAL(test.is<std::string>(), true);
        BOOST_CHECK_EQUAL(test.get<std::string>(), "foo");
    }

    {
        reactive::json::value test = U"éàç";
        BOOST_CHECK_EQUAL(test.is<std::string>(), true);
        BOOST_CHECK_EQUAL(test.get<std::string>(), "éàç");
    }

    {
        reactive::json::value test = 1;
        BOOST_CHECK_EQUAL(test.is<double>(), true);
        BOOST_CHECK_EQUAL(test.get<double>(), 1);
    }

    {
        reactive::json::value test = (int64_t)1;
        BOOST_CHECK_EQUAL(test.is<int64_t>(), true);
        BOOST_CHECK_EQUAL(test.get<int64_t>(), 1);
    }

    {
        reactive::json::value test = 1.5;
        BOOST_CHECK_EQUAL(test.is<double>(), true);
        BOOST_CHECK_EQUAL(test.get<double>(), 1.5);
    }

    {
        reactive::json::value test = true;
        BOOST_CHECK_EQUAL(test.is<bool>(), true);
        BOOST_CHECK_EQUAL(test.get<bool>(), true);
    }

    {
        reactive::json::value test = false;
        BOOST_CHECK_EQUAL(test.is<bool>(), true);
        BOOST_CHECK_EQUAL(test.get<bool>(), false);
    }

    {
        reactive::json::array data;
        data.push_back("foo");
        data.push_back("bar");

        reactive::json::value test = data;
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(), true);
        BOOST_CHECK_EQUAL(test.get<reactive::json::array>().size(), 2);
    }

    {
        reactive::json::object data;
        data["foo"] = "bar";
        data["foo1"] = "bar2";

        reactive::json::value test = data;
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), true);
        BOOST_CHECK_EQUAL(test.get<reactive::json::object>().size(), 2);
    }

    {
        std::size_t size = 5;
        reactive::json::value test = size;
        BOOST_CHECK_EQUAL(test.is<double>(), true);
        BOOST_CHECK_EQUAL(test.get<double>(), 5);
    }
}

BOOST_AUTO_TEST_CASE(test_to_string)
{
    {
        reactive::json::value test("foo");
        BOOST_CHECK_EQUAL(test.toString(), "foo");
    }

    {
        reactive::json::array data;
        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(test.toString(), "array");
    }

    {
        reactive::json::object data;
        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(test.toString(), "object");
    }

    {
        reactive::json::value test(reactive::json::null_type);
        BOOST_CHECK_EQUAL(test.toString(), "null");
    }

    {
        reactive::json::value test(reactive::json::boolean_type);
        BOOST_CHECK_EQUAL(test.toString(), "false");
    }

    {
        reactive::json::value test(reactive::json::number_type);
        BOOST_CHECK_EQUAL(test.toString(), "0");
    }

    {
        reactive::json::value test(reactive::json::int64_type);
        BOOST_CHECK_EQUAL(test.toString(), "0");
    }

    {
        reactive::json::value test(reactive::json::string_type);
        BOOST_CHECK_EQUAL(test.toString(), "");
    }

    {
        reactive::json::value test(reactive::json::array_type);
        BOOST_CHECK_EQUAL(test.toString(), "array");
    }

    {
        reactive::json::value test(reactive::json::object_type);
        BOOST_CHECK_EQUAL(test.toString(), "object");
    }

    /*{
        reactive::json::value test(7);
        BOOST_CHECK_THROW(test.toString(), std::runtime_error);
    }*/
}

BOOST_AUTO_TEST_CASE(test_get)
{
    {
        reactive::json::value test("foo");
        BOOST_CHECK_EQUAL(test.get<std::string>(), "foo");
    }

    {
        reactive::json::value test(true);
        BOOST_CHECK_EQUAL(test.get<bool>(), true);
    }

    {
        reactive::json::value test(false);
        BOOST_CHECK_EQUAL(test.get<bool>(), false);
    }

    {
        reactive::json::value test(1.15);
        BOOST_CHECK_EQUAL(test.get<double>(), 1.15);
    }

    {
        reactive::json::value test((int64_t)1);
        BOOST_CHECK_EQUAL(test.get<int64_t>(), 1);
    }

    {
        reactive::json::array data;
        data.push_back(reactive::json::value("foo"));

        reactive::json::value val(data);

        BOOST_CHECK_EQUAL(val.get(0).is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get(0).get<std::string>(), "foo");
    }

    {
        reactive::json::object data;
        data["foo"] = reactive::json::value("bar");

        reactive::json::value val(data);

        BOOST_CHECK_EQUAL(val.get("foo").is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get("foo").get<std::string>(), "bar");
    }

    {
        reactive::json::value val;

        BOOST_CHECK_THROW(val.get(1), std::runtime_error);
        BOOST_CHECK_THROW(val.get("foo"), std::runtime_error);
    }
}

BOOST_AUTO_TEST_CASE(test_is)
{
    {
        reactive::json::value test("foo");
        BOOST_CHECK_EQUAL(test.is<std::string>(),       true);
        BOOST_CHECK_EQUAL(test.is<double>(),            false);
        BOOST_CHECK_EQUAL(test.is<int64_t>(),           false);
        BOOST_CHECK_EQUAL(test.is<bool>(),              false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(),  false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), false);
    }

    {
        reactive::json::value test(true);
        BOOST_CHECK_EQUAL(test.is<std::string>(),       false);
        BOOST_CHECK_EQUAL(test.is<double>(),            false);
        BOOST_CHECK_EQUAL(test.is<int64_t>(),           false);
        BOOST_CHECK_EQUAL(test.is<bool>(),              true);
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(),  false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), false);
    }

    {
        reactive::json::value test(false);
        BOOST_CHECK_EQUAL(test.is<std::string>(),       false);
        BOOST_CHECK_EQUAL(test.is<double>(),            false);
        BOOST_CHECK_EQUAL(test.is<int64_t>(),           false);
        BOOST_CHECK_EQUAL(test.is<bool>(),              true);
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(),  false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), false);
    }

    {
        reactive::json::value test(1.15);
        BOOST_CHECK_EQUAL(test.is<std::string>(),       false);
        BOOST_CHECK_EQUAL(test.is<double>(),            true);
        BOOST_CHECK_EQUAL(test.is<int64_t>(),           false);
        BOOST_CHECK_EQUAL(test.is<bool>(),              false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(),  false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), false);
    }

    {
        reactive::json::value test((int64_t)1);
        BOOST_CHECK_EQUAL(test.is<std::string>(),       false);
        BOOST_CHECK_EQUAL(test.is<double>(),            true);
        BOOST_CHECK_EQUAL(test.is<int64_t>(),           true);
        BOOST_CHECK_EQUAL(test.is<bool>(),              false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::array>(),  false);
        BOOST_CHECK_EQUAL(test.is<reactive::json::object>(), false);
    }
}

BOOST_AUTO_TEST_CASE(test_evaluate_as_boolean)
{
    {
        reactive::json::value test;
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), false);
    }

    {
        reactive::json::value test(true);
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), true);
    }

    {
        reactive::json::value test(false);
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), false);
    }

    {
        reactive::json::value test(0);
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), false);
    }

    {
        reactive::json::value test(1);
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), true);
    }

    {
        reactive::json::value test("");
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), false);
    }

    {
        reactive::json::value test("foo");
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), true);
    }

    {
        reactive::json::value test(reactive::json::object_type);
        BOOST_CHECK_EQUAL(test.evaluateAsBoolean(), true);
    }
}

BOOST_AUTO_TEST_CASE(test_escape)
{
    {
        reactive::json::value test("\"");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\\"\"");
    }

    {
        reactive::json::value test("\\");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\\\\"");
    }

    {
        reactive::json::value test("/");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\/\"");
    }

    {
        reactive::json::value test("\b");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\b\"");
    }

    {
        reactive::json::value test("\f");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\f\"");
    }

    {
        reactive::json::value test("\n");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\n\"");
    }

    {
        reactive::json::value test("\r");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\r\"");
    }

    {
        reactive::json::value test("\t");
        BOOST_CHECK_EQUAL(test.serialize(), "\"\\t\"");
    }

    {
        reactive::json::value test("hello \0");
        BOOST_CHECK_EQUAL(test.serialize(), "\"hello \"");
    }
}

BOOST_AUTO_TEST_CASE(test_operator)
{
    {
        reactive::json::value val1, val2;

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1((int64_t)1), val2((int64_t)1);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1(true), val2(true);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1(false), val2(false);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1("hello"), val2("hello");

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1(15), val2(15);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        reactive::json::value val1(15.5), val2(15.5);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    /*{
        reactive::json::value val1(7), val2(7);

        BOOST_CHECK_THROW(val1 == val2, std::runtime_error);
    }*/
}

BOOST_AUTO_TEST_CASE(test_contains)
{
    {
        reactive::json::array data;
        data.push_back(reactive::json::value("foo"));

        reactive::json::value val(data);

        BOOST_CHECK_EQUAL(val.contains(0), true);
        BOOST_CHECK_EQUAL(val.contains(1), false);
    }

    {
        reactive::json::object data;
        data["foo"] = reactive::json::value("bar");

        reactive::json::value val(data);

        BOOST_CHECK_EQUAL(val.contains("foo"), true);
        BOOST_CHECK_EQUAL(val.contains("bar"), false);
    }

    {
        reactive::json::value val;

        BOOST_CHECK_THROW(val.contains(1), std::runtime_error);
        BOOST_CHECK_THROW(val.contains("foo"), std::runtime_error);
    }
}

BOOST_AUTO_TEST_SUITE_END()

