/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/json/decode.hpp>

BOOST_AUTO_TEST_SUITE(reactive_json_decode)


BOOST_AUTO_TEST_CASE(test_decode)
{
    {
        std::string json = "[1,true,\"hello\"]";
        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::array>(), true);
        BOOST_CHECK_EQUAL(val.get<reactive::json::array>().size(), 3);

        reactive::json::array data = val.get<reactive::json::array>();

        BOOST_CHECK_EQUAL(data[0].is<double>(), true);
        BOOST_CHECK_EQUAL(data[0].get<double>(), 1);

        BOOST_CHECK_EQUAL(data[1].is<bool>(), true);
        BOOST_CHECK_EQUAL(data[1].get<bool>(), true);

        BOOST_CHECK_EQUAL(data[2].is<std::string>(), true);
        BOOST_CHECK_EQUAL(data[2].get<std::string>(), "hello");
    }

    {
        std::string json = "{ \"a\": true }";
        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::object>(), true);
        BOOST_CHECK_EQUAL(val.get<reactive::json::object>().size(), 1);

        reactive::json::object data = val.get<reactive::json::object>();

        BOOST_CHECK_EQUAL(data["a"].is<bool>(), true);
        BOOST_CHECK_EQUAL(data["a"].get<bool>(), true);
    }

    {
        std::string json;

        json = "{ \"b\": true, \"a\": [1,2,\"three\"], \"d\": 2 }";

        reactive::json::value val1 = reactive::json::decode(json);

        json = "{ \"d\": 2.0, \"b\": true, \"a\": [1,2,\"three\"] }";

        reactive::json::value val2 = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val1 == val2, true);
    }

    {
        std::string json = "{\n    \"foo\": \"bar\"\n}\n";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::object>(), true);
    }

    {
        std::string json = "\"foo\"";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get<std::string>(), "foo");
    }

    {
        std::string json = "null";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::null>(), true);
    }

    {
        std::string json = "true";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<bool>(), true);
        BOOST_CHECK_EQUAL(val.get<bool>(), true);
    }

    {
        std::string json = "false";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<bool>(), true);
        BOOST_CHECK_EQUAL(val.get<bool>(), false);
    }

    {
        std::string json = "9223372036854775807";
        int64_t expected = 9223372036854775807;

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<int64_t>(), true);
        BOOST_CHECK_EQUAL(val.get<int64_t>(), expected);
    }

    {
        std::string json = "-9223372036854775809";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<int64_t>(), false);
    }

    {
        std::string json = "1.15";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<double>(), true);
        BOOST_CHECK_EQUAL(val.get<double>(), 1.15);
    }

    {
        std::string json = "{]";
        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: ]");
    }

    {
        std::string json = "nul";
        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: ");
    }

    {
        std::string json = "\"\u00e9\u00f9\u00e7\u00e8\"";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get<std::string>(), "éùçè");
    }

    {
        std::string json = "\"\\ud840\\udc0b\"";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get<std::string>(), "\xf0\xa0\x80\x8b");
    }

    {
        std::string json = "\"\\u\"";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: \"");
    }

    {
        std::string json = "hello ";
        json += (char)0x1f;

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: hello ");
    }

    {
        std::string json = "\\u4";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: \\u4");
    }

    {
        std::string json = "\\u";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: \\u");
    }

    {
        std::string json = "\"\\u4ECA\\u65E5\\u306F\"";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get<std::string>(), "今日は");
    }

    {
        std::string json = "\"\\\"\\\\\\/\\b\\f\\n\\r\\t\"";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<std::string>(), true);
        BOOST_CHECK_EQUAL(val.get<std::string>(), "\"\\/\b\f\n\r\t");
    }

    {
        std::string json = "\"\\i\"";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: \"");
    }

    {
        std::string json = "[]";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::array>(), true);
        BOOST_CHECK_EQUAL(val.get<reactive::json::array>().size(), 0);
    }

    {
        std::string json = "[1, 4 2]";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: 2]");
    }

    {
        std::string json = "{}";

        reactive::json::value val = reactive::json::decode(json);

        BOOST_CHECK_EQUAL(val.is<reactive::json::object>(), true);
        BOOST_CHECK_EQUAL(val.get<reactive::json::object>().size(), 0);
    }

    {
        std::string json = "-";

        std::string error;
        reactive::json::value val = reactive::json::decode(json, error);

        BOOST_CHECK_EQUAL(error.empty(), false);
        BOOST_CHECK_EQUAL(error, "syntax error at line 1 near: ");
    }

    {
        std::string json = "{\"foo\": true}";

        std::stringstream ss;
        ss << json;
        reactive::json::value val;
        ss >> val;

        BOOST_CHECK_EQUAL(val.is<reactive::json::object>(), true);
    }

    {
        std::string json = "{]";

        std::stringstream ss;
        ss << json;
        reactive::json::value val;
        ss >> val;

        BOOST_CHECK_EQUAL(ss.fail(), true);
    }
}

BOOST_AUTO_TEST_SUITE_END()

