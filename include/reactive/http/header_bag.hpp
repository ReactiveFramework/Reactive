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
#include <vector>
#include <reactive/http/header.hpp>

namespace reactive {
namespace http {

    class header_bag: public std::vector<header_t>
    {
    public:
        /**
         * Constructor
         */
        header_bag() : std::vector<header_t>() {}

        ~header_bag() {}

        /**
         * Add header
         *
         * @param header_ The header object to add
         * @param unique_ By default "True" to consider each header as unique
         * headers. Use "False" to add several header with the same name. If
         * False there is no search of an existing header.
         */
        void add(const header_t& header_, bool unique_ = true);

        /**
         * Add header
         *
         * @param name_   The string of header name
         * @param value_  The string of header value
         * @param unique_
         */
        void add(const std::string& name_, const std::string& value_, bool unique_ = true);

        /**
         * Check if header exists
         *
         * @param  name_ The name of header
         * @return       True if header exists
         */
        bool has(const std::string& name_) const;

        /**
         * Get header by name
         *
         * @param  name_ The name of header
         * @return       The header struct
         */
        const header_t& get(const std::string& name_) const;

        /**
         * Delete header by name
         *
         * @param  name_ The header name
         * @return       True if header has been deleted
         */
        bool del(const std::string& name_);

        /**
         * Make headers string
         *
         * @return The string of headers
         */
        std::string toString() const;
    };

} // end of http namespace
} // end of reactive namespace
