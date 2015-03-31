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
#include <vector>
#include <string>
#include <boost/asio/ip/address.hpp>
#include <reactive/http/cookie_bag.hpp>
#include <reactive/http/header_bag.hpp>
#include <reactive/http/protocol.hpp>
#include <reactive/http/exception.hpp>
#include <reactive/uri/decode.hpp>
#include <reactive/uri/encode.hpp>
#include <reactive/uri/url.hpp>
#include <reactive/uri/query.hpp>
#include <reactive/json/decode.hpp>
#include <reactive/net/ip.hpp>
#include <reactive/string.hpp>
#include <http_parser.h>
#include <cstring>
#include <exception>

#define REACTIVE_HTTP_REQUEST_USER_AGENT "Reactive (+https://github.com/euskadi31/reactive)"

namespace reactive {
namespace http {

    class request
    {
    public:
        /**
         * Information about the request
         */
        typedef struct info_t
        {
            /**
             * Version of ip (IPV4 or IPV6)
             */
            reactive::net::ip::version_t ip_version = reactive::net::ip::UNDEFINED;

            /**
             * Address if the client making the request
             */
            std::string ip;

            /**
             * Port of the remote client
             */
            std::string port;

            /**
             * Set if the request passes through a proxy
             */
            bool by_proxy = false;

            /**
             * Version of proxy ip (IPV4 or IPV6)
             */
            reactive::net::ip::version_t proxy_ip_version = reactive::net::ip::UNDEFINED;

            /**
             * Proxy ip
             */
            std::string proxy_ip;

            /**
             * Proxy port
             */
            std::string proxy_port;

            /**
             * Version of local ip (IPV4 or IPV6)
             */
            reactive::net::ip::version_t local_ip_version = reactive::net::ip::UNDEFINED;

            /**
             * Host ip (local server)
             */
            std::string local_ip;

            /**
             * Port of the server
             */
            std::string local_port;

            /**
             * Reset all parameters to default values
             */
            void reset()
            {
                ip_version = reactive::net::ip::UNDEFINED;
                ip         = "";
                port       = "";

                by_proxy         = false;
                proxy_ip_version = reactive::net::ip::UNDEFINED;
                proxy_ip         = "";
                proxy_port       = "";

                local_ip_version = reactive::net::ip::UNDEFINED;
                local_ip         = "";
                local_port       = "";
            }
        } info_t;


    private:
        /**
         * Headers list
         */
        header_bag m_headers;

        /**
         * Cookies list
         */
        cookie_bag m_cookies;

        /**
         * Http version
         */
        std::string m_version;

        /**
         * Http method
         */
        std::string m_method;

        /**
         * Url
         */
        reactive::uri::url m_url;

        /**
         * Request content type
         */
        std::string m_content_type;

        /**
         * Request content
         */
        std::string m_content;

        /**
         * User agent
         */
        std::string m_useragent;

        /**
         * Http parser settings
         */
        http_parser_settings m_settings;

        /**
         * Http parser
         */
        http_parser m_parser;

        /**
         * What is complete ?
         */
        //bool m_complete;
        //bool m_headers_complete;

        std::string m_current_field;
        std::string m_current_value;

        reactive::uri::query m_query;
        reactive::uri::query m_body;


        /**
         * Callback for the http_parser when on a http message
         *
         * TBD: what is a message
         */
        static int on_message_begin(http_parser* parser_);

        /**
         * Callback for the http_parser when on a http header field
         *
         * @param  parser_ [description]
         * @return         [description]
         */
        static int on_message_complete(http_parser* parser_);

        /**
         * Callback for the http_parser when on a http header field
         *
         * @param  parser_ [description]
         * @param  data_   [description]
         * @param  size_   [description]
         * @return         [description]
         */
        static int on_header_field(http_parser* parser_, const char* data_, std::size_t size_);

        /**
         * Callback for the http_parser when on a http header value
         *
         * @param  parser_ [description]
         * @param  data_   [description]
         * @param  size_   [description]
         * @return         [description]
         */
        static int on_header_value(http_parser* parser_, const char* data_, std::size_t size_);

        /**
         * Callback for the http_parser when on the http header is completed
         *
         * @param  parser_ [description]
         * @return         [description]
         */
        static int on_headers_complete(http_parser* parser_);

        /**
         * Callback for the http_parser when on a http header field
         *
         * @param  parser_ [description]
         * @param  data_   [description]
         * @param  size_   [description]
         * @return         [description]
         */
        static int on_url(http_parser* parser_, const char* data_, std::size_t size_);

        /**
         * Callback for the http_parser when on a http body
         *
         * @param  parser_ [description]
         * @param  data_   [description]
         * @param  size_   [description]
         * @return         [description]
         */
        static int on_body(http_parser* parser_, const char* data_, std::size_t size_);

        /**
         * Process called by http_parser callbacks when scan is on an http header
         *
         * @param request_  Resquest object to augment
         * @param field_    HTTP header field
         * @param value_    HTTP corresponding header value
         */
        static void process_header(request& request_, const std::string& field_, const std::string& value_);

        /**
         * Process called by http_parser callbacks when scan is on an http header for a proxied ip
         *
         * Basic information such as info.ip has already been set during the
         * connection establishment.
         *
         * @param request_  Resquest object to augment
         * @param ip_       IP string of the remote client to analyze for type and value
         */
        static void process_ip(request& request_, const std::string& ip_);

    public:
        /**
         * Construct default request
         *
         */
        request();

        /**
         * Create a request by parsing raw http request
         *
         * @param request_  The http request string
         */
        request(const std::string& request_) : request()
        {
            parse(request_);
        }

        /**
         * Default destructor
         */
        ~request();

        /**
         * Server Information structure
         *
         * IP and other info about the client and connection
         *
         * @see request_info
         */
        info_t info;

        /**
         * What is completed.
         * @param completed_ [description]
         */
        /*void setComplete(bool completed_)
        {
            m_complete = completed_;
        }*/

        /*void setHeadersComplete(bool completed_)
        {
            m_headers_complete = completed_;
        }*/

        inline void setCurrentValue(const std::string& value_)
        {
            m_current_value = value_;
        }

        inline std::string& getCurrentValue()
        {
            return m_current_value;
        }

        inline void setCurrentField(const std::string& field_)
        {
            m_current_field = field_;
        }

        inline std::string& getCurrentField()
        {
            return m_current_field;
        }

        /**
         * Reset request variables to be able to reuse this request object
         */
        void reset();

        /**
         * Initialize a request with plain text request data
         *
         * Only fields present in the input data will be overwritten.
         * If you want to first clean the request object then you have to call
         * the reset() function first.
         *
         * @param data_  Raw http request string received by the server
         */
        void parse(const std::string& data_)
        {
            // Data size limit is 80*1024 bytes
            parse(data_.c_str(), data_.length());
        }

        std::size_t parse(const void* data_, std::size_t size_)
        {
            return (parse(static_cast<const char*>(data_), size_));
        }

        /**
         * Main feed function that calls the http parser execution
         *
         * @param data_  Raw http request string received by the server
         * @param size_  Number of bytes in string data_
         */
        std::size_t parse(const char* data_, std::size_t size_);

        bool shouldKeepAlive() const;

        /**
         * Get a const reference to the header bag
         *
         * @return The header bag
         */
        const header_bag& getHeaders() const;

        /**
         * Get a reference to the header bag that can be modified
         *
         * @return The header bag
         */
        header_bag& getHeaders();

        /**
         * Get a const reference to the cookie bag
         *
         * @return The cookie bag
         */
        const cookie_bag& getCookies() const;

        /**
         * Get a reference to the cookie bag that can be modified
         *
         * @return The cookie bag
         */
        cookie_bag& getCookies();

        /**
         * Set request method
         *
         * @param method_ The string of request method
         */
        void setMethod(const std::string& method_);

        /**
         * Get request method
         *
         * @return The string of request method
         */
        const std::string& getMethod() const;

        /**
         * Set http version
         *
         * @param version_ The string of http version
         */
        void setVersion(const std::string& version_);

        /**
         * Get http version
         *
         * @return The string of http version
         */
        const std::string& getVersion() const;

        /**
         * Set authentication
         *
         * @param auth_ The authentication object
         */
        /*template<typename AuthType>
        inline void setAuth(AuthType& auth_)
        {
            auth_.apply((*this));
        }*/

        /**
         * Set user agent
         *
         * @param useragent_ The string of user agent
         */
        void setUserAgent(const std::string& useragent_);

        /**
         * Get user agent of request
         *
         * @return The string of user agent
         */
        const std::string& getUserAgent() const;

        /**
         * Set request url
         *
         * @param url_ The url object
         */
        void setUrl(reactive::uri::url& url_);

        /**
         * Set request url
         *
         * @param url The string of url
         */
        void setUrl(const std::string& url_);

        /**
         * Get url of request
         *
         * @return The url object of request
         */
        const reactive::uri::url& getUrl() const;

        /**
         * Get url of request
         *
         * @return The url object of request that can be modified
         */
        reactive::uri::url& getUrl();

        /**
         * Set content type
         *
         * @param type_ The string of content type
         */
        void setContentType(const std::string& type_);

        /**
         * Get content type
         *
         * @return The string of content type
         */
        const std::string& getContentType() const;

        /**
         * Set request content
         *
         * @param content_ The string of request content
         */
        void setContent(const std::string& content_);

        /**
         * Get request content
         *
         * @return The string of request content
         */
        const std::string& getContent() const;

        /**
         * Get request content
         *
         * @return The string of request content that can be modified
         */
        std::string& getContent();

        reactive::uri::query getData() const;

        reactive::json::value getJson() const;

        /**
         * Check if key_ exists in query arguments list
         *
         * @param  key_ [description]
         * @return      [description]
         */
        const bool hasQueryArgument(const std::string& key_) const;

        template<typename T = std::string>
        const T getQueryArgument(const std::string& key_, const T& default_) const;

        template<typename T = std::string>
        const T getQueryArgument(const std::string& key_) const;

        /**
         * Check if key_ exists in body arguments list
         *
         * @param  key_ [description]
         * @return      [description]
         */
        const bool hasBodyArgument(const std::string& key_) const;

        template<typename T = std::string>
        const T getBodyArgument(const std::string& key_, const T& default_) const;

        template<typename T = std::string>
        const T getBodyArgument(const std::string& key_) const;

        /**
         * Check if request is forwarded
         *
         * @return True if forwarded
         */
        inline bool isForwarded() const
        {
            return info.by_proxy;
        }

        /**
         * Check if use upgrade protocol
         *
         * @return True if upgrade protocol
         */
        inline bool isUpgrade() const
        {
            return (m_parser.upgrade != 0);
        }

        /**
         * Check if use connection keep alive
         *
         * @return True if connection keep alive
         */
        inline bool isConnectionKeepAlive() const
        {
            return (m_parser.flags & F_CONNECTION_KEEP_ALIVE);
        }

        /**
         * Check if use connection close
         *
         * @return True if connection close
         */
        inline bool isConnectionClose() const
        {
            return (m_parser.flags & F_CONNECTION_CLOSE);
        }
/*
        inline bool isTrailing() const
        {
            return (m_parser.flags & F_TRAILING);
        }

        inline bool isSkipBody() const
        {
            return (m_parser.flags & F_SKIPBODY);
        }
*/
        /**
         * Check if request is GET
         *
         * @return True if GET
         */
        inline bool isGet() const
        {
            return (m_method == protocol::METHOD_GET);
        }

        /**
         * Check if request is HEAD
         *
         * @return True if HEAD
         */
        inline bool isHead() const
        {
            return (m_method == protocol::METHOD_HEAD);
        }

        /**
         * Check if request is POST
         *
         * @return True if POST
         */
        inline bool isPost() const
        {
            return (m_method == protocol::METHOD_POST);
        }

        /**
         * Check if request is PUT
         *
         * @return True if PUT
         */
        inline bool isPut() const
        {
            return (m_method == protocol::METHOD_PUT);
        }

        /**
         * Check if request is DELETE
         *
         * @return True if DELETE
         */
        inline bool isDelete() const
        {
            return (m_method == protocol::METHOD_DELETE);
        }

        /**
         * Check if request is TRACE
         *
         * @return True if TRACE
         */
        inline bool isTrace() const
        {
            return (m_method == protocol::METHOD_TRACE);
        }

        /**
         * Check if request is OPTIONS
         *
         * @return True if OPTIONS
         */
        inline bool isOptions() const
        {
            return (m_method == protocol::METHOD_OPTIONS);
        }

        /**
         * Check if request is CONNECT
         *
         * @return True if CONNECT
         */
        inline bool isConnect() const
        {
            return (m_method == protocol::METHOD_CONNECT);
        }

        /**
         * Check if request is PATCH
         *
         * @return True if PATCH
         */
        inline bool isPatch() const
        {
            return (m_method == protocol::METHOD_PATCH);
        }

        /**
         * Check if request is Xml Http Request
         *
         * @return True if XmlHttpRequest
         */
        bool isXmlHttpRequest() const;

        /**
         * Return string of http request
         *
         * @return The string of http request
         */
        std::string toString() const;

        /** Formatting a one line string about the request for logs
         *
         * Examples:
         *
         * from: 10.0.0.2:36264, server: 127.0.0.1:8080, request: GET /demo?aa=123 HTTP/1.1
         *
         * Used format:
         * ipForwardedFor<>remoteEndpoint<>localEndpoint, GET /request HTTP/1.1, user-agent
         *
         */
        std::string toLog() const;
    };



    template<>
    inline const std::string request::getQueryArgument<std::string>(const std::string& key_, const std::string& default_) const
    {
        if (m_query.has(key_))
        {
            return m_query.at(key_);
        }

        return default_;
    }

    template<>
    inline const std::string request::getQueryArgument<std::string>(const std::string& key_) const
    {
        if (m_query.has(key_))
        {
            return m_query.at(key_);
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const int request::getQueryArgument<int>(const std::string& key_, const int& default_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<int>(m_query.at(key_));
        }

        return default_;
    }

    template<>
    inline const int request::getQueryArgument<int>(const std::string& key_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<int>(m_query.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const int64_t request::getQueryArgument<int64_t>(const std::string& key_, const int64_t& default_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<int64_t>(m_query.at(key_));
        }

        return default_;
    }

    template<>
    inline const int64_t request::getQueryArgument<int64_t>(const std::string& key_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<int64_t>(m_query.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const double request::getQueryArgument<double>(const std::string& key_, const double& default_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<double>(m_query.at(key_));
        }

        return default_;
    }

    template<>
    inline const double request::getQueryArgument<double>(const std::string& key_) const
    {
        if (m_query.has(key_))
        {
            return reactive::string::to_number<double>(m_query.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const bool request::getQueryArgument<bool>(const std::string& key_, const bool& default_) const
    {
        if (m_query.has(key_))
        {
            std::string value = m_query.at(key_);

            if (value == "1" || value == "true" || value == "TRUE" || value == "True")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return default_;
    }

    template<>
    inline const bool request::getQueryArgument<bool>(const std::string& key_) const
    {
        if (m_query.has(key_))
        {
            std::string value = m_query.at(key_);

            if (value == "1" || value == "true" || value == "TRUE" || value == "True")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const std::string request::getBodyArgument<std::string>(const std::string& key_, const std::string& default_) const
    {
        if (m_body.has(key_))
        {
            return m_body.at(key_);
        }

        return default_;
    }

    template<>
    inline const std::string request::getBodyArgument<std::string>(const std::string& key_) const
    {
        if (m_body.has(key_))
        {
            return m_body.at(key_);
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const int request::getBodyArgument<int>(const std::string& key_, const int& default_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<int>(m_body.at(key_));
        }

        return default_;
    }

    template<>
    inline const int request::getBodyArgument<int>(const std::string& key_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<int>(m_body.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const int64_t request::getBodyArgument<int64_t>(const std::string& key_, const int64_t& default_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<int64_t>(m_body.at(key_));
        }

        return default_;
    }

    template<>
    inline const int64_t request::getBodyArgument<int64_t>(const std::string& key_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<int64_t>(m_body.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const double request::getBodyArgument<double>(const std::string& key_, const double& default_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<double>(m_body.at(key_));
        }

        return default_;
    }

    template<>
    inline const double request::getBodyArgument<double>(const std::string& key_) const
    {
        if (m_body.has(key_))
        {
            return reactive::string::to_number<double>(m_body.at(key_));
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }

    template<>
    inline const bool request::getBodyArgument<bool>(const std::string& key_, const bool& default_) const
    {
        if (m_body.has(key_))
        {
            std::string value = m_body.at(key_);

            if (value == "1" || value == "true" || value == "TRUE" || value == "True")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return default_;
    }

    template<>
    inline const bool request::getBodyArgument<bool>(const std::string& key_) const
    {
        if (m_body.has(key_))
        {
            std::string value = m_body.at(key_);

            if (value == "1" || value == "true" || value == "TRUE" || value == "True")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        throw exception("Missing \"" + key_ + "\" argument.", reactive::http::status::BAD_REQUEST);
    }


} // end of http namespace
} // end of reactive namespace
