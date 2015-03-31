/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <cctype>
#include <string>
#include <reactive/uri/decode.hpp>

namespace reactive {
namespace uri {

    std::string decode(const std::string& data_)
    {
        std::string decoded;

        for (std::size_t i = 0; i < data_.length(); ++i)
        {
            if (data_[i] != '%')
            {
                decoded += data_[i];
                continue;
            }

            if (!isdigit(data_[i+1]) || !isdigit(data_[i+2]))
            {
                decoded += data_[i];
                continue;
            }

            decoded += ((data_[i+1] - '0') << 4) | (data_[i+2] - '0');
            i += 2;
        }

        return decoded;
    }

} // end of uri namespace
} // end of reactive namespace
