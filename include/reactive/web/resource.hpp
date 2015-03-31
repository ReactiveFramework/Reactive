/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <reactive/http/response.hpp>
#include <reactive/http/request.hpp>
#include <reactive/web/router/parameters.hpp>

namespace reactive {
namespace web {

    class resource
    {
    public:
        /**
         * Default virtual destructor
         */
        virtual ~resource() {}

        /**
         * Pre dispatch
         *
         * Put here any action to do before the requested action.
         *
         * @param request_ The request object
         */
        virtual void preDispatch(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const {}

        /**
         * Option action (called for OPTIONS request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void optionsAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Head action (called for HEAD request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void headAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Get action (called for GET request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void getAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Post action (called for POST request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void postAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Put action (called for PUT request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void putAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Delete action (called for DELETE request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void deleteAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Trace action (called for TRACE request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void traceAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Conenct action (called for CONNECT request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void connectAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Patch action (called for PATCH request)
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void patchAction(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        /**
         * Post dispatch
         *
         * Put here any action to post process the response and add some customization.
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        virtual void postDispatch(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const {}

        /**
         * Dispatch action
         *
         * The correct action of this resource will be called according to the
         * request HTTP type. The called action is in charge of filling the
         * response.
         *
         * @param request_  The request object
         * @param response_ The response object
         */
        void dispatch(
            const reactive::http::request& request_,
            const reactive::web::router::parameters& parameters_,
            reactive::http::response& response_
        ) const;
    };

} // end of web namespace
} // end of reactive namespace
