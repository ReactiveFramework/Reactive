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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <cmath>

#include <reactive/json/func.hpp>

#define __STDC_FORMAT_MACROS
#include <errno.h>
#include <inttypes.h>

#include <unicode/unistr.h>

namespace reactive {
namespace json {

    typedef enum {
        null_type,
        boolean_type,
        number_type,
        string_type,
        array_type,
        object_type,
        int64_type
    } type_t;

    enum {
        INDENT_WIDTH = 4
    };

    struct null
    {
    };

    class value
    {
    public:
        typedef std::vector<value> array;
        typedef std::map<std::string, value> object;

        union _storage
        {
            bool boolean_;
            double number_;
            int64_t int64_;

            std::string* string_;
            std::u32string* string32_;
            array* array_;
            object* object_;
        };

    protected:
        type_t m_type;
        _storage m_u;

    public:
        value() : m_type(null_type)
        {
        }

        value(type_t type_) : m_type(type_)
        {
            switch (type_)
            {
#define INIT(p, v) case p##type: m_u.p = v; break
                INIT(boolean_,  false);
                INIT(number_,   0.0);
                INIT(int64_,    0);
                INIT(string_,   new std::string());
                INIT(array_,    new array());
                INIT(object_,   new object());
#undef INIT
                case null_type:
                default:
                    break;
            }
        }

        value(std::nullptr_t) : m_type(null_type)
        {

        }

        value(bool b_) : m_type(boolean_type)
        {
            m_u.boolean_ = b_;
        }

        value(int n_) : m_type(number_type)
        {
            m_u.number_ = n_;
        }

        value(std::size_t n_) : m_type(number_type)
        {
            m_u.number_ = n_;
        }

        value(int64_t i_) : m_type(int64_type)
        {
            m_u.int64_ = i_;
        }

        value(double n_) : m_type(number_type)
        {
            if (std::isnan(n_) || std::isinf(n_))
            {
                throw std::overflow_error("");
            }

            m_u.number_ = n_;
        }

        value(const std::string& s_) : m_type(string_type)
        {
            m_u.string_ = new std::string(s_);
        }

        value(const std::u32string& s_) : m_type(string_type)
        {
            m_u.string_ = new std::string();

            icu::UnicodeString uni_str = icu::UnicodeString::fromUTF32(
                (const UChar32*)s_.c_str(),
                (int32_t)s_.length()
            );
            //std::string str;
            uni_str.toUTF8String((*m_u.string_));
            //m_u.string_ = new std::string(str.begin(), str.end());
        }

        value(const array& a_) : m_type(array_type)
        {
            m_u.array_ = new array(a_);
        }

        value(const object& o_) : m_type(object_type)
        {
            m_u.object_ = new object(o_);
        }

        value(const char* s_) : m_type(string_type)
        {
            m_u.string_ = new std::string(s_);
        }

        value(const char* s_, std::size_t len_) : m_type(string_type)
        {
            m_u.string_ = new std::string(s_, len_);
        }

        value(const char32_t* s_) : value(std::u32string(s_))
        {
        }

        ~value()
        {
            switch (m_type)
            {
#define DEINIT(p) case p##type: delete m_u.p; break
                DEINIT(string_);
                DEINIT(array_);
                DEINIT(object_);
#undef DEINIT
                default: break;
            }
        }

        value(const value& x_) : m_type(x_.m_type)
        {
            switch (m_type)
            {
    #define INIT(p, v) case p##type: m_u.p = v; break
                INIT(string_,   new std::string(*x_.m_u.string_));
                INIT(array_,    new array(*x_.m_u.array_));
                INIT(object_,   new object(*x_.m_u.object_));
    #undef INIT
                default:
                    m_u = x_.m_u;
                    break;
            }
        }

        value& operator=(const value& x_)
        {
            if (this != &x_)
            {
                this->~value();
                new (this) value(x_);
            }

            return *this;
        }

        void swap(value& x_)
        {
            std::swap(m_type, x_.m_type);
            std::swap(m_u, x_.m_u);
        }

        template <typename T>
        bool is() const;

        template <typename T>
        const T& get() const;

        template <typename T>
        T& get();

        bool evaluateAsBoolean() const
        {
            switch (m_type)
            {
                case null_type:
                    return false;
                case boolean_type:
                    return m_u.boolean_;
                case number_type:
                    return m_u.number_ != 0;
                case string_type:
                    return !m_u.string_->empty();
                default:
                    return true;
            }
        }

        const value& get(std::size_t idx_) const;

        const value& get(const std::string& key_) const;

        value& get(std::size_t idx_);

        value& get(const std::string& key_);

        bool contains(std::size_t idx_) const;

        bool contains(const std::string& key_) const;

        std::string toString() const
        {
            switch (m_type)
            {
                case boolean_type:
                    return m_u.boolean_ ? "true" : "false";

                case int64_type:
                {
                    char buf[sizeof("-9223372036854775808")];
                    snprintf(buf, sizeof(buf), "%" PRId64, m_u.int64_);
                    return buf;
                }

                case number_type:
                {
                    char buf[256];
                    double tmp;
                    snprintf(
                        buf,
                        sizeof(buf),
                        (fabs(m_u.number_) < (1ULL << 53) && modf(m_u.number_, &tmp) == 0) ? "%.f" : "%.17g",
                        m_u.number_
                    );

                    return buf;
                }
                case string_type:
                    return *m_u.string_;
                case array_type:
                    return "array";
                case object_type:
                    return "object";
                case null_type:
                default:
                    return "null";
            }
        }

        template <typename Iter>
        inline void serialize(Iter os_, bool prettify_ = false) const
        {
            return _serialize(os_, prettify_ ? 0 : -1);
        }

        inline std::string serialize(bool prettify_ = false) const
        {
            return _serialize(prettify_ ? 0 : -1);
        }

    private:
        /*template <typename T>
        value(const T*); // intentionally defined to block implicit conversion of pointer to bool
    */
        template <typename Iter>
        static void _indent(Iter os_, int indent_)
        {
            *os_++ = '\n';
            for (int i = 0; i < indent_ * INDENT_WIDTH; ++i)
            {
                *os_++ = ' ';
            }
        }

        template <typename Iter>
        void _serialize(Iter os_, int indent_) const
        {
            switch (m_type)
            {
                case string_type:
                    serialize_str(*m_u.string_, os_);
                    break;
                case array_type:
                {
                    *os_++ = '[';

                    if (indent_ != -1)
                    {
                        ++indent_;
                    }

                    for (array::const_iterator i = m_u.array_->begin(); i != m_u.array_->end(); ++i)
                    {
                        if (i != m_u.array_->begin())
                        {
                            *os_++ = ',';
                        }

                        if (indent_ != -1)
                        {
                            _indent(os_, indent_);
                        }

                        i->_serialize(os_, indent_);
                    }

                    if (indent_ != -1)
                    {
                        --indent_;
                        if (!m_u.array_->empty())
                        {
                            _indent(os_, indent_);
                        }
                    }

                    *os_++ = ']';
                    break;
                }
                case object_type:
                {
                    *os_++ = '{';

                    if (indent_ != -1)
                    {
                        ++indent_;
                    }

                    for (object::const_iterator i = m_u.object_->begin(); i != m_u.object_->end(); ++i)
                    {
                        if (i != m_u.object_->begin())
                        {
                            *os_++ = ',';
                        }

                        if (indent_ != -1)
                        {
                            _indent(os_, indent_);
                        }

                        serialize_str(i->first, os_);

                        *os_++ = ':';

                        if (indent_ != -1)
                        {
                            *os_++ = ' ';
                        }

                        i->second._serialize(os_, indent_);
                    }

                    if (indent_ != -1)
                    {
                        --indent_;
                        if (!m_u.object_->empty())
                        {
                            _indent(os_, indent_);
                        }
                    }

                    *os_++ = '}';
                    break;
                }
                default:
                    copy(toString(), os_);
                    break;
            }

            if (indent_ == 0)
            {
                *os_++ = '\n';
            }
        }

        std::string _serialize(int indent_) const
        {
            std::string s;
            _serialize(std::back_inserter(s), indent_);
            return s;
        }
    };

    typedef value::array array;
    typedef value::object object;


#define IS(ctype, jtype)                  \
    template <>                           \
    inline bool value::is<ctype>() const  \
    {                                     \
        return m_type == jtype##_type;    \
    }

    IS(null,        null)
    IS(bool,        boolean)
    IS(int64_t,     int64)
    IS(std::string, string)
    IS(array,       array)
    IS(object,      object)
#undef IS


    template <>
    inline bool value::is<double>() const
    {
        return ((m_type == number_type) || (m_type == int64_type));
    }


#define GET(ctype, var)                                                                   \
    template <>                                                                           \
    inline const ctype& value::get<ctype>() const                                         \
    {                                                                                     \
        JSON_ASSERT("type mismatch! call vis<type>() before get<type>()" && is<ctype>()); \
        return var;                                                                       \
    }                                                                                     \
    template <>                                                                           \
    inline ctype& value::get<ctype>()                                                     \
    {                                                                                     \
        JSON_ASSERT("type mismatch! call is<type>() before get<type>()" && is<ctype>());  \
        return var;                                                                       \
    }

    GET(bool,           m_u.boolean_)
    GET(std::string,    *m_u.string_)
    GET(array,          *m_u.array_)
    GET(object,         *m_u.object_)
    GET(double,         (m_type == int64_type && (const_cast<value*>(this)->m_type = number_type, const_cast<value*>(this)->m_u.number_ = m_u.int64_), m_u.number_))
    GET(int64_t,        m_u.int64_)
#undef GET

    inline const value& value::get(std::size_t idx_) const
    {
        static value s_null;
        JSON_ASSERT(is<array>());
        return idx_ < m_u.array_->size() ? (*m_u.array_)[idx_] : s_null;
    }

    inline value& value::get(std::size_t idx_)
    {
        static value s_null;
        JSON_ASSERT(is<array>());
        return idx_ < m_u.array_->size() ? (*m_u.array_)[idx_] : s_null;
    }

    inline const value& value::get(const std::string& key_) const
    {
        static value s_null;
        JSON_ASSERT(is<object>());
        object::const_iterator i = m_u.object_->find(key_);
        return i != m_u.object_->end() ? i->second : s_null;
    }

    inline value& value::get(const std::string& key_)
    {
        static value s_null;
        JSON_ASSERT(is<object>());
        object::iterator i = m_u.object_->find(key_);
        return i != m_u.object_->end() ? i->second : s_null;
    }

    inline bool value::contains(std::size_t idx_) const
    {
        JSON_ASSERT(is<array>());
        return idx_ < m_u.array_->size();
    }

    inline bool value::contains(const std::string& key_) const
    {
        JSON_ASSERT(is<object>());
        object::const_iterator i = m_u.object_->find(key_);
        return i != m_u.object_->end();
    }

    inline bool operator==(const value& x_, const value& y_)
    {
#define JSON_CMP(type)                                            \
    if (x_.is<type>())                                            \
    {                                                             \
        return y_.is<type>() && x_.get<type>() == y_.get<type>(); \
    }

        JSON_CMP(bool);
        JSON_CMP(double);
        JSON_CMP(std::string);
        JSON_CMP(array);
        JSON_CMP(object);
#undef JSON_CMP

        return y_.is<null>();
    }

    inline bool operator!=(const value& x_, const value& y_)
    {
        return !(x_ == y_);
    }

} // end of json namespace
} // end of reactive namespace

namespace std {
    template<>
    inline void swap(reactive::json::value& x_, reactive::json::value& y_)
    {
        x_.swap(y_);
    }
} // end of std namespace

