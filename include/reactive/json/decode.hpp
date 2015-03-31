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
#include <iostream>
#include <iterator>
#include <reactive/json/types.hpp>
#include <reactive/json/parser.hpp>
#include <reactive/json/contexts.hpp>

namespace reactive {
namespace json {

    inline value decode(const std::string& json_)
    {
        value retval;
        std::string err;
        default_parse_context ctx(&retval);

        _parse(ctx, json_.begin(), json_.end(), &err);

        return std::move(retval);
    }

    inline value decode(const std::string& json_, std::string& error_)
    {
        value retval;
        default_parse_context ctx(&retval);

        _parse(ctx, json_.begin(), json_.end(), &error_);

        return std::move(retval);
    }

    inline value decode(std::istream& is_)
    {
        value retval;
        std::string err;
        default_parse_context ctx(&retval);

        _parse(
            ctx,
            std::istreambuf_iterator<char>(is_.rdbuf()),
            std::istreambuf_iterator<char>(),
            &err
        );

        return std::move(retval);
    }

    inline value decode(std::istream& is_, std::string& error_)
    {
        value retval;
        default_parse_context ctx(&retval);

        _parse(
            ctx,
            std::istreambuf_iterator<char>(is_.rdbuf()),
            std::istreambuf_iterator<char>(),
            &error_
        );

        return std::move(retval);
    }

    inline std::istream& operator>>(std::istream& is_, value& x_)
    {
        set_last_error(std::string());
        std::string err;

        x_ = decode(is_, err);

        if (!err.empty())
        {
            set_last_error(err);
            is_.setstate(std::ios::failbit);
        }

        return is_;
    }

} // end of json namespace
} // end of reactive namespace
