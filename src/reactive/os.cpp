/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/os.hpp>

namespace reactive {

    const std::string& os::hostname()
    {
        return details::hostname;
    }

} // end of reactive namespace
