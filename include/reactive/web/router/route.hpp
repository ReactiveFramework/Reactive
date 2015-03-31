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
#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <reactive/web/router/parameters.hpp>
#include <reactive/logger.hpp>

namespace reactive {
namespace web {
namespace router {

    class route
    {
    private:
        /**
         * The route regex
         */
        boost::regex m_route_regex;

        /**
         * Raw route set up by the developper
         */
        std::string m_route;

        /**
         * Compiled route with regexp info
         */
        std::string m_route_compiled;

        /**
         * True of the raw route is a regular expression
         */
        bool m_is_regex;

        /**
         *
         */
        bool m_is_compiled;

        /**
         *
         */
        std::vector<std::string> m_keys;

        /**
         *
         */
        const std::map<std::string, std::string>* m_constraints;

    public:

        /**
         * Init route
         *
         * @param route_ The string of route
         */
        explicit route(const std::string& route_) : m_route(route_)
        {
            m_constraints = NULL;
            m_is_compiled = false;

            // check if static route
            if (m_route.find(":") == std::string::npos)
            {
                m_is_regex = false;

            }
            else
            {
                m_is_regex = true;
                m_route_compiled = route_;
            }
        }

        ~route();

        /**
         * Get route string
         *
         * @return The string of route
         */
        const std::string& getRoute() const;

        /**
         * Set constraints for route regex
         *
         * "id" => "[0-9]+"
         *
         * @param constraints_ The map string string of constraints
         */
        void setConstraints(const std::map<std::string, std::string>& constraints_);

        /**
         * Compile route regex
         *
         */
        void compile();

        /**
         * Match route
         *
         * @param  path_ The string of path
         * @return       True if route is matched
         */
        bool match(const std::string& path_, parameters& parameters_);
    };

} // end of router namespace
} // end of web namespace
} // end of reactive namespace
