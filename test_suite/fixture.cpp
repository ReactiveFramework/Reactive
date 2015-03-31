/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <reactive/http/server.hpp>
#include <reactive/web/application.hpp>
#include <chrono>
#include <thread>
#include <future>

class server_fixture
{
public:
    server_fixture()
    {
        m_app = new reactive::web::application();

        m_app->match("/test/hello/world", [] (
            const reactive::http::request& req_,
            const reactive::web::router::parameters& params_,
            reactive::http::response& res_
        ) -> void {

            res_.setStatusCode(reactive::http::status::OK);
            res_.setContent("Hello World");

        }).method("GET");


        m_app->match("/test/remote/ip", [] (
            const reactive::http::request& req_,
            const reactive::web::router::parameters& params_,
            reactive::http::response& res_
        ) -> void {

            res_.setStatusCode(reactive::http::status::OK);
            res_.setContent(req_.info.ip);

        }).method("GET");


        m_server = new reactive::http::server(*m_app);

        m_thread = std::thread([this]() -> void {
            std::cout << "Starting http server..." << std::endl;
            m_server->run();
        });

        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
    }

    ~server_fixture()
    {
        std::cout << "Stoping http server..." << std::endl;
        m_server->stop();

        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);

        m_thread.detach();

        delete m_server;
        delete m_app;
    }

    private:
        reactive::web::application* m_app;
        reactive::http::server* m_server;
        std::thread m_thread;
};

BOOST_GLOBAL_FIXTURE(server_fixture)
