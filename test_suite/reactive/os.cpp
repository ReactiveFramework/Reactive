/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/os.hpp>
#include <stdlib.h>
#include <sys/utsname.h>

BOOST_AUTO_TEST_SUITE(reactive_os)

BOOST_AUTO_TEST_CASE(test_hostname)
{
    struct utsname uname_data;
    uname(&uname_data);
    std::string expected(uname_data.nodename);

    BOOST_CHECK_EQUAL(reactive::os::hostname(), expected);
}

BOOST_AUTO_TEST_SUITE_END()
