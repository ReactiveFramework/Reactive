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
#include <reactive/http/cookie.hpp>

namespace reactive {
namespace http {

    class cookie_bag: public std::vector<cookie_t>
    {
    public:

        /**
         * Add cookie
         *
         * @param cookie_ The cookie
         */
        void add(const cookie_t& cookie_);

        /**
         * Add cookie
         *
         * @param name_   The string of cookie name
         * @param value_  The string of cookie value
         */
        void add(const std::string& name_, const std::string& value_);

        /**
         * Check if cookie exists
         *
         * @param  name_ The name of cookie
         * @return       True if cookie exists
         */
        bool has(const std::string& name_) const;

        /**
         * Get cookie by name
         *
         * @param  name_ The name of cookie
         * @return       The cookie struct
         */
        cookie_t get(const std::string& name_) const;

        /**
         * Delete cookie by name
         *
         * @param  name_ The cookie name
         * @return       True if cookie has been deleted
         */
        bool del(const std::string& name_);

        /**
         * Make cookies string
         *
         * @return The string of cookies
         */
        std::string toString() const;
    };

} // end of http namespace
} // end of reactive namespace
