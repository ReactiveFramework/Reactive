/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <algorithm>
#include <cstdio>
#include <string>
#include <stdexcept>

#ifndef JSON_ASSERT
# define JSON_ASSERT(e) do { if (! (e)) throw std::runtime_error(#e); } while (0)
#endif

namespace reactive {
namespace json {

    template <typename Iter>
    void copy(const std::string& s, Iter os)
    {
        std::copy(s.begin(), s.end(), os);
    }

    template <typename Iter>
    void serialize_str(const std::string& s, Iter os)
    {
        *os++ = '"';

        for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
        {
            switch (*i)
            {
#define MAP(val, sym) case val: copy(sym, os); break
                MAP('"',    "\\\"");
                MAP('\\',   "\\\\");
                MAP('/',    "\\/");
                MAP('\b',   "\\b");
                MAP('\f',   "\\f");
                MAP('\n',   "\\n");
                MAP('\r',   "\\r");
                MAP('\t',   "\\t");
#undef MAP
                default:
                    if (static_cast<unsigned char>(*i) < 0x20 || *i == 0x7f)
                    {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", *i & 0xff);
                        copy(buf, buf + 6, os);
                    }
                    else
                    {
                        *os++ = *i;
                    }
                    break;
            }
        }

        *os++ = '"';
    }


    template <typename T>
    struct last_error_t
    {
        static std::string s;
    };

    template <typename T>
    std::string last_error_t<T>::s;

    inline void set_last_error(const std::string& s)
    {
        last_error_t<bool>::s = s;
    }

    inline const std::string& get_last_error()
    {
        return last_error_t<bool>::s;
    }

} // end of json namespace
} // end of reactive namespace
