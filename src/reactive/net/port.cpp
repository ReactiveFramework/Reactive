/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/net/port.hpp>
#include <cstdlib>

namespace reactive {
namespace net {

    bool port::is_valid(const int& port_)
    {
        if (port_ > 0 && port_ <= 65535)
        {
            return true;
        }

        return false;
    }

    bool port::is_valid(const std::string& port_)
    {
        std::size_t value_len = port_.length();

        if (value_len > 5)
        {
            return false;
        }

        for (std::size_t i = 0; i < value_len; i++)
        {
            int chr = port_.at(i);
            // check if char is not number
            if (
                chr < 48 || chr > 57 // 0-9
            )
            {
                return false;
            }
        }

        return is_valid(atoi(port_.c_str()));
    }


} // end of net namespace
} // end of reactive namespace
