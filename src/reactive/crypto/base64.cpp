/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/crypto/base64.hpp>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

namespace reactive {
namespace crypto {

    std::string base64::encode(const std::string& data_)
    {
        if (data_.empty())
        {
            return std::string("");
        }

        std::string encoded;
        CryptoPP::StringSource(
            data_,
            true,
            new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(encoded),
                false
            )
        );

        return encoded;
    }

    std::string base64::decode(const std::string& data_)
    {
        if (data_.empty())
        {
            return std::string("");
        }

        std::string decoded;
        CryptoPP::StringSource(
            data_,
            true,
            new CryptoPP::Base64Decoder(
                new CryptoPP::StringSink(decoded)
            )
        );

        return decoded;
    }

} // end of crypto namespace
} // end of reactive namespace
