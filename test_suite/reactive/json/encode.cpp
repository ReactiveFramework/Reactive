/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/json/encode.hpp>

BOOST_AUTO_TEST_SUITE(reactive_json_encode)

BOOST_AUTO_TEST_CASE(test_encode)
{
    // null test
    {
        reactive::json::value test;
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "null");
    }

    // boolean test
    {
        reactive::json::value test(true);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "true");
    }

    {
        reactive::json::value test(false);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "false");
    }

    {
        reactive::json::value test(1);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "1");
    }

    {
        BOOST_CHECK_THROW(reactive::json::value(NAN), std::overflow_error);
    }

    // int64_t test
    {
        reactive::json::value test((int64_t)1);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "1");
    }
    //@BUG
    /*{
        reactive::json::object data;
        data["foo"] = reactive::json::value(1.1);

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "{\"foo\": 1.1}");
    }*/

    // string test
    {
        reactive::json::value test("hello");
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"hello\"");
    }

    {
        reactive::json::value test(std::string("hello"));
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"hello\"");
    }

    {
        reactive::json::value test(std::string("éùçè"));
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"\u00e9\u00f9\u00e7\u00e8\"");
    }

    {
        std::string data = "hello ";

        data += (char)0x7f;

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"hello \\u007f\"");
    }

    //@BUG
    /*{
        reactive::json::value test("今日は");
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"\\u4ECA\\u65E5\\u306F\"");
    }*/

    // object test
    {
        reactive::json::object data;

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "{}");
    }

    {
        reactive::json::object data;

        data["foo"] = reactive::json::value("bar");

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "{\"foo\":\"bar\"}");
    }

    {
        reactive::json::object data;

        data["foo"] = reactive::json::value("bar");
        data["foo1"] = reactive::json::value("bar2");

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "{\"foo\":\"bar\",\"foo1\":\"bar2\"}");
    }

    // array test
    {
        reactive::json::array data;

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "[]");
    }

    {
        reactive::json::array data;

        data.push_back(reactive::json::value("bar"));

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "[\"bar\"]");
    }

    {
        reactive::json::array list;

        reactive::json::object data;
        data["foo"] = reactive::json::value("bar");

        list.push_back(reactive::json::value(data));

        reactive::json::value test(list);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "[{\"foo\":\"bar\"}]");
    }

    {
        reactive::json::array list, list2;

        list2.push_back(reactive::json::value(1));
        list2.push_back(reactive::json::value(2));
        list2.push_back(reactive::json::value(3));

        list.push_back(reactive::json::value(list2));

        reactive::json::value test(list);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "[[1,2,3]]");
    }

    {
        reactive::json::object data;

        data["foo"] = reactive::json::value("bar");

        reactive::json::value test(data);
        BOOST_CHECK_EQUAL(test.serialize(true), "{\n    \"foo\": \"bar\"\n}\n");
    }

    {
        reactive::json::array list, list2;

        list2.push_back(reactive::json::value(1));
        list2.push_back(reactive::json::value(2));
        list2.push_back(reactive::json::value(3));

        list.push_back(reactive::json::value(list2));

        reactive::json::value test(list);
        BOOST_CHECK_EQUAL(test.serialize(true), "[\n    [\n        1,\n        2,\n        3\n    ]\n]\n");
    }

    {
        reactive::json::array list;

        reactive::json::object data;
        data["foo"] = reactive::json::value("bar");

        list.push_back(reactive::json::value(data));

        reactive::json::value test(list);

        std::stringstream ss;
        ss << test;

        BOOST_CHECK_EQUAL(ss.str(), "[{\"foo\":\"bar\"}]");
    }

    //@BUG
    /*{
        reactive::json::value test("\"\\/\b\f\n\r\t\x00\x19 !#[]\x7F\xFF", 17);
        BOOST_CHECK_EQUAL(reactive::json::encode(test), "\"\\\"\\\\\\/\\b\\f\\n\\r\\t\\u0000\\u0019 !#[]\\u007f\\u00ff\"");
    }*/
}

BOOST_AUTO_TEST_SUITE_END()

