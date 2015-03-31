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
#include <reactive/json/types.hpp>

namespace reactive {
namespace json {

    inline std::string encode(const value& value_, bool prettify_ = false)
    {
        return value_.serialize(prettify_);
    }

    inline std::ostream& operator<<(std::ostream& os_, const value& x_)
    {
        x_.serialize(std::ostream_iterator<char>(os_));
        return os_;
    }

} // end of json namespace
} // end of reactive namespace
