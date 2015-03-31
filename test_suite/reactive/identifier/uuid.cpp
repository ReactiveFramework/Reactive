/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/identifier/uuid.hpp>

BOOST_AUTO_TEST_SUITE(reactive_identifier_uuid)

BOOST_AUTO_TEST_CASE(test_v4)
{
    std::string uuid = reactive::identifier::uuid::v4();

    BOOST_CHECK_EQUAL(uuid.length(), 36);

    BOOST_CHECK_EQUAL(uuid[14], '4');

    bool is_valid = false;

    if (uuid[19] == '8' || uuid[19] == '9' || uuid[19] == 'a' || uuid[19] == 'b')
    {
        is_valid = true;
    }

    BOOST_CHECK_EQUAL(is_valid, true);
}


BOOST_AUTO_TEST_SUITE_END()
