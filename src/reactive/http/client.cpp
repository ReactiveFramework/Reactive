/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/client.hpp>
#include <stdexcept>
#include <iostream>
#include <uv.h>

namespace reactive {
namespace http {

    void client::onResolved(uv_getaddrinfo_t* resolver_, int status_, struct addrinfo* res_)
    {
        if (status_ == -1)
        {
            throw std::runtime_error(uv_err_name(status_));
        }

        int r = 0;

        data_t* data = (data_t*)resolver_->data;

        //char addr[17] = {'\0'};
        //uv_ip4_name((struct sockaddr_in*)res_->ai_addr, addr, 16);

        //@TODO add ip6 support
        //std::cout << "Resolved ip : " << addr << std::endl;

        uv_tcp_init(data->loop, &data->sock);

        data->connect_req.data = data;

        r = uv_tcp_connect(
            &(data->connect_req),
            &(data->sock),
            (const sockaddr*)res_->ai_addr,
            onConnect
        );

        if (r)
        {
            throw std::runtime_error(uv_err_name(r));
        }

        uv_freeaddrinfo(res_);
    }

    void client::onConnect(uv_connect_t* conn_req_, int status_)
    {
        if (status_ == -1)
        {
            uv_close((uv_handle_t*)conn_req_->handle, onClose);
            throw std::runtime_error(uv_err_name(status_));
        }

        data_t* data = (data_t*)conn_req_->data;

        uv_write_t* req;
        uv_buf_t buf;
        int r;

        std::string request = data->req->toString();

        buf = uv_buf_init((char*)request.c_str(), request.length());

        req = (uv_write_t*)malloc(sizeof *req);

        if (req == NULL)
        {
            throw std::runtime_error("Error malloc.");
        }

        r = uv_write(req, conn_req_->handle, &buf, 1, onWrite);

        if (r)
        {
            throw std::runtime_error(uv_err_name(r));
        }

        conn_req_->handle->data = data;

        r = uv_read_start(conn_req_->handle, onAlloc, onRead);

        if (r)
        {
            throw std::runtime_error(uv_err_name(r));
        }
    }

    void client::onWrite(uv_write_t* write_req_, int status_)
    {
        free(write_req_);

        if (status_)
        {
            throw std::runtime_error(uv_err_name(status_));
        }
    }

    void client::onAlloc(uv_handle_t* handle_, std::size_t suggested_size_, uv_buf_t* buf_)
    {
        *buf_ = uv_buf_init((char*) malloc(suggested_size_), suggested_size_);
    }

    void client::onRead(uv_stream_t* tcp_handle_, ssize_t nread_, const uv_buf_t* buf_)
    {
        data_t* data = (data_t*)tcp_handle_->data;

        if (nread_ < 0)
        {
            uv_close((uv_handle_t*)tcp_handle_, onClose);
        }

        if (nread_ > 0)
        {
            ssize_t parsed = (ssize_t)data->res->parse(buf_->base, (std::size_t)nread_);

            if (parsed != nread_)
            {
                throw std::runtime_error("Http Parse Error");
            }
        }

        if (data->res->isComplete())
        {
            uv_close((uv_handle_t*)tcp_handle_, onClose);
        }

        if (buf_->base)
        {
            free(buf_->base);
        }
    }

    void client::onClose(uv_handle_t* handle_)
    {
        //free(handle_);
    }

    std::future<response> client::send(const request& req_)
    {
        return std::async(std::launch::async, [=]() -> response {
            response res;

            struct addrinfo hints;

            hints.ai_family     = PF_INET;
            hints.ai_socktype   = SOCK_STREAM;
            hints.ai_protocol   = IPPROTO_TCP;
            hints.ai_flags      = 0;

            uv_getaddrinfo_t resolver;

            data_t data;
            data.req    = &req_;
            data.res    = &res;
            data.loop   = uv_loop_new();

            resolver.data = &data;

            int r = uv_getaddrinfo(
                data.loop,
                &resolver,
                onResolved,
                req_.getUrl().getHost().c_str(),
                std::to_string(req_.getUrl().getPort()).c_str(),
                &hints
            );

            if (r)
            {
                throw std::runtime_error(uv_err_name(r));
            }

            uv_run(data.loop, UV_RUN_DEFAULT);

            uv_loop_delete(data.loop);

            return res;
        });
    }


    std::future<response> client::get(const std::string& url_)
    {
        request req;
        req.setMethod("GET");
        req.setUrl(url_);

        return send(req);
    }

} // end of http namespace
} // end of reactive namespace
