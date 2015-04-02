/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/cookie.hpp>
#include <reactive/uri/encode.hpp>
#include <reactive/string.hpp>
#include <algorithm>

namespace reactive {
namespace http {
namespace cookie {

    typedef enum
    {
        STATE_NAME,
        STATE_VALUE,
        STATE_ATTRIBUTE_NAME,
        STATE_ATTRIBUTE_VALUE
    } state_t;


    std::string to_string(const cookie_t& cookie_)
    {
        std::string cookie;

        cookie.append(reactive::uri::encode(cookie_.name));
        cookie.append("=");

        if (cookie_.value.empty())
        {
            cookie.append("deleted");
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("Expires=Thu, 01-Jan-1970 00:00:00 GMT");
            // delete cookie
        }
        else
        {
            cookie.append(reactive::uri::encode(cookie_.value));

            if (cookie_.expires != 0)
            {
                struct tm * timeinfo;
                timeinfo = gmtime(&cookie_.expires);

                char date[35];
                strftime(date, 35, "%a, %d-%b-%Y %H:%M:%S %Z", timeinfo);

                cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
                cookie.append("Expires=" + std::string(date));
            }
        }

        if (cookie_.max_age != 0)
        {
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("Max-Age=" + std::to_string(cookie_.max_age));
        }

        if (!cookie_.path.empty())
        {
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("Path=" + cookie_.path);
        }

        if (!cookie_.domain.empty())
        {
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("Domain=" + cookie_.domain);
        }

        if (cookie_.secure)
        {
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("Secure");
        }

        if (cookie_.http_only)
        {
            cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            cookie.append("HttpOnly");
        }

        return cookie;
    }

    namespace internal {

        void set_cookie_attr(cookie_t& cookie_, const std::string& name_, const std::string& value_)
        {
            std::string name = name_;

            std::transform(name.begin(), name.end(), name.begin(), ::tolower);

            if (name == "domain")
            {
                cookie_.domain = value_;
            }
            else if (name == "expires")
            {
                struct tm tm = {0};
                strptime(value_.c_str(), "%a, %d-%b-%Y %H:%M:%S %Z", &tm);
                cookie_.expires = timegm(&tm);
            }
            else if (name == "max-age")
            {
                cookie_.max_age = reactive::string::to_number<unsigned int>(value_);
            }
            else if (name == "path")
            {
                cookie_.path = value_;
            }
            else if (name == "secure")
            {
                cookie_.secure = true;
            }
            else if (name == "httponly")
            {
                cookie_.http_only = true;
            }
        }

    } // end of internal namespace

    cookie_t from_string(const std::string& cookie_)
    {
        std::size_t cookie_length = cookie_.length();
        cookie_t cookie;
        state_t state = STATE_NAME;
        std::string data;
        std::string attr_name;
        std::string attr_value;

        for (std::size_t i = 0; i < cookie_length; ++i)
        {
            char chr = cookie_[i];

            switch (state)
            {
                case STATE_NAME:
                    if (chr == '=')
                    {
                        cookie.name = data;
                        data.clear();
                        state = STATE_VALUE;
                        continue;
                    }

                    data += chr;
                    break;

                case STATE_VALUE:
                    if (chr == ';')
                    {
                        cookie.value = data;
                        data.clear();
                        state = STATE_ATTRIBUTE_NAME;
                        continue;
                    }

                    data += chr;

                    if (i == (cookie_length - 1))
                    {
                        cookie.value = data;
                        data.clear();
                    }

                    break;

                case STATE_ATTRIBUTE_NAME:

                    // skip beginning space
                    if (attr_name.empty() && chr == ' ')
                    {
                        continue;
                    }

                    if (chr == ';')
                    {
                        internal::set_cookie_attr(cookie, attr_name, "");
                        attr_name.clear();
                        state = STATE_ATTRIBUTE_NAME;
                        continue;
                    }

                    if (chr == '=')
                    {
                        state = STATE_ATTRIBUTE_VALUE;
                        continue;
                    }

                    attr_name += chr;

                    if (i == (cookie_length - 1))
                    {
                        internal::set_cookie_attr(cookie, attr_name, "");
                        attr_name.clear();
                    }

                    break;

                case STATE_ATTRIBUTE_VALUE:

                    if (chr == ';')
                    {
                        internal::set_cookie_attr(cookie, attr_name, attr_value);
                        attr_name.clear();
                        attr_value.clear();
                        state = STATE_ATTRIBUTE_NAME;
                        continue;
                    }

                    attr_value += chr;

                    if (i == (cookie_length - 1))
                    {
                        internal::set_cookie_attr(cookie, attr_name, attr_value);
                        attr_name.clear();
                        attr_value.clear();
                    }
                    break;
            }
        }

        return cookie;
    }

} // end of cookie namespace
} // end of http namespace
} // end of reactive namespace
