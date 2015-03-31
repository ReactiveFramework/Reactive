/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/mime_types.hpp>

namespace reactive {
namespace http {
namespace mime_types {

    std::string extension_to_type(const std::string& extension_)
    {
        for (mapping m: mappings)
        {
            if (m.extension == extension_)
            {
                return m.mime_type;
            }
        }

        return "text/plain";
    }

} // end of mime_types namespace
} // end of http namespace
} // end of reactive namespace
