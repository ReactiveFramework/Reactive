/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <sstream>
#include <string>
#include <functional>
#include <mutex>

#include <reactive/web/resource.hpp>
#include <reactive/web/router/route.hpp>
#include <reactive/web/router/context.hpp>
#include <reactive/http/response.hpp>
#include <reactive/http/request.hpp>
#include <reactive/web/router/parameters.hpp>
#include <reactive/logger.hpp>

namespace reactive {
namespace web {

    class application
    {
    private:
        /**
         * Vector of context object
         */
        std::vector<router::context*> m_resources;

    public:
        /**
         * Empty constructor with no command line options
         */
        application() {}

        /**
         * Destructor
         */
        ~application();

        /**
         * Set class resource to a route
         *
         * @param route_    The string of route
         * @param resource_ The resource object
         * @return          The ref of router context
         */
        //reactive::web::router::context& match(const std::string& route_, resource* resource_);
        template<typename T>
        reactive::web::router::context& match(const std::string& route_, T& resource_)
        {
            router::route route(route_);
            router::context* context = new router::context(route);

            context->setResource(resource_);
            context->setMethod("HEAD|GET|POST|PUT|DELETE|OPTIONS|TRACE|CONNECT|PATCH");

            m_resources.push_back(context);

            return (*context);
        }

        /**
         * Set functional resource to a route
         *
         * @param route_    The string of route
         * @param resource_ The resource function
         * @param method_   The string of method
         * @return          The ref of router context
         */
        reactive::web::router::context& match(const std::string& route_, std::function<void (const reactive::http::request&, const reactive::web::router::parameters&, reactive::http::response&)> resource_);

        /**
         * [getResourcesSize description]
         * @return [description]
         */
        std::size_t getResourcesSize() const;

        /**
         * Matches request's route to its correct resource.
         *
         * More exactly the requested url path is match to the first of the
         * context which route correspond (regex) to the url path.
         * Then the request is handled by the corresponding context resource in
         * order to build the response.
         *
         * @param request_   Incoming request
         * @param response_  Outgoing response
         *
         */
        void handleRequest(const reactive::http::request& request_, reactive::http::response& response_) const;

        /** Log line prefix for an application
         *
         * Format used:
         * [app] status-code/bytes-of-response-content/number-of-resources-in-app
         */
        std::string toLog(const reactive::http::response resp_) const;
    };

} // end of web namespace
} // end of reactive namespace
