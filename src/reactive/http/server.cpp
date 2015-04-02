/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/http/server.hpp>

namespace reactive {
namespace http {

    void server::setKeepAliveTimeout(const int timeout_)
    {
        m_default_keep_alive_timeout = timeout_;
    }

    const int server::getKeepAliveTimeout() const
    {
        return m_default_keep_alive_timeout;
    }

    reactive::web::application* server::getApplication()
    {
        return m_application;
    }

    int server::run()
    {
        int status;

        if (m_loop == NULL)
        {
            m_loop = uv_loop_new();
        }

        m_signal.data = this;

        status = uv_signal_init(m_loop, &m_signal);

        REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_signal_init");

        status = uv_signal_start(&m_signal, onExit, SIGINT);

        REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_signal_start");

        uv_async_t service_handle;
        //uv_async_t* service_handle = 0;

        status = uv_tcp_init(m_loop, &m_handle);

        REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_tcp_init");

        //service_handle = (uv_async_t*)malloc(sizeof(uv_async_t));
        status = uv_async_init(m_loop, &service_handle, NULL);

        REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_async_init");

        /*if (m_host == "0.0.0.0")
        {
            uv_interface_address_t* info;
            int count, i;

            status = uv_interface_addresses(&info, &count);

            REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_interface_addresses");

            i = count;

            while (i--)
            {
                uv_interface_address_t interface = info[i];

                if (interface.is_internal == false)
                {
                    continue;
                }

                if (interface.address.address4.sin_family == AF_INET)
                {
                    interface.address.address4.sin_port = htons(m_port);
                    status = uv_tcp_bind(
                        &m_handle,
                        (const struct sockaddr*)&interface.address.address4,
                        0
                    );

                    REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_tcp_bind");
                }
                else if (interface.address.address4.sin_family == AF_INET6)
                {
                    interface.address.address6.sin6_port = htons(m_port);
                    status = uv_tcp_bind(
                        &m_handle,
                        (const struct sockaddr*)&interface.address.address6,
                        0
                    );

                    REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_tcp_bind");
                }
            }

            uv_free_interface_addresses(info, count);
        }
        else */if (reactive::net::ip::is_v4(m_host))
        {
            struct sockaddr_in listen_address;

            uv_ip4_addr(
                m_host.c_str(),
                m_port,
                &listen_address
            );

            status = uv_tcp_bind(&m_handle, (const struct sockaddr*)&listen_address, 0);

            REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_tcp_bind");
        }
        else if (reactive::net::ip::is_v6(m_host))
        {
            struct sockaddr_in6 listen_address;

            uv_ip6_addr(
                m_host.c_str(),
                m_port,
                &listen_address
            );

            status = uv_tcp_bind(&m_handle, (const struct sockaddr*)&listen_address, 0);

            REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_tcp_bind");
        }
        else
        {
            reactive::logger::error("ip address %s is invalid.", m_host.c_str());
            return 1;
        }

        m_handle.data = this;

        status = uv_listen((uv_stream_t*)&m_handle, 128, onConnect);

        REACTIVE_HTTP_SERVER_ASSERT_EXIT("uv_listen");

        uv_unref((uv_handle_t*)&m_handle);

        reactive::logger::info(
            "Listening on %s:%d",
            m_host.c_str(),
            m_port
        );

        return uv_run(m_loop, UV_RUN_DEFAULT);
    }

    void server::stop()
    {
        reactive::logger::info("Stop server");

        int status = uv_signal_stop(&m_signal);

        REACTIVE_HTTP_SERVER_ASSERT("uv_signal_stop");

        uv_close((uv_handle_t*)&m_handle, NULL);

        if (m_loop != NULL)
        {
            uv_walk(m_loop, onCloseWalk, NULL);
            uv_run(m_loop, UV_RUN_DEFAULT);
            //uv_loop_delete(m_loop);
            m_loop = NULL;
        }
    }

    void server::listen(unsigned short port_)
    {
        m_port = port_;
    }

    void server::listen(unsigned short port_, const std::string& host_)
    {
        m_port = port_;
        m_host = host_;
    }

    void server::listen(const std::string& host_)
    {
        m_host = host_;
    }

    void server::onCloseWalk(uv_handle_t* handle_, void* arg_)
    {
        if (!uv_is_closing(handle_))
        {
            uv_close(handle_, NULL);
        }
    }

    void server::onAlloc(uv_handle_t* handle_, std::size_t suggested_size_, uv_buf_t* buf_)
    {
        *buf_ = uv_buf_init((char*) malloc(suggested_size_), suggested_size_);
    }

    void server::onConnect(uv_stream_t* stream_, int status_)
    {
        if (status_ != 0)
        {
            reactive::logger::error(
                "Connect error on %s at %d error: %s - %s",
                __FILE__,
                __LINE__,
                uv_err_name(status_),
                uv_strerror(status_)
            );
            uv_close((uv_handle_t*) stream_, NULL);
            return;
        }

        int status;

        connection_t* connection = new connection_t;

        server* serv = (server*)stream_->data;

        status = uv_tcp_init(serv->getLoop(), &connection->stream);

        REACTIVE_HTTP_SERVER_ASSERT("uv_tcp_init");

        connection->server_loop = (server*)stream_->data;

        connection->stream.data = connection;

        status = uv_accept(stream_, (uv_stream_t*)&connection->stream);

        if (status != 0)
        {
            reactive::logger::error(
                "uv_accept on %s at %d error: %s - %s",
                __FILE__,
                __LINE__,
                uv_err_name(status),
                uv_strerror(status)
            );

            uv_close(
                (uv_handle_t*) &connection->stream,
                onClose
            );

            return;
        }

        status = processLocalIp(connection);

        if (status != 0)
        {
            return;
        }

        status = processRemoteIp(connection);

        if (status != 0)
        {
            return;
        }

        status = uv_read_start(
            (uv_stream_t*)&connection->stream,
            onAlloc,
            onAfterRead
        );

        if (status != 0)
        {
            reactive::logger::error(
                "uv_read_start on %s at %d error: %s - %s",
                __FILE__,
                __LINE__,
                uv_err_name(status),
                uv_strerror(status)
            );

            uv_close(
                (uv_handle_t*) &connection->stream,
                onClose
            );

            return;
        }
    }

    void server::onAfterRead(uv_stream_t* tcp_, ssize_t nread_, const uv_buf_t* buf_)
    {
        connection_t* connection = (connection_t*)tcp_->data;

        if (nread_ > 0)
        {
            connection->request.parse(buf_->base, nread_);

            connection->keep_alive = connection->request.shouldKeepAlive();

            reactive::http::response response(reactive::http::status::OK);

            if (connection->keep_alive)
            {
                uv_tcp_keepalive((uv_tcp_t*)tcp_, 1, connection->server_loop->getKeepAliveTimeout());

                response.addHeader("Connection", "keep-alive");
            }
            else
            {
                response.addHeader("Connection", "close");
            }

            connection->server_loop->getApplication()->handleRequest(connection->request, response);

            uv_write_t* write_req = (uv_write_t *)malloc(sizeof(*write_req) + sizeof(uv_buf_t));
            uv_buf_t* resbuf = (uv_buf_t *)(write_req+1);

            std::string res = response.toString();

            resbuf->base = (char*)res.c_str();
            resbuf->len = res.length();

            write_req->data = connection;

            /* TODO: Use the return values from uv_write() */
            uv_write(
                write_req,
                (uv_stream_t*)&connection->stream,
                resbuf,
                1,
                onAfterWrite
            );
        }
        else
        {
            uv_close(
                (uv_handle_t*) &connection->stream,
                onClose
            );
        }

        if (buf_->base)
        {
            free(buf_->base);
        }
    }

    void server::onAfterWrite(uv_write_t* req_, int status_)
    {
        connection_t* connection = (connection_t*)req_->data;

        if (!connection->keep_alive)
        {
            uv_close((uv_handle_t*)req_->handle, onClose);
        }

        if (status_ != 0)
        {
            reactive::logger::error(
                "uv_write error: %s - %s",
                uv_err_name(status_),
                uv_strerror(status_)
            );

            if (!uv_is_closing((uv_handle_t*) req_->handle))
            {
                uv_close((uv_handle_t*)req_->handle, onClose);
            }
        }

        free(req_);
    }

    void server::onClose(uv_handle_t* peer_)
    {
        connection_t* conn = (connection_t*)peer_->data;
        delete conn;
    }

    void server::onExit(uv_signal_t* handle_, int signum_)
    {
        server* data = (server*)handle_->data;

        data->stop();
    }

    uv_loop_t* server::getLoop()
    {
        return m_loop;
    }

    int server::processLocalIp(connection_t* connection_)
    {
        struct sockaddr sockname;
        int namelen = sizeof(sockname);
        int status = 0;

        status = uv_tcp_getsockname(&(connection_->stream), &sockname, &namelen);

        if (status != 0)
        {
            reactive::logger::error(
                "uv_tcp_getsockname on %s at %d error: %s - %s",
                __FILE__,
                __LINE__,
                uv_err_name(status),
                uv_strerror(status)
            );

            uv_close(
                (uv_handle_t*) &(connection_->stream),
                onClose
            );

            return status;
        }

        if (sockname.sa_family == AF_INET)
        {
            struct sockaddr_in* local_addr = (struct sockaddr_in*)&sockname;
            char ipv4[17];
            status = uv_ip4_name(local_addr, (char*)ipv4, sizeof(ipv4));

            if (status != 0)
            {
                reactive::logger::error(
                    "uv_ip4_name on %s at %d error: %s - %s",
                    __FILE__,
                    __LINE__,
                    uv_err_name(status),
                    uv_strerror(status)
                );

                uv_close(
                    (uv_handle_t*) &connection_->stream,
                    onClose
                );

                return status;
            }

            connection_->request.info.local_ip_version = reactive::net::ip::IPV4;
            connection_->request.info.local_ip         = std::string(ipv4);
            connection_->request.info.local_port       = std::to_string(ntohs(local_addr->sin_port));
        }
        else
        {
            struct sockaddr_in6* local_addr = (struct sockaddr_in6*)&sockname;
            char ipv6[47];
            status = uv_ip6_name(local_addr, (char*)ipv6, sizeof(ipv6));

            if (status != 0)
            {
                reactive::logger::error(
                    "uv_ip6_name on %s at %d error: %s - %s",
                    __FILE__,
                    __LINE__,
                    uv_err_name(status),
                    uv_strerror(status)
                );

                uv_close(
                    (uv_handle_t*) &(connection_->stream),
                    onClose
                );

                return status;
            }

            connection_->request.info.local_ip_version = reactive::net::ip::IPV6;
            connection_->request.info.local_ip         = std::string(ipv6);
            connection_->request.info.local_port       = std::to_string(ntohs(local_addr->sin6_port));
        }

        return 0;
    }

    int server::processRemoteIp(connection_t* connection_)
    {
        struct sockaddr peername;
        int namelen = sizeof(peername);
        int status = 0;

        status = uv_tcp_getpeername(&(connection_->stream), &peername, &namelen);

        if (status != 0)
        {
            reactive::logger::error(
                "uv_tcp_getpeername on %s at %d error: %s - %s",
                __FILE__,
                __LINE__,
                uv_err_name(status),
                uv_strerror(status)
            );

            uv_close(
                (uv_handle_t*) &(connection_->stream),
                onClose
            );

            return status;
        }

        if (peername.sa_family == AF_INET)
        {
            struct sockaddr_in* remote_addr = (struct sockaddr_in*)&peername;
            char ipv4[17];
            status = uv_ip4_name(remote_addr, (char*)ipv4, sizeof(ipv4));

            if (status != 0)
            {
                reactive::logger::error(
                    "uv_ip4_name on %s at %d error: %s - %s",
                    __FILE__,
                    __LINE__,
                    uv_err_name(status),
                    uv_strerror(status)
                );

                uv_close(
                    (uv_handle_t*) &(connection_->stream),
                    onClose
                );

                return status;
            }

            connection_->request.info.ip_version = reactive::net::ip::IPV4;
            connection_->request.info.ip         = std::string(ipv4);
            connection_->request.info.port       = std::to_string(ntohs(remote_addr->sin_port));
        }
        else
        {
            struct sockaddr_in6* remote_addr = (struct sockaddr_in6*)&peername;
            char ipv6[47];
            status = uv_ip6_name(remote_addr, (char*)ipv6, sizeof(ipv6));

            if (status != 0)
            {
                reactive::logger::error(
                    "uv_ip6_name on %s at %d error: %s - %s",
                    __FILE__,
                    __LINE__,
                    uv_err_name(status),
                    uv_strerror(status)
                );

                uv_close(
                    (uv_handle_t*) &(connection_->stream),
                    onClose
                );

                return status;
            }

            connection_->request.info.ip_version = reactive::net::ip::IPV6;
            connection_->request.info.ip         = std::string(ipv6);
            connection_->request.info.port       = std::to_string(ntohs(remote_addr->sin6_port));
        }

        return 0;
    }

} // end of http namespace
} // end of reactive namespace
