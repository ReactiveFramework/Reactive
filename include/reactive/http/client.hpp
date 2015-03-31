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
#include <uv.h>
#include <future>
#include <reactive/http/request.hpp>
#include <reactive/http/response.hpp>

namespace reactive {
namespace http {

    class client
    {
    public:

        /**
         *
         * @param req_ [description]
         */
        static std::future<response> send(const request& req_);

        /**
         *
         */
        static std::future<response> get(const std::string& url_);

    private:
        struct data_t
        {
            const request* req;
            response* res;
            uv_tcp_t sock;
            uv_connect_t connect_req;
            uv_loop_t* loop;
        };

        /**
         *
         * @param resolver_ [description]
         * @param status_   [description]
         * @param res_      [description]
         */
        static void onResolved(uv_getaddrinfo_t* resolver_, int status_, struct addrinfo* res_);

        /**
         * [onConnect description]
         * @param conn_req_ [description]
         * @param status_   [description]
         */
        static void onConnect(uv_connect_t* conn_req_, int status_);

        /**
         * [onWrite description]
         * @param write_req_ [description]
         * @param status_    [description]
         */
        static void onWrite(uv_write_t* write_req_, int status_);

        /**
         * [onAlloc description]
         * @param handle_         [description]
         * @param suggested_size_ [description]
         * @param buf_            [description]
         */
        static void onAlloc(uv_handle_t* handle_, std::size_t suggested_size_, uv_buf_t* buf_);

        /**
         * [onRead description]
         * @param tcp_handle_ [description]
         * @param nread_      [description]
         * @param buf_        [description]
         */
        static void onRead(uv_stream_t* tcp_handle_, ssize_t nread_, const uv_buf_t* buf_);

        /**
         * [onClose description]
         * @param handle_ [description]
         */
        static void onClose(uv_handle_t* handle_);

    };

} // end of http namespace
} // end of reactive namespace
