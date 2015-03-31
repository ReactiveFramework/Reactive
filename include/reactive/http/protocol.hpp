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
namespace protocol {

    static const std::string CRLF       = "\r\n";
    static const std::string VERSION_10 = "1.0";
    static const std::string VERSION_11 = "1.1";

    static const std::string METHOD_HEAD       = "HEAD";
    static const std::string METHOD_GET        = "GET";
    static const std::string METHOD_POST       = "POST";
    static const std::string METHOD_PUT        = "PUT";
    static const std::string METHOD_DELETE     = "DELETE";
    static const std::string METHOD_OPTIONS    = "OPTIONS";
    static const std::string METHOD_TRACE      = "TRACE";
    static const std::string METHOD_CONNECT    = "CONNECT";
    static const std::string METHOD_PATCH      = "PATCH";

} // end of protocol namespace
} // end of http namespace
} // end of reactive namespace
