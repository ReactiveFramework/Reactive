/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/web/resource.hpp>

namespace reactive {
namespace web {

    void resource::dispatch(
        const reactive::http::request& request_,
        const reactive::web::router::parameters& parameters_,
        reactive::http::response& response_
    ) const
    {
        // call preDispatch
        preDispatch(request_, parameters_, response_);

        if (request_.isGet())
        {
            getAction(request_, parameters_, response_);
        }
        else if (request_.isPost())
        {
            postAction(request_, parameters_, response_);
        }
        else if (request_.isPut())
        {
            putAction(request_, parameters_, response_);
        }
        else if (request_.isDelete())
        {
            deleteAction(request_, parameters_, response_);
        }
        else if (request_.isOptions())
        {
            optionsAction(request_, parameters_, response_);
        }
        else if (request_.isTrace())
        {
            traceAction(request_, parameters_, response_);
        }
        else if (request_.isPatch())
        {
            patchAction(request_, parameters_, response_);
        }
        else if (request_.isConnect())
        {
            connectAction(request_, parameters_, response_);
        }
        else
        {
            response_.setStatusCode(reactive::http::status::METHOD_NOT_ALLOWED);
        }

        // call postDispatch
        postDispatch(request_, parameters_, response_);
    }

} // end of web namespace
} // end of reactive namespace
