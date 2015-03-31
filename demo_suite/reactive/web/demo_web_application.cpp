/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/http/request.hpp>
#include <reactive/http/response.hpp>
#include <reactive/web/application.hpp>
#include <reactive/http/server.hpp>
#include <reactive/web/router/parameters.hpp>
#include <reactive/json/encode.hpp>
#include <reactive/uri/query.hpp>

int main(int argc, char const *argv[])
{
    reactive::web::application app;

    app.match("/user/:pseudo", [] (
        const reactive::http::request& req_,
        const reactive::web::router::parameters& params_,
        reactive::http::response& res_
    ) -> void {

        res_.setStatusCode(reactive::http::status::OK);
        res_.setContent("Hello " + params_.get("pseudo"));

    }).method("GET").constrain("pseudo", "[a-zA-Z0-9]{3,50}");

    app.match("/test", [] (
        const reactive::http::request& req_,
        const reactive::web::router::parameters& params_,
        reactive::http::response& res_
    ) -> void {

        res_.setStatusCode(reactive::http::status::OK);
        reactive::uri::query data = req_.getData();
        res_.setContent(data["content"]);

    }).method("POST");

    app.match("/api/user", [] (
        const reactive::http::request& req_,
        const reactive::web::router::parameters& params_,
        reactive::http::response& res_
    ) -> void {

        reactive::json::array response;

        reactive::json::object user1;

        user1["firstname"]   = "Axel";
        user1["lastname"]    = "Etcheverry";
        user1["city"]        = "Toulouse";
        user1["country"]     = "France";
        user1["id"]          = 123456;
        user1["website"]     = "http://www.dacteev.com/";
        user1["acl"]         = nullptr;
        user1["active"]      = true;

        response.push_back(user1);

        reactive::json::object user2;

        user2["firstname"]   = "Axel";
        user2["lastname"]    = "Etcheverry";
        user2["city"]        = "Toulouse";
        user2["country"]     = "France";
        user2["id"]          = 123456;
        user2["website"]     = "http://www.dacteev.com/";
        user2["acl"]         = nullptr;
        user2["active"]      = false;

        response.push_back(user2);

        res_.setStatusCode(reactive::http::status::OK);
        res_.setContentType("application/json");
        res_.setContent(reactive::json::encode(response, true));

    }).method("GET");

    app.match("/api/user/:id", [] (
        const reactive::http::request& req_,
        const reactive::web::router::parameters& params_,
        reactive::http::response& res_
    ) -> void {

        reactive::json::object user;

        user["firstname"]   = "Axel";
        user["lastname"]    = "Etcheverry";
        user["city"]        = "Toulouse";
        user["country"]     = "France";
        user["id"]          = 123456;
        user["website"]     = "http://www.dacteev.com/";

        res_.setStatusCode(reactive::http::status::OK);
        res_.setContentType("application/json");
        res_.setContent(reactive::json::encode(user, true));

    }).method("GET").constrain("id", "[0-9]+");

    app.match("/api/user/:id", [] (
        const reactive::http::request& req_,
        const reactive::web::router::parameters& params_,
        reactive::http::response& res_
    ) -> void {

        reactive::json::object user;
        user["id"]          = params_.get("id");
        user["firstname"]   = "Axel";
        user["lastname"]    = "Etcheverry";
        user["city"]        = "Toulouse";
        user["country"]     = "France";
        user["website"]     = "http://www.dacteev.com/";

        res_.setStatusCode(reactive::http::status::OK);
        res_.setContentType("application/json");
        res_.setContent(reactive::json::encode(user, true));

    }).method("PUT").constrain("id", "[0-9]+");

    reactive::http::server* server = new reactive::http::server(app);
    server->run();

    return 0;
}
