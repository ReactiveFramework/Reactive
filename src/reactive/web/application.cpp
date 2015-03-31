/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/web/application.hpp>

namespace reactive {
namespace web {

    application::~application()
    {
        for (std::size_t i = 0; i < m_resources.size(); ++i)
        {
            delete m_resources[i];
        }

        m_resources.clear();
    }

    reactive::web::router::context& application::match(const std::string& route_, std::function<void (const reactive::http::request&, const reactive::web::router::parameters&, reactive::http::response&)> resource_)
    {
        router::route route(route_);
        router::context* context = new router::context(route);

        // Setting parameters for the newly built context
        // this includes implicit definition of the method.
        context->setResource(std::move(resource_));

        m_resources.push_back(context);

        return (*context);
    }

    std::size_t application::getResourcesSize() const
    {
        return m_resources.size();
    }

    void application::handleRequest(const reactive::http::request& request_, reactive::http::response& response_) const
    {
        reactive::web::router::parameters parameters_map;

        // Match the request url path to one of the registred application's
        // context (route + resource)
        for (std::size_t i = 0; i < m_resources.size(); ++i)
        {
            if (m_resources[i]->match(request_, parameters_map))
            {
                if (!(m_resources[i]->handleRequest(request_, parameters_map, response_)))
                {
                    // The fallback response depends on the handler
                    // Here we set the status code only
                    response_.setStatusCode(reactive::http::status::INTERNAL_SERVER_ERROR);
                    reactive::logger::error(toLog(response_) + " error in selected request handler.");
                }

                // The first matching context is the only one used for now.
                reactive::logger::info(toLog(response_) + "ok, " + request_.toLog());
                return;
            }
        }

        response_.setStatusCode(reactive::http::status::NOT_FOUND);

        reactive::logger::error(toLog(response_) + "no handler found for request, " + request_.toLog());
    }

    std::string application::toLog(const reactive::http::response resp_) const
    {
        std::stringstream output;

        output << "[reactive-" << "8080" << "] (";
        output << resp_.getStatusCode();
        output << ")/";
        output << resp_.getContent().length();
        output << "/";
        output << m_resources.size();
        output << ", ";

        return output.str();
    }

} // end of web namespace
} // end of reactive namespace
