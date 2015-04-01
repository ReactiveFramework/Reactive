Reactive Framework [![Build Status](https://travis-ci.org/ReactiveFramework/Reactive.svg?branch=master)](https://travis-ci.org/ReactiveFramework/Reactive)
==================

![alt text](https://raw.githubusercontent.com/ReactiveFramework/Reactive/master/_resources/logo.png "Reactive logo")

Reactive is a C++11 asynchronous http server framework.

Example
-------

lambda version
~~~cpp
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

    reactive::http::server* server = new reactive::http::server(app);
    server->run();

    return 0;
}
~~~

class version
~~~cpp
#include <reactive/http/request.hpp>
#include <reactive/http/response.hpp>
#include <reactive/web/application.hpp>
#include <reactive/web/resource.hpp>
#include <reactive/http/server.hpp>
#include <reactive/web/router/parameters.hpp>
#include <reactive/json/encode.hpp>
#include <reactive/uri/query.hpp>

class api_users_resource: public reactive::web::resource
{
public:
    void getAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
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

        response_.setStatusCode(reactive::http::status::OK);
        response_.setContentType("application/json");
        response_.setContent(reactive::json::encode(response, true));
    }
};

class api_user_resource: public reactive::web::resource
{
public:
    void getAction(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        reactive::json::object user;

        user["firstname"]   = "Axel";
        user["lastname"]    = "Etcheverry";
        user["city"]        = "Toulouse";
        user["country"]     = "France";
        user["id"]          = 123456;
        user["website"]     = "http://www.dacteev.com/";

        response_.setStatusCode(reactive::http::status::OK);
        response_.setContentType("application/json");
        response_.setContent(reactive::json::encode(user, true));
    }
};

int main(int argc, char const *argv[])
{
    api_users_resource* users = new api_users_resource();
    api_user_resource* user = new api_user_resource();

    reactive::web::application app;

    app.match("/api/user", *users);
    app.match("/api/user/:id", *user).constrain("id", "[0-9]+");

    reactive::http::server* server = new reactive::http::server(app);
    server->run();

    delete users;
    delete user;

    return 0;
}

~~~

Dependencies
------------

* GCC 4.8
* scons
* boost
* cryptopp
* http_parser
* libuv
* curl
* icu

~~~shell
emerge -av dev-libs/boost dev-libs/icu dev-libs/libuv net-libs/http-parser net-misc/curl dev-libs/crypto++ dev-util/scons
~~~

Build
-----

~~~shell
make release
~~~

Test
----

~~~shell
make test
~~~

