/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>

namespace reactive {
namespace net {

    class port
    {
    public:
        static bool is_valid(const int& port_);

        /** Check if port is valid
         *
         * @param  port_ The string of port
         * @return       True if port is valid
         */
        static bool is_valid(const std::string& port_);
    };

} // end of net namespace
} // end of reactive namespace
