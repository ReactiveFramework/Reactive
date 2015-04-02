/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <cstdlib>
#include <reactive/http/request.hpp>
#include <reactive/logger.hpp>
#include <reactive/net/ip.hpp>
#include <reactive/web/application.hpp>
#include <string>
#include <iostream>
#include <uv.h>

#define REACTIVE_HTTP_SERVER_ASSERT(_name_) \
    if (status != 0)                        \
    {                                       \
        reactive::logger::error(            \
            "%s error: %s - %s",            \
            _name_,                         \
            uv_err_name(status),            \
            uv_strerror(status)             \
        );                                  \
        return;                             \
    }

#define REACTIVE_HTTP_SERVER_ASSERT_EXIT(_name_) \
    if (status != 0)                             \
    {                                            \
        reactive::logger::error(                 \
            "%s error: %s - %s",                 \
            _name_,                              \
            uv_err_name(status),                 \
            uv_strerror(status)                  \
        );                                       \
        return 1;                                \
    }

namespace reactive {
namespace http {

    class server;

    struct connection_t
    {
        uv_tcp_t stream;
        reactive::http::request request;
        server* server_loop;
        uv_write_t write_req;
        bool keep_alive;
    };

    class server
    {
    public:

        /**
         * Constructor from an application
         *
         * @param app_      Web application which links routes to resources
         * thanks to contexts and also has command line options information.
         */
        server(reactive::web::application& app_) :
            m_loop(NULL),
            m_application(&app_),
            m_default_keep_alive_timeout(15),
            //m_host("127.0.0.1"),
            m_host("0.0.0.0"),
            m_port(8080)
        {
        }

        ~server()
        {
            if (m_loop != NULL)
            {
                stop();
            }
        }

        /**
         * Set keep alive timeout
         *
         * @param timeout_ The timeout value
         */
        void setKeepAliveTimeout(const int timeout_);

        /**
         * Get keep alive timeout
         *
         * @return The timeout value
         */
        const int getKeepAliveTimeout() const;

        /**
         * Get application
         *
         * @return The pointer of application
         */
        reactive::web::application* getApplication();

        /**
         * Run loop
         *
         * @return status
         */
        int run();

        /**
         * Stop server
         */
        void stop();

        /**
         *
         * @param port_ [description]
         */
        void listen(unsigned short port_);

        /**
         *
         * @param port_ [description]
         * @param host_ [description]
         */
        void listen(unsigned short port_, const std::string& host_);

        /**
         *
         * @param host_ [description]
         */
        void listen(const std::string& host_);

        std::string toLog()
        {
            return "";
        }

        /**
         * Get server loop
         *
         * @return [description]
         */
        uv_loop_t* getLoop();

    private:

        /**
         *
         */
        uv_loop_t* m_loop;

        /**
         *
         */
        uv_tcp_t m_handle;

        /**
         *
         */
        uv_signal_t m_signal;

        /**
         *
         */
        reactive::web::application* m_application;

        /**
         *
         */
        int m_default_keep_alive_timeout;

        /**
         * Host
         */
        std::string m_host;

        /**
         * Port
         */
        unsigned short m_port;

        /**
         *
         * @param handle_ [description]
         * @param arg_    [description]
         */
        static void onCloseWalk(uv_handle_t* handle_, void* arg_);

        /**
         * Alloc buffer size
         *
         * @param handle_         [description]
         * @param suggested_size_ [description]
         * @param buf_            [description]
         */
        static void onAlloc(uv_handle_t* handle_, std::size_t suggested_size_, uv_buf_t* buf_);

        /**
         * Accept new connection
         *
         * @param stream_ [description]
         * @param status_ [description]
         */
        static void onConnect(uv_stream_t* stream_, int status_);

        /**
         * Read request
         *
         * @param tcp_   [description]
         * @param nread_ [description]
         * @param buf_   [description]
         */
        static void onAfterRead(uv_stream_t* tcp_, ssize_t nread_, const uv_buf_t* buf_);

        /**
         * Close write
         *
         * @param req_    [description]
         * @param status_ [description]
         */
        static void onAfterWrite(uv_write_t* req_, int status_);

        /**
         * Close connection
         *
         * @param handle_ [description]
         */
        static void onClose(uv_handle_t* peer_);

        /**
         * Exit server
         *
         * @param handle_ [description]
         * @param signum_ [description]
         */
        static void onExit(uv_signal_t* handle_, int signum_);

        /**
         * [processLocalIp description]
         * @param  connection_ [description]
         * @return             [description]
         */
        static int processLocalIp(connection_t* connection_);

        /**
         * [processRemoteIp description]
         * @param  connection_ [description]
         * @return             [description]
         */
        static int processRemoteIp(connection_t* connection_);
    };


} // end of http namespace
} // end of reactive namespace
