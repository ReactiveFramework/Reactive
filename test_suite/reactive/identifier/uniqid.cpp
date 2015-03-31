/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/identifier/uniqid.hpp>
#include <boost/regex.hpp>

BOOST_AUTO_TEST_SUITE(reactive_identifier_uniqid)

BOOST_AUTO_TEST_CASE(test_uniqid)
{
    std::string id = reactive::identifier::uniqid();

    BOOST_CHECK_EQUAL(id.length(), 13);

    boost::regex r("[a-f0-9]{13}");
    boost::cmatch matches;
    BOOST_CHECK_EQUAL(boost::regex_match(id.c_str(), matches, r), true);
}

BOOST_AUTO_TEST_SUITE_END()
