/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/identifier/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace reactive {
namespace identifier {

    std::string uuid::v4()
    {
        boost::mt19937 ran;
        boost::uuids::uuid uuid = boost::uuids::random_generator(ran)();
        //boost::uuids::random_generator  random_generator;
        //boost::uuids::uuid              uuid = random_generator();
        return boost::uuids::to_string(uuid);
    }

} // end of identifier namespace
} // end of reactive namespace
