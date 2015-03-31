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
#include <stdlib.h>
#include <sys/utsname.h>

namespace reactive {

    namespace details {

        static inline std::string get_hostname()
        {
            struct utsname uname_data;
            uname(&uname_data);

            return std::string(uname_data.nodename);
        }

        static const std::string hostname = get_hostname();
    }

    class os
    {
    public:
        /** Returns the hostname of the operating system.
         *
         * @return string of hostname
         */
        static const std::string& hostname();
        /*{
            return details::hostname;
        }*/
    };

} // end of reactive namespace
