/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <stdexcept>
#include <reactive/http/status.hpp>

namespace reactive {
namespace http {

    class exception : public std::runtime_error
    {
    public:
        /**
         * Construct exception with raison and http status code
         *
         * @param  raison_           The raison of exception
         * @param  http_status_code_ The http status code of exception
         */
        explicit exception(const std::string& raison_, const status_t& http_status_code_)
            : std::runtime_error(raison_)
        {
            m_http_status_code = http_status_code_;
        }

        ~exception()
        {
        }

        /**
         * Get http status code of exception
         *
         * @return The status code
         */
        const status_t& getHttpStatusCode() const;

    private:
        status_t m_http_status_code;
    };

} // end of http namespace
} // end of reactive namespace
