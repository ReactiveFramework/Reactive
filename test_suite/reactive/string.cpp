/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/string.hpp>
#include <vector>

BOOST_AUTO_TEST_SUITE(reactive_string_tests)

BOOST_AUTO_TEST_CASE(test_slice)
{
    std::string str_in = "http://user:admin@www.google.com/search?q=test";
    std::string str_out;

    reactive::string::headSlice("://", str_in, str_out);
    BOOST_CHECK_EQUAL(str_out, "http");

    str_out.clear();

    reactive::string::tailSlice("?", str_in, str_out, false);
    BOOST_CHECK_EQUAL(str_out, "q=test");

    str_out.clear();

    reactive::string::tailSlice("/", str_in, str_out, true);
    BOOST_CHECK_EQUAL(str_out, "/search");

    str_out.clear();

    reactive::string::headSlice("@", str_in, str_out);
    BOOST_CHECK_EQUAL(str_out, "user:admin");

    str_out.clear();

    str_in = "http://www.yahoo.com";
    reactive::string::headSlice("://", str_in, str_out);
    BOOST_CHECK_EQUAL(str_out, "http");

    str_out.clear();

    reactive::string::tailSlice("#", str_in, str_out, false);
    BOOST_CHECK_EQUAL(str_out, "");

    str_out.clear();

    reactive::string::tailSlice("?", str_in, str_out, false);
    BOOST_CHECK_EQUAL(str_out, "");

    str_out.clear();

    reactive::string::tailSlice("/", str_in, str_out, true);
    BOOST_CHECK_EQUAL(str_out, "");

    str_out.clear();
}

BOOST_AUTO_TEST_CASE(test_split_as_vector)
{
    std::string str_in = "foo,bar;what";

    std::vector<std::string> vstr_out;

    reactive::string::split(";", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 2);

    BOOST_CHECK_EQUAL(vstr_out[0], "foo,bar");
    BOOST_CHECK_EQUAL(vstr_out[1], "what");
}

BOOST_AUTO_TEST_CASE(test_split_as_vector_no_deli)
{
    std::string str_in = "This is a string with no delimiter";

    std::vector<std::string> vstr_out;
    reactive::string::split(":", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 1);
    BOOST_CHECK_EQUAL(vstr_out.at(0), str_in);

}

BOOST_AUTO_TEST_CASE(test_split_as_vector_deli_at_start)
{
    // Simple delimiter start
    std::string str_in = ";This is starting with a delimiter";

    std::vector<std::string> vstr_out;
    reactive::string::split(";", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 2);
    BOOST_CHECK_EQUAL(vstr_out.at(0), "");
    BOOST_CHECK_EQUAL(vstr_out.at(1), "This is starting with a delimiter");

    // Double delimiter start
    vstr_out.clear();
    str_in = ";;This is starting with a double delimiter";
    reactive::string::split(";", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 3);
    BOOST_CHECK_EQUAL(vstr_out.at(0), "");
    BOOST_CHECK_EQUAL(vstr_out.at(1), "");
    BOOST_CHECK_EQUAL(vstr_out.at(2), "This is starting with a double delimiter");

}

BOOST_AUTO_TEST_CASE(test_split_as_vector_deli_at_end)
{
    // Simple delimiter start
    std::string str_in = "This is ending with a delimiter;";

    std::vector<std::string> vstr_out;
    reactive::string::split(";", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 2);
    BOOST_CHECK_EQUAL(vstr_out.at(0), "This is ending with a delimiter");
    BOOST_CHECK_EQUAL(vstr_out.at(1), "");

    // Double delimiter start
    vstr_out.clear();
    str_in = "This is ending with a double delimiter;;";
    reactive::string::split(";", str_in, vstr_out);

    BOOST_REQUIRE_EQUAL(vstr_out.size(), 3);
    BOOST_CHECK_EQUAL(vstr_out.at(0), "This is ending with a double delimiter");
    BOOST_CHECK_EQUAL(vstr_out.at(1), "");
    BOOST_CHECK_EQUAL(vstr_out.at(2), "");

}

BOOST_AUTO_TEST_CASE(test_split_as_set)
{
    std::string str_in = "foo,bar;what";

    std::set<std::string> sstr_out;
    std::set<std::string>::iterator sstr_it;

    reactive::string::split(";", str_in, sstr_out);

    BOOST_REQUIRE_EQUAL(sstr_out.size(), 2);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "foo,bar");

    sstr_it++;
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "what");
}

BOOST_AUTO_TEST_CASE(test_split_as_set_no_deli)
{
    std::string str_in = "This is a string with no delimiter";

    std::set<std::string> sstr_out;
    std::set<std::string>::iterator sstr_it;

    reactive::string::split(":", str_in, sstr_out);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE_EQUAL(sstr_out.size(), 1);
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, str_in);

}

BOOST_AUTO_TEST_CASE(test_split_as_set_deli_at_start)
{
    // Simple delimiter start
    std::string str_in = ";This is starting with a delimiter";
    std::set<std::string> sstr_out;
    std::set<std::string>::iterator sstr_it;

    reactive::string::split(";", str_in, sstr_out);

    BOOST_REQUIRE_EQUAL(sstr_out.size(), 2);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "");

    sstr_it++;
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "This is starting with a delimiter");

    // Double delimiter start
    sstr_out.clear();
    str_in = ";;This is starting with a double delimiter";
    reactive::string::split(";", str_in, sstr_out);

    BOOST_REQUIRE_EQUAL(sstr_out.size(), 2);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, ""); // set two times

    sstr_it++;
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "This is starting with a double delimiter");
}

BOOST_AUTO_TEST_CASE(test_split_as_set_deli_at_end)
{
    // Simple delimiter start
    std::string str_in = "This is ending with a delimiter;";
    std::set<std::string> sstr_out;
    std::set<std::string>::iterator sstr_it;

    reactive::string::split(";", str_in, sstr_out);

    BOOST_REQUIRE_EQUAL(sstr_out.size(), 2);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "");

    sstr_it++;
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "This is ending with a delimiter");

    // Double delimiter start
    sstr_out.clear();
    str_in = "This is ending with a double delimiter;;";
    reactive::string::split(";", str_in, sstr_out);

    BOOST_REQUIRE_EQUAL(sstr_out.size(), 2);

    sstr_it = sstr_out.begin();
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, ""); // set two times but only one occurence in set

    sstr_it++;
    BOOST_REQUIRE(sstr_it != sstr_out.end());
    BOOST_CHECK_EQUAL(*sstr_it, "This is ending with a double delimiter");
}

BOOST_AUTO_TEST_CASE(test_recursive_replace)
{
    std::string str_in = "foo%2Fbar%3Awhat";
    std::string str_out = reactive::string::substitute("%", "%%", str_in);
    BOOST_CHECK_EQUAL(str_out, "foo%%2Fbar%%3Awhat");

    str_in = "Atomic world is Amarine";
    str_out = reactive::string::substitute("A", "suba", str_in);
    BOOST_CHECK_EQUAL(str_out, "subatomic world is subamarine");

    str_in = "Atomic world in Amarine is ranked A";
    str_out = reactive::string::substitute("A", "X", str_in);
    BOOST_CHECK_EQUAL(str_out, "Xtomic world in Xmarine is ranked X");
}


BOOST_AUTO_TEST_CASE(test_to_number)
{
    char test[] = "123";
    BOOST_CHECK_EQUAL(reactive::string::to_number<int>(test), 123);
    BOOST_CHECK_EQUAL(reactive::string::to_number<int>("123"), 123);
    BOOST_CHECK_EQUAL(reactive::string::to_number<double>("123.34"), 123.34);
    BOOST_CHECK_EQUAL(reactive::string::to_number<long>("12346456"), 12346456);
    BOOST_CHECK_EQUAL(reactive::string::to_number<int>("-1234"), -1234);
    BOOST_CHECK_EQUAL(reactive::string::to_number<int>("a"), 0);
}


BOOST_AUTO_TEST_SUITE_END()
