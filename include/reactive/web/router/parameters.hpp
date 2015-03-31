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
#include <map>
#include <stdexcept>
#include <reactive/string.hpp>

namespace reactive {
namespace web {
namespace router {

    class parameters: public std::map<std::string, std::string>
    {
    public:
        parameters()
        {}

        ~parameters()
        {
        }

        /** Check if parameter exists
         *
         * @param  key_ The string of key
         * @return      True if exists
         */
        inline bool has(const std::string& key_) const
        {
            return (count(key_) > 0);
        }

        /** Get value of parameter by key
         *
         * @param  key_ The string of key
         * @return      The T of value
         */
        template<typename T = std::string>
        const T get(const std::string& key_) const;
    };

    template<>
    inline const std::string parameters::get<std::string>(const std::string& key_) const
    {
        if ((*this).has(key_))
        {
            return (*this).at(key_);
        }

        throw std::invalid_argument("Invalid \"" + key_ + "\" parameter.");
    }

    template<>
    inline const int parameters::get<int>(const std::string& key_) const
    {
        if ((*this).has(key_))
        {
            return reactive::string::to_number<int>((*this).at(key_));
        }

        throw std::invalid_argument("Invalid \"" + key_ + "\" parameter.");
    }

    template<>
    inline const int64_t parameters::get<int64_t>(const std::string& key_) const
    {
        if ((*this).has(key_))
        {
            return reactive::string::to_number<int64_t>((*this).at(key_));
        }

        throw std::invalid_argument("Invalid \"" + key_ + "\" parameter.");
    }

    template<>
    inline const double parameters::get<double>(const std::string& key_) const
    {
        if ((*this).has(key_))
        {
            return reactive::string::to_number<double>((*this).at(key_));
        }

        throw std::invalid_argument("Invalid \"" + key_ + "\" parameter.");
    }

} // end of router namespace
} // end of web namespace
} // end of reactive namespace
