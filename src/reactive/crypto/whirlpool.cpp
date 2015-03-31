/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <algorithm>
#include <reactive/crypto/whirlpool.hpp>
#include <cryptopp/whrlpool.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

namespace reactive {
namespace crypto {

    std::string whirlpool(const std::string& data_)
    {
        CryptoPP::Whirlpool whirlpool;
        std::string hash = "";

        CryptoPP::StringSource(
            data_,
            true,
            new CryptoPP::HashFilter(
                whirlpool,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash)
                )
            )
        );

        std::transform(hash.begin(), hash.end(), hash.begin(), ::tolower);

        return hash;
    }

} // end of crypto namespace
} // end of reactive namespace
