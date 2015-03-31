/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/web/router/context.hpp>

namespace reactive {
namespace web {
namespace router {

    context::~context()
    {
        m_methods.clear();
        m_constraints.clear();
    }

    const context_type_t& context::getType() const
    {
        return m_type;
    }

    void context::setMethod(const std::string& method_)
    {
        if (method_.find("|") != std::string::npos)
        {
            std::string method;

            for (std::size_t i = 0; i < method_.length(); ++i)
            {
                char chr = method_[i];

                if (chr == '|')
                {
                    m_methods.insert(method);
                    method.clear();
                    continue;
                }

                method += chr;
            }

            m_methods.insert(method);
            method.clear();
        }
        else
        {
            m_methods.insert(method_);
        }
    }


    context& context::method(const std::string& method_)
    {
        setMethod(method_);

        return (*this);
    }

    bool context::hasMethod(const std::string& method_)
    {
        return (m_methods.find(method_) != m_methods.end());
    }

    context& context::constrain(const std::string& key_, const std::string& pattern_)
    {
        m_constraints[key_] = pattern_;

        m_route.compile();

        return (*this);
    }

    void context::setResource(std::function<void (const reactive::http::request&, const parameters&, reactive::http::response&)> resource_)
    {
        m_func = std::move(resource_);
        m_type = TYPE_FUNCTIONAL;
    }

    reactive::web::resource& context::getClass()
    {
        return *m_class;
    }

    std::function<void (const reactive::http::request&, const parameters&, reactive::http::response&)>& context::getFunction()
    {
        return m_func;
    }

    bool context::match(const reactive::http::request& request_, parameters& parameters_)
    {
        // check if Context match to request method
        if (!hasMethod(request_.getMethod()))
        {
            return false;
        }

        return m_route.match(request_.getUrl().getPath(), parameters_);
    }

    bool context::handleRequest(const reactive::http::request& request_, parameters& parameters_, reactive::http::response& response_) const
    {
        try
        {
            if (m_type == TYPE_OBJECT)
            {
                // Build the response using the ressource class
                m_class->dispatch(request_, parameters_, response_);
                return true;
            }
            else
            {
                // Build the response using the ressource function
                m_func(request_, parameters_, response_);
                return true;
            }
        }
        catch (reactive::http::exception httpex)
        {
            response_.setStatusCode(httpex.getHttpStatusCode());

            reactive::logger::error(toLogException(httpex, request_));
        }
        catch (std::exception& expt)
        {
            response_.setStatusCode(reactive::http::status::INTERNAL_SERVER_ERROR);

            reactive::logger::error(toLogException(expt, request_));
        }

        // Request could not be handled
        return false;
    }

} // end of router namespace
} // end of web namespace
} // end of reactive namespace
