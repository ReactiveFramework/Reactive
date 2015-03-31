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
#include <reactive/crypto/sha256.hpp>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

namespace reactive {
namespace crypto {

    std::string sha256(const std::string& data_)
    {
        CryptoPP::SHA256 sha;
        std::string hash = "";

        CryptoPP::StringSource(
            data_,
            true,
            new CryptoPP::HashFilter(
                sha,
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
