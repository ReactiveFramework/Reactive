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

    class ip
    {
    public:
        typedef enum {
            UNDEFINED,
            IPV4,
            IPV6
        } version_t;

        static bool is_v4(const std::string& address_);

        static bool is_v6(const std::string& address_);

        static bool is_valid(const std::string& address_, const version_t& version_);

        static bool is_valid(const std::string& address_);
    };

} // end of net namespace
} // end of reactive namespace
