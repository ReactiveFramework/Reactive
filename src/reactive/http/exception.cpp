/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/exception.hpp>

namespace reactive {
namespace http {

    const status_t& exception::getHttpStatusCode() const
    {
        return m_http_status_code;
    }

} // end of identifier namespace
} // end of reactive namespace
