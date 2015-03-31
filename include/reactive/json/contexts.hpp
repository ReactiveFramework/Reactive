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
#include <reactive/json/func.hpp>
#include <reactive/json/types.hpp>
#include <reactive/json/parser.hpp>

namespace reactive {
namespace json {

    class deny_parse_context
    {
    public:
        bool setNull()
        {
            return false;
        }

        bool setBool(bool)
        {
            return false;
        }

        bool setInt64(int64_t)
        {
            return false;
        }

        bool setNumber(double)
        {
            return false;
        }

        template <typename Iter>
        bool parseString(parser<Iter>&)
        {
            return false;
        }

        bool parseArrayStart()
        {
            return false;
        }

        template <typename Iter>
        bool parseArrayItem(parser<Iter>&, std::size_t)
        {
            return false;
        }

        bool parseArrayStop(std::size_t)
        {
            return false;
        }

        bool parseObjectStart()
        {
            return false;
        }

        template <typename Iter>
        bool parseObjectItem(parser<Iter>&, const std::string&)
        {
            return false;
        }
    };

    class default_parse_context
    {
    protected:
        value* m_out;
    public:
        default_parse_context(value* out_) : m_out(out_)
        {
        }

        bool setNull()
        {
            *m_out = value();
            return true;
        }

        bool setBool(bool b_)
        {
            *m_out = value(b_);
            return true;
        }

        bool setInt64(int64_t i_)
        {
            *m_out = value(i_);
            return true;
        }

        bool setNumber(double f_)
        {
            *m_out = value(f_);
            return true;
        }

        template<typename Iter>
        bool parseString(parser<Iter>& in_)
        {
            *m_out = value(string_type);
            return _parse_string(m_out->get<std::string>(), in_);
        }

        bool parseArrayStart()
        {
            *m_out = value(array_type);
            return true;
        }

        template <typename Iter>
        bool parseArrayItem(parser<Iter>& in_, std::size_t)
        {
            array& a = m_out->get<array>();
            a.push_back(value());
            default_parse_context ctx(&a.back());
            return _parse(ctx, in_);
        }

        bool parseArrayStop(std::size_t)
        {
            return true;
        }

        bool parseObjectStart()
        {
            *m_out = value(object_type);
            return true;
        }

        template <typename Iter>
        bool parseObjectItem(parser<Iter>& in_, const std::string& key_)
        {
            object& o = m_out->get<object>();
            default_parse_context ctx(&o[key_]);
            return _parse(ctx, in_);
        }

    private:
        default_parse_context(const default_parse_context&);
        default_parse_context& operator=(const default_parse_context&);
    };

    class null_parse_context
    {
    public:
        struct dummy_str
        {
            void push_back(int)
            {
            }
        };
    public:
        null_parse_context()
        {
        }

        bool setNull()
        {
            return true;
        }

        bool setBool(bool)
        {
            return true;
        }

        bool setInt64(int64_t)
        {
            return true;
        }

        bool setNumber(double)
        {
            return true;
        }

        template <typename Iter>
        bool parseString(parser<Iter>& in_)
        {
            dummy_str s;
            return _parse_string(s, in_);
        }

        bool parseArrayStart()
        {
            return true;
        }

        template <typename Iter>
        bool parseArrayItem(parser<Iter>& in_, std::size_t)
        {
            return _parse(*this, in_);
        }

        bool parseArrayStop(std::size_t)
        {
            return true;
        }

        bool parseObjectStart()
        {
            return true;
        }

        template <typename Iter>
        bool parseObjectItem(parser<Iter>& in_, const std::string&)
        {
            return _parse(*this, in_);
        }
    private:
        null_parse_context(const null_parse_context&);
        null_parse_context& operator=(const null_parse_context&);
    };

} // end of json namespace
} // end of reactive namespace
