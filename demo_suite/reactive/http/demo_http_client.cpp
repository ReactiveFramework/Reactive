/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/http/client.hpp>
#include <future>
#include <iostream>

int main(int argc, char const *argv[])
{
    //reactive::http::request request;
    //request.setMethod("GET");
    //request.setUrl("http://curlmyip.com/");

    //std::future<reactive::http::response> future_response = reactive::http::client::send(request);

    std::future<reactive::http::response> future_response = reactive::http::client::get("http://curlmyip.com/");

    reactive::http::response response = future_response.get();

    if (response.isSuccessful())
    {
        std::cout << "my ip address is : " << response.getContent() << std::endl;
    }
    else
    {
        std::cout << "Error: " << response.getStatusCode() << std::endl;
    }

    return 0;
}
