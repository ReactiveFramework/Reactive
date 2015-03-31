/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/status.hpp>

namespace reactive {
namespace http {
namespace status {

    std::string to_string(const reactive::http::status_t& code_)
    {
        switch (code_) {
            // 1XX
            case 100: return "Continue";
            case 101: return "Switching Protocols";
            case 102: return "Processing";

            // 2XX
            case 200: return "OK";
            case 201: return "Created";
            case 202: return "Accepted";
            case 203: return "Non-Authoritative Information";
            case 204: return "No Content";
            case 205: return "Reset Content";
            case 206: return "Partial Content";
            case 207: return "Multi-Status";
            case 208: return "Already Reported";
            case 226: return "IM Used";

            // 3XX
            case 300: return "Multiple Choices";
            case 301: return "Moved Permanently";
            case 302: return "Found";
            case 303: return "See Other";
            case 304: return "Not Modified";
            case 305: return "Use Proxy";
            //case 306: return "Reserved";
            case 307: return "Temporary Redirect";
            case 308: return "Permanent Redirect";
            case 310: return "Too Many Redirect";

            // 4XX
            case 400: return "Bad Request";
            case 401: return "Unauthorized";
            case 402: return "Payment Required";
            case 403: return "Forbidden";
            case 404: return "Not Found";
            case 405: return "Method Not Allowed";
            case 406: return "Not Acceptable";
            case 407: return "Proxy Authentication Required";
            case 408: return "Request Time-out";
            case 409: return "Conflict";
            case 410: return "Gone";
            case 411: return "Length Required";
            case 412: return "Precondition Failed";
            case 413: return "Request Entity Too Large";
            case 414: return "Request-URI Too Large";
            case 415: return "Unsupported Media Type";
            case 416: return "Requested range not satisfiable";
            case 417: return "Expectation Failed";
            case 418: return "I'm a teapot";
            case 422: return "Unprocessable Entity";
            case 423: return "Locked";
            case 424: return "Failed Dependency";
            case 426: return "Upgrade Required";
            case 428: return "Precondition Required";
            case 429: return "Too Many Requests";
            case 431: return "Request Header Fields Too Large";

            // 5XX
            case 500: return "Internal Server Error";
            case 501: return "Not Implemented";
            case 502: return "Bad Gateway";
            case 503: return "Service Unavailable";
            case 504: return "Gateway Time-out";
            case 505: return "HTTP Version not supported";
            case 507: return "Insufficient Storage";
            case 508: return "Loop Detected";
            case 510: return "Not Extended";
            case 511: return "Network Authentication Required";
            case 520:
            default:
                return "Unknown error";
        }
    }

} // end of status namespace
} // end of http namespace
} // end of reactive namespace
