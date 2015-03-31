/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <reactive/uri/encode.hpp>

namespace reactive {
namespace uri {

    std::string encode(const std::string& data_)
    {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = data_.begin(), n = data_.end(); i != n; ++i)
        {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << '%' << std::setw(2) << int((unsigned char) c);
        }

        return escaped.str();
    }

} // end of uri namespace
} // end of reactive namespace
