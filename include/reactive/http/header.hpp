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
namespace http {

    /** Structure for a piece of header */
    struct header_t
    {
        std::string name;
        std::string value;
    };

namespace header {

    /**
     * Convert header_t to header string
     *
     * @param  header_ The header_t struct
     * @return         The string of header
     */
    std::string to_string(const header_t& header_);

    /**
     * Parse header string to header_t
     *
     * @param  header_ The string of header
     * @return         The header_t struct
     */
    header_t from_string(const std::string& header_);

} // end of header namespace
} // end of http namespace
} // end of reactive namespace
