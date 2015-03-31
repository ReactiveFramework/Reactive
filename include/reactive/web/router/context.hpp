/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <iostream>
#include <string>
#include <set>
#include <functional>
#include <algorithm>
#include <exception>
#include <reactive/web/resource.hpp>
#include <reactive/web/router/route.hpp>
#include <reactive/http/response.hpp>
#include <reactive/http/request.hpp>
#include <reactive/logger.hpp>

#include <cxxabi.h>

namespace reactive {
namespace web {
namespace router {

    typedef enum {
        TYPE_UNKNOWN,
        TYPE_FUNCTIONAL,
        TYPE_OBJECT
    } context_type_t;

    class context
    {
    private:
        /**
         * The route object
         */
        route m_route;

        /**
         * The method list
         */
        std::set<std::string> m_methods;

        /**
         * The function binded for route
         */
        std::function<void (const reactive::http::request&, const parameters&, reactive::http::response&)> m_func;

        /**
         * The class resource binded for route
         */
        reactive::web::resource* m_class;

        /**
         * Type of resource (func or resource)
         */
        context_type_t m_type;

        /**
         *
         */
        std::map<std::string, std::string> m_constraints;

    public:

        context(const reactive::web::router::route& route_) :
            m_route(route_),
            m_class(NULL),
            m_type(TYPE_UNKNOWN)
        {
            m_route.setConstraints(m_constraints);
        }

        context(const context&) = delete;
        context& operator=(const context&) = delete;

        ~context();

        /**
         * Get type of context
         *
         * @return The type number
         */
        const context_type_t& getType() const;

        /**
         * Set method in context
         *
         * @param method_ The method name
         */
        void setMethod(const std::string& method_);

        /**
         * Set method in context
         *
         * @param method_ The method name
         * @return        The ref of this context
         */
        context& method(const std::string& method_);

        /**
         * Check if method_ exists in context
         *
         * @param  method_ The method name
         * @return         True if method_ exists in context
         */
        bool hasMethod(const std::string& method_);

        /**
         * Add constraint for this context
         *
         * @param  key_     The string of key
         * @param  pattern_ The string of regex pattern
         * @return          The ref of this context
         */
        context& constrain(const std::string& key_, const std::string& pattern_);

        /**
         * Set functional resource
         *
         * @param resource_ The functional of resource
         */
        void setResource(std::function<void (const reactive::http::request&, const parameters&, reactive::http::response&)> resource_);

        /**
         * Set class resource
         *
         * @param resource_ The class of resource
         */
        template<typename T>
        void setResource(T& resource_)
        {
            m_class = &resource_;
            m_type = TYPE_OBJECT;
        }

        /**
         * Get class resource
         *
         * @return The class of resource
         */
        reactive::web::resource& getClass();

        /**
         * Get function resource
         *
         * @return The function of resource
         */
        std::function<void (const reactive::http::request&, const parameters&, reactive::http::response&)>& getFunction();

        /**
         * Check if request_ is match with context
         *
         * @param  request_ The request object
         * @return          True if metched with context
         */
        bool match(const reactive::http::request& request_, parameters& parameters_);

        /**
         * Handle a request according to the the context type
         *
         * A response is built according to the fact that this current context
         * has been set with a resource function or a resource class.
         * It is possible that the response R is unchanged at the end.
         *
         * @param request_    Incoming request this context has to manage
         * @param parameters_
         * @param response_   Output response
         * @return True if the response has been set, else False
         */
        bool handleRequest(const reactive::http::request& request_, parameters& parameters_, reactive::http::response& response_) const;

        template<typename T>
        std::string toLogException(const T& ex_, const reactive::http::request& request_) const
        {
            std::string retval;

            int expt_status;
            char* expt_demangled = abi::__cxa_demangle(typeid(ex_).name(), 0, 0, &expt_status);

            if (m_type == TYPE_OBJECT)
            {
                std::string method = request_.getMethod();

                std::transform(
                    method.begin(),
                    method.end(),
                    method.begin(),
                    ::tolower
                );

                int class_status;
                char* class_demangled = abi::__cxa_demangle(typeid(*m_class).name(), 0, 0, &class_status);

                retval += class_demangled;
                retval += "::";
                retval += method;
                retval += "Action(...) => ";
                retval += expt_demangled;
                retval += " : ";
                retval += ex_.what();

                free(class_demangled);
            }
            else
            {
                std::string methods;
                std::set<std::string>::iterator it = m_methods.begin();

                while (it != m_methods.end())
                {
                    methods += (*it);

                    ++it;

                    if (it != m_methods.end())
                    {
                        methods += "|";
                    }
                }

                retval += "app.match(\"";
                retval += m_route.getRoute();
                retval += "\", ...).method(\"";
                retval += methods;
                retval += "\") => ";
                retval += expt_demangled;
                retval += " : ";
                retval += ex_.what();
            }

            free(expt_demangled);

            return retval;
        }
    };

} // end of router namespace
} // end of web namespace
} // end of reactive namespace
