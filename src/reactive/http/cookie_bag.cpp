/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <stdexcept>
#include <reactive/http/cookie_bag.hpp>
#include <reactive/http/protocol.hpp>

namespace reactive {
namespace http {

    void cookie_bag::add(const cookie_t& cookie_)
    {
        int pos = -1;

        for (std::size_t i = 0; i < size(); ++i)
        {
            if (at(i).name.compare(cookie_.name) == 0)
            {
                pos = i;
            }
        }

        if (pos >= 0)
        {
            at(pos).value = cookie_.value;
        }
        else
        {
            push_back(cookie_);
        }
    }

    void cookie_bag::add(const std::string& name_, const std::string& value_)
    {
        cookie_t cookie;
        cookie.name = name_;
        cookie.value = value_;
        add(cookie);
    }

    bool cookie_bag::has(const std::string& name_) const
    {
        for (std::size_t i = 0; i < size(); ++i)
        {
            if (at(i).name.compare(name_) == 0)
            {
                return true;
            }
        }

        return false;
    }

    cookie_t cookie_bag::get(const std::string& name_) const
    {
        for (std::size_t i = 0; i < size(); ++i)
        {
            if (at(i).name.compare(name_) == 0)
            {
                return at(i);
                break;
            }
        }

        throw std::runtime_error("Cookie " + name_ + " not found.");
    }

    bool cookie_bag::del(const std::string& name_)
    {
        bool retval = false;

        for (std::size_t i = 0; i < size(); ++i)
        {
            if (at(i).name.compare(name_) == 0)
            {
                erase(begin()+i);
                retval = true;
            }
        }

        return retval;
    }

    std::string cookie_bag::toString() const
    {
        std::string cookies = "";

        for (std::size_t i = 0; i < size(); ++i)
        {
            cookies += cookie::to_string(at(i)) + protocol::CRLF;
        }

        return cookies;
    }

} // end of identifier namespace
} // end of reactive namespace
