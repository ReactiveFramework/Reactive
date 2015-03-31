/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <reactive/web/router/route.hpp>

namespace reactive {
namespace web {
namespace router {

    route::~route()
    {
        m_keys.clear();
    }

    const std::string& route::getRoute() const
    {
        return m_route;
    }

    void route::setConstraints(const std::map<std::string, std::string>& constraints_)
    {
        m_constraints = &constraints_;
    }

    void route::compile()
    {
        if (m_is_regex && (m_route_compiled.find(":") != std::string::npos))
        {
            std::string varname;

            bool is_variable = false;

            for (std::size_t i = 0; i < m_route.size(); i++)
            {
                char chr = m_route[i];

                if (chr == ':')
                {
                    is_variable = true;
                    continue;
                }

                if (is_variable)
                {
                    if ((chr >= 'a' && chr <= 'z') || (chr >= '0' && chr <= '9') || chr == '_')
                    {
                        varname += chr;
                    }
                    else
                    {
                        is_variable = false;

                        m_keys.push_back(varname);
                        varname.clear();

                        continue;
                    }
                }
            }

            m_keys.push_back(varname);
            varname.clear();

            // replace varname by regex
            for (std::size_t j = 0; j < m_keys.size(); j++)
            {
                std::string key = m_keys[j];

                std::size_t pos = m_route_compiled.find(":" + key);

                if (pos != std::string::npos)
                {
                    if (m_constraints->count(key) > 0)
                    {
                        m_route_compiled.replace(pos, key.length() + 1, "(" + m_constraints->at(key) + ")");
                    }
                    else
                    {
                        m_route_compiled.replace(pos, key.length() + 1, "([a-zA-Z0-9\\-_]+)");
                    }
                }
            }

            m_route_compiled = "^" + m_route_compiled + "$";

            if (m_route_compiled.find(":") == std::string::npos)
            {
                m_is_compiled = true;

                #ifdef DEBUG
                // This test is long so it is activated only in debug mode
                reactive::logger::debug("route: compiling route " + m_route + " -to- " + m_route_compiled);
                #endif
            }

            boost::regex r(m_route_compiled, boost::regex::perl|boost::regex::optimize);
            m_route_regex = r;
        }
    }

    bool route::match(const std::string& path_, parameters& parameters_)
    {
        if (m_is_regex)
        {

            if (m_is_compiled == false)
            {
                compile();
            }

            bool retval;

            boost::cmatch matches;

            retval = boost::regex_match(path_.c_str(), matches, m_route_regex);

            for (std::size_t i = 1; i < matches.size(); i++)
            {
                std::string match(matches[i].first, matches[i].second);

                if (m_keys.size() >= i)
                {
                    parameters_[m_keys[i-1]] = match;
                }
            }

            return retval;
        }

        // implicit else:
        return (m_route.compare(path_) == 0);
    }

} // end of router namespace
} // end of web namespace
} // end of reactive namespace
