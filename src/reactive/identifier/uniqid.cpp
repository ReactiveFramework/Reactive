/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <sys/time.h>
#include <cstdio>
#include <reactive/identifier/uniqid.hpp>

namespace reactive {
namespace identifier {

    std::string uniqid()
    {
        char id[14];
        int sec, usec, size;
        struct timeval tv;

        gettimeofday(
            (struct timeval *) &tv,
            (struct timezone *) NULL
        );

        sec = (int) tv.tv_sec;
        usec = (int) (tv.tv_usec % 0x100000);

        size = sprintf(id, "%08x%05x", sec, usec);

        return std::string(id, size);
    }

} // end of identifier namespace
} // end of reactive namespace
