/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/net/ip.hpp>
#include <arpa/inet.h>

namespace reactive {
namespace net {

    bool ip::is_v4(const std::string& address_)
    {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, address_.c_str(), &(sa.sin_addr));

        return (result != 0);
    }

    bool ip::is_v6(const std::string& address_)
    {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET6, address_.c_str(), &(sa.sin_addr));

        return (result != 0);
    }

    bool ip::is_valid(const std::string& address_, const version_t& version_)
    {
        switch (version_)
        {
            case IPV4:
                return is_v4(address_);
            case IPV6:
                return is_v6(address_);
            case UNDEFINED:
            default:
                return false;
        }
    }

    bool ip::is_valid(const std::string& address_)
    {
        if (is_valid(address_, IPV4))
        {
            return true;
        }
        else if (is_valid(address_, IPV6))
        {
            return true;
        }

        return false;
    }

} // end of net namespace
} // end of reactive namespace
