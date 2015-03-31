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
#include <reactive/http/header_bag.hpp>
#include <reactive/http/protocol.hpp>

namespace reactive {
namespace http {

    void header_bag::add(const header_t& header_, bool unique_)
    {
        int pos = -1;

        // check if unique header
        if (unique_)
        {
            for (std::size_t i = 0; i < size(); ++i)
            {
                if (at(i).name.compare(header_.name) == 0)
                {
                    pos = i;
                }
            }
        }

        if (pos >= 0)
        {
            at(pos).value = header_.value;
        }
        else
        {
            push_back(header_);
        }
    }

    void header_bag::add(const std::string& name_, const std::string& value_, bool unique_)
    {
        header_t header = {
            name_,
            value_
        };
        add(header, unique_);
    }

    bool header_bag::has(const std::string& name_) const
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

    const header_t& header_bag::get(const std::string& name_) const
    {
        for (std::size_t i = 0; i < size(); ++i)
        {
            if (at(i).name.compare(name_) == 0)
            {
                return at(i);
            }
        }

        throw std::runtime_error("Header " + name_ + " not found.");
    }

    bool header_bag::del(const std::string& name_)
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

    std::string header_bag::toString() const
    {
        std::string headers = "";

        for (std::size_t i = 0; i < size(); ++i)
        {
            headers += header::to_string(at(i)) + protocol::CRLF;
            //headers.append();
        }

        return headers;
    }

} // end of identifier namespace
} // end of reactive namespace
