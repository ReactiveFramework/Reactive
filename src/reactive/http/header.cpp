/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/header.hpp>

#define REACTIVE_HTTP_HEADER_SEPARATOR ": "

namespace reactive {
namespace http {
namespace header {

    std::string to_string(const header_t& header_)
    {
        return std::string(header_.name + REACTIVE_HTTP_HEADER_SEPARATOR + header_.value);
    }

    header_t from_string(const std::string& header_)
    {
        header_t header;

        std::string data;
        bool is_separator = false;

        for (std::size_t i = 0; i < header_.length(); ++i)
        {
            char chr = header_[i];

            if (is_separator && chr == ' ')
            {
                is_separator = false;
                continue;
            }

            if (chr == '\r' && !header.name.empty())
            {
                header.value = data;
                data.clear();
                break;
            }

            if (chr == ':')
            {
                header.name = data;
                data.clear();
                is_separator = true;
                continue;
            }

            data += chr;
        }

        return header;
    }

} // end of header namespace
} // end of identifier namespace
} // end of reactive namespace
