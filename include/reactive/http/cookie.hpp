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
#include <ctime>

#define REACTIVE_HTTP_COOKIE_SEPARATOR "; "

namespace reactive {
namespace http {

    /**
     * Structure for a piece of cookie
     */
    struct cookie_t
    {
        std::string name;
        std::string value     = "";
        std::string domain    = "";
        time_t expires        = 0;
        unsigned int max_age  = 0;
        std::string path      = "";
        bool secure           = false;
        bool http_only        = false;
    };

namespace cookie {

    /**
     * Convert cookie_t to cookie string
     *
     * @param  cookie_ [description]
     * @return         [description]
     */
    std::string to_string(const cookie_t& cookie_);

    namespace internal {

        void set_cookie_attr(
            cookie_t& cookie_,
            const std::string& name_,
            const std::string& value_
        );

    } // end of internal namespace

    /**
     * Convert cookie string to cookie_t
     *
     * @param  cookie_ [description]
     * @return         [description]
     */
    cookie_t from_string(const std::string& cookie_);

} // end of cookie namespace
} // end of http namespace
} // end of reactive namespace
