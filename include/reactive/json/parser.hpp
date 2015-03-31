/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <limits>
#include <string>
#include <inttypes.h>
#include <reactive/json/func.hpp>

namespace reactive {
namespace json {

    template <typename Iter>
    class parser
    {
    protected:
        Iter m_cur;
        Iter m_end;
        int m_last_ch;
        bool m_ungot;
        int m_line;

    public:
        parser(const Iter& first_, const Iter& last_) :
            m_cur(first_),
            m_end(last_),
            m_last_ch(-1),
            m_ungot(false),
            m_line(1)
        {
        }

        int getc()
        {
            if (m_ungot)
            {
                m_ungot = false;
                return m_last_ch;
            }

            if (m_cur == m_end)
            {
                m_last_ch = -1;
                return -1;
            }

            if (m_last_ch == '\n')
            {
                m_line++;
            }

            m_last_ch = *m_cur & 0xff;
            ++m_cur;

            return m_last_ch;
        }

        void ungetc()
        {
            if (m_last_ch != -1)
            {
                JSON_ASSERT(!m_ungot);
                m_ungot = true;
            }
        }

        inline Iter cur() const
        {
            return m_cur;
        }

        inline int line() const
        {
            return m_line;
        }

        void skipWs()
        {
            while (1)
            {
                int ch = getc();
                if (! (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'))
                {
                    ungetc();
                    break;
                }
            }
        }

        bool expect(int expect_)
        {
            skipWs();
            if (getc() != expect_)
            {
                ungetc();
                return false;
            }

            return true;
        }

        bool match(const std::string& pattern_)
        {
            for (std::string::const_iterator pi(pattern_.begin()); pi != pattern_.end(); ++pi)
            {
                if (getc() != *pi)
                {
                    ungetc();
                    return false;
                }
            }

            return true;
        }
    };


    template<typename Iter>
    inline int _parse_quadhex(parser<Iter> &in_)
    {
        int uni_ch = 0,
            hex;

        for (int i = 0; i < 4; i++)
        {
            if ((hex = in_.getc()) == -1)
            {
                return -1;
            }

            if ('0' <= hex && hex <= '9')
            {
                hex -= '0';
            }
            else if ('A' <= hex && hex <= 'F')
            {
                hex -= 'A' - 0xa;
            }
            else if ('a' <= hex && hex <= 'f')
            {
                hex -= 'a' - 0xa;
            }
            else
            {
                in_.ungetc();
                return -1;
            }

            uni_ch = uni_ch * 16 + hex;
        }

        return uni_ch;
    }

    template<typename String, typename Iter>
    inline bool _parse_codepoint(String& out, parser<Iter>& in)
    {
        int uni_ch;

        if ((uni_ch = _parse_quadhex(in)) == -1)
        {
            return false;
        }

        if (0xd800 <= uni_ch && uni_ch <= 0xdfff)
        {
            if (0xdc00 <= uni_ch)
            {
                // a second 16-bit of a surrogate pair appeared
                return false;
            }

            // first 16-bit of surrogate pair, get the next one
            if (in.getc() != '\\' || in.getc() != 'u')
            {
                in.ungetc();
                return false;
            }

            int second = _parse_quadhex(in);

            if (!(0xdc00 <= second && second <= 0xdfff))
            {
                return false;
            }

            uni_ch = ((uni_ch - 0xd800) << 10) | ((second - 0xdc00) & 0x3ff);
            uni_ch += 0x10000;
        }

        if (uni_ch < 0x80)
        {
            out.push_back(uni_ch);
        }
        else
        {
            if (uni_ch < 0x800)
            {
                out.push_back(0xc0 | (uni_ch >> 6));
            }
            else
            {
                if (uni_ch < 0x10000)
                {
                    out.push_back(0xe0 | (uni_ch >> 12));
                }
                else
                {
                    out.push_back(0xf0 | (uni_ch >> 18));
                    out.push_back(0x80 | ((uni_ch >> 12) & 0x3f));
                }

                out.push_back(0x80 | ((uni_ch >> 6) & 0x3f));
            }

            out.push_back(0x80 | (uni_ch & 0x3f));
        }

        return true;
    }

    template<typename String, typename Iter>
    inline bool _parse_string(String& out, parser<Iter>& in)
    {
        while (1)
        {
            int ch = in.getc();
            if (ch < ' ')
            {
                in.ungetc();
                return false;
            }
            else if (ch == '"')
            {
                return true;
            }
            else if (ch == '\\')
            {
                if ((ch = in.getc()) == -1)
                {
                    return false;
                }

                switch (ch)
                {
#define MAP(sym, val) case sym: out.push_back(val); break
                    MAP('"',    '\"');
                    MAP('\\',   '\\');
                    MAP('/',    '/');
                    MAP('b',    '\b');
                    MAP('f',    '\f');
                    MAP('n',    '\n');
                    MAP('r',    '\r');
                    MAP('t',    '\t');
#undef MAP
                    case 'u':
                        if (!_parse_codepoint(out, in))
                        {
                            return false;
                        }
                        break;
                    default:
                        return false;
                }
            }
            else
            {
                out.push_back(ch);
            }
        }
        return false;
    }

    template <typename Context, typename Iter>
    inline bool _parse_array(Context& ctx, parser<Iter>& in)
    {
        if (!ctx.parseArrayStart())
        {
            return false;
        }

        std::size_t idx = 0;

        if (in.expect(']'))
        {
            return ctx.parseArrayStop(idx);
        }
        do
        {
            if (!ctx.parseArrayItem(in, idx))
            {
                return false;
            }

            idx++;
        }
        while (in.expect(','));

        return in.expect(']') && ctx.parseArrayStop(idx);
    }

    template <typename Context, typename Iter>
    inline bool _parse_object(Context& ctx, parser<Iter>& in)
    {
        if (!ctx.parseObjectStart())
        {
            return false;
        }

        if (in.expect('}'))
        {
            return true;
        }

        do
        {
            std::string key;
            if (!in.expect('"') || !_parse_string(key, in) || !in.expect(':'))
            {
                return false;
            }

            if (!ctx.parseObjectItem(in, key))
            {
                return false;
            }
        }
        while (in.expect(','));

        return in.expect('}');
    }

    template <typename Iter>
    inline std::string _parse_number(parser<Iter>& in)
    {
        std::string num_str;
        while (1)
        {
            int ch = in.getc();
            if (('0' <= ch && ch <= '9') || ch == '+' || ch == '-' || ch == 'e' || ch == 'E')
            {
                num_str.push_back(ch);
            }
            else if (ch == '.')
            {
                num_str.push_back('.');
            }
            else
            {
                in.ungetc();
                break;
            }
        }

        return num_str;
    }

    template <typename Context, typename Iter>
    inline bool _parse(Context& ctx, parser<Iter>& in)
    {
        in.skipWs();
        int ch = in.getc();
        switch (ch)
        {
#define IS(ch, text, op) case ch: \
        if (in.match(text) && op) \
        {                         \
            return true;          \
        }                         \
        else                      \
        {                         \
            return false;         \
        }

            IS('n', "ull",  ctx.setNull());
            IS('f', "alse", ctx.setBool(false));
            IS('t', "rue",  ctx.setBool(true));
#undef IS
            case '"':
                return ctx.parseString(in);
            case '[':
                return _parse_array(ctx, in);
            case '{':
                return _parse_object(ctx, in);
            default:
                if (('0' <= ch && ch <= '9') || ch == '-')
                {
                    double f;
                    char *endp;
                    in.ungetc();
                    std::string num_str = _parse_number(in);

                    if (num_str.empty())
                    {
                        return false;
                    }

                    {
                        errno = 0;
                        intmax_t ival = strtoimax(num_str.c_str(), &endp, 10);
                        if (
                            (errno == 0) &&
                            (std::numeric_limits<int64_t>::min() <= ival) &&
                            (ival <= std::numeric_limits<int64_t>::max()) &&
                            (endp == (num_str.c_str() + num_str.size()))
                        )
                        {
                            ctx.setInt64(ival);
                            return true;
                        }
                    }

                    f = strtod(num_str.c_str(), &endp);

                    if (endp == (num_str.c_str() + num_str.size()))
                    {
                        ctx.setNumber(f);
                        return true;
                    }

                    return false;
                }
                break;
        }

        in.ungetc();

        return false;
    }

    template <typename Context, typename Iter>
    inline Iter _parse(Context& ctx, const Iter& first, const Iter& last, std::string* err)
    {
        parser<Iter> in(first, last);

        if (!_parse(ctx, in) && err != NULL)
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "syntax error at line %d near: ", in.line());
            *err = buf;
            while (1)
            {
                int ch = in.getc();
                if (ch == -1 || ch == '\n')
                {
                    break;
                }
                else if (ch >= ' ')
                {
                    err->push_back(ch);
                }
            }
        }

        return in.cur();
    }

} // end of json namespace
} // end of reactive namespace
