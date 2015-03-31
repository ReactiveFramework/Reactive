/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <map>
#include <reactive/uri/decode.hpp>
#include <reactive/uri/encode.hpp>
#include <reactive/uri/query.hpp>

namespace reactive {
namespace uri {

    typedef enum query_state {
        QUERY_STATE_KEY,
        QUERY_STATE_VALUE
    } query_state_t;


    void query::parse(const std::string& query_)
    {
        query_state_t state = QUERY_STATE_KEY;
        std::string key;
        std::string val;

        for (std::size_t i = 0; i < query_.length(); ++i)
        {
            char chr = query_[i];
            int chr_dec = (int)chr;

            switch (state)
            {
                case QUERY_STATE_KEY:

                    // check if chr_dec is =
                    if (chr_dec == 61)
                    {
                        state = QUERY_STATE_VALUE;
                        continue;
                    }

                    // append chr in key
                    key += chr;

                    break;
                case QUERY_STATE_VALUE:

                    // check if chr_dec is &
                    if (chr_dec == 38)
                    {
                        (*this)[decode(key)] = decode(val);

                        key.clear();
                        val.clear();

                        state = QUERY_STATE_KEY;
                        continue;
                    }

                    val += chr;

                    break;
            }
        }

        (*this)[decode(key)] = decode(val);

        key.clear();
        val.clear();
    }

    bool query::has(const std::string& key_) const
    {
        return ((*this).count(key_) > 0);
    }

    std::string query::toString() const
    {
        std::string retval;

        const_iterator it = (*this).begin();

        while (it != (*this).end())
        {
            retval += encode((*it).first) + "=" + encode((*it).second);

            ++it;

            if (it != (*this).end())
            {
                retval += "&";
            }
        }

        return retval;
    }

} // end of uri namespace
} // end of reactive namespace
