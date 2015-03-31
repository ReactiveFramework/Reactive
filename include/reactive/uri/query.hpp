/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <map>
#include <string>

namespace reactive {
namespace uri {

    class query : public std::map<std::string, std::string>
    {
    public:
        query() {}

        query(const std::string& query_)
        {
            parse(query_);
        }

        ~query()
        {
        }

        void parse(const std::string& query_);

        /**
         * Check if key exists
         *
         * @param  key_ The string of key
         * @return      True if the key exists
         */
        bool has(const std::string& key_) const;

        /**
         * Stringify query
         *
         * @return The string of query
         */
        std::string toString() const;
    };

} // end of uri namespace
} // end of reactive namespace
