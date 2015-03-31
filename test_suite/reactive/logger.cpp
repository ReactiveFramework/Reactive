/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/logger.hpp>

BOOST_AUTO_TEST_SUITE(reactive_logger)

BOOST_AUTO_TEST_CASE(test__log)
{
    reactive::logger::setId("reactive_test_custom_string");

    reactive::logger::write(LOG_NOTICE, "Test %d", 2);

    int N = 0;

    reactive::logger::debug("debug %d", ++N);
    reactive::logger::info("information %d", ++N);
    reactive::logger::notice("notice %d", ++N);
    reactive::logger::warning("warning %d", ++N);
    reactive::logger::error("error %d", ++N);
    reactive::logger::critical("critical %d", ++N);
    reactive::logger::alert("alert %d", ++N);
    reactive::logger::emergency("emergency %d", ++N);

    BOOST_CHECK_EQUAL(N, 8);
}


BOOST_AUTO_TEST_SUITE_END()
