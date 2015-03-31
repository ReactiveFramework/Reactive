/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <vector>
#include <string>
#include <http_parser.h>
#include <reactive/http/request.hpp>
#include <reactive/http/header_bag.hpp>
#include <reactive/http/cookie_bag.hpp>
#include <reactive/http/protocol.hpp>
#include <reactive/http/status.hpp>

namespace reactive {
namespace http {

    class response
    {
    private:
        status_t m_code;

        /**
         * HTTP Version of the response, by default this is version 1.1
         */
        std::string m_version;

        /**
         *
         */
        std::string m_charset;

        /**
         *
         */
        header_bag m_headers;

        /**
         *
         */
        cookie_bag m_cookies;

        /**
         *
         */
        std::string m_content_type;

        /**
         *
         */
        std::string m_content;

        /**
         * Is the request a HEAD request
         */
        bool m_request_is_head;

        /**
         * Does the request header contains an Accept-Encoding
         */
        bool m_request_has_accept_encoding;

        /**
         * Accept encoding part of the request header if necessary
         */
        header_t m_request_accept_encoding_header;


        /**
         * Http parser settings
         */
        http_parser_settings m_settings;

        /**
         * Http parser
         */
        http_parser m_parser;

        /**
         *
         */
        std::string m_current_field;

        /**
         *
         */
        std::string m_current_value;

        /**
         *
         */
        bool m_is_complete;

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
         * @param response_ Response object to augment
         * @param field_    HTTP header field
         * @param value_    HTTP corresponding header value
         */
        static void process_header(response& response_, const std::string& field_, const std::string& value_);


    public:
        response(const status_t& http_status_code_) :
            m_code(http_status_code_),
            m_version(protocol::VERSION_11),
            m_charset("UTF-8"),
            m_content_type("text/html"),
            m_request_is_head(false),
            m_request_has_accept_encoding(false),
            m_is_complete(true)
        {
            memset(&m_settings, 0, sizeof(m_settings));

            // Setting state machine callbacks
            m_settings.on_message_begin    = &on_message_begin;
            m_settings.on_message_complete = &on_message_complete;
            m_settings.on_header_field     = &on_header_field;
            m_settings.on_header_value     = &on_header_value;
            m_settings.on_headers_complete = &on_headers_complete;
            //m_settings.on_url              = &on_url;
            m_settings.on_body             = &on_body;

            memset(&m_parser, 0, sizeof(m_parser));
            http_parser_init(&m_parser, HTTP_RESPONSE);
            m_parser.data = this;
        }

        response(): response(status::OK)
        {
        }

        /**
         * Create a response object from a prepared text string
         */
        response(const std::string& response_) : response()
        {
            parse(response_);
        }

        ~response()
        {
            reset();
        }

        bool isComplete();

        void setComplete(bool ready_);

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
         * All parameters are reset here. This is useful in a reuse context.
         */
        void reset();

        /**
         * Reset the response object and directly set a new request
         *
         * @param request_  New request to consider
         */
        void reset(const request& request_)
        {
            reset();
            setRequest(request_);
        }

        /**
         * All parameters are reset here. This is useful in a reuse context.
         * @param response_ [description]
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

        std::size_t parse(const char* data_, std::size_t size_);

        /**
         * [setVersion description]
         * @param version_ [description]
         */
        void setVersion(const std::string& version_);

        /**
         * [getVersion description]
         * @return [description]
         */
        const std::string& getVersion() const;

        /**
         * Set content type
         *
         * @param content_type_ The string of content type
         */
        void setContentType(const std::string& content_type_);

        /**
         * Get content type
         *
         * @return The string of content type
         */
        const std::string& getContentType() const;
        /**
         * Set response content
         *
         * @param content_ The string of response content
         */
        void setContent(const std::string& content_);

        /**
         * Get response content
         *
         * @return The string of response content
         */
        const std::string& getContent() const;

        std::string& getContent();

        /**
         * Set charset
         *
         * @param charset_ The string of charset
         */
        void setCharset(const std::string& charset_);

        /**
         * Get charset
         *
         * @return The string of response charset
         */
        const std::string& getCharset() const;

        /**
         * Set the http status code
         *
         * @param code_ The http status code
         */
        void setStatusCode(const status_t& code_);

        /**
         * Get http status code
         *
         * @return The http status code
         */
        const status_t& getStatusCode() const;

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
         * Add header in header_bag
         *
         * @param name_   The string of header name
         * @param value_  The string of header value
         * @param unique_ Determin if header is unique
         */
        void addHeader(const std::string& name_, const std::string& value_, bool unique_ = true);

        /**
         * Get a const reference to the headers
         *
         * @return The header_bag object
         */
        const header_bag& getHeaders() const;

        /**
         * Get a mutable reference to the headers
         *
         * @return The header_bag object
         */
        header_bag& getHeaders();

        /**
         * Add cookie in cookie_bag
         *
         * @param name_  The string of cookie name
         * @param value_ The string of cookie value
         */
        void addCookie(const std::string& name_, const std::string& value_);

        /**
         * Add cookie in cookie_bag
         *
         * @param cookie_ The struct of cookie
         */
        void addCookie(const cookie_t& cookie_);

        /**
         * Get a reference to the cookies, they are mutable
         *
         * @return The cookie_bag object
         */
        cookie_bag& getCookies();

        /**
         * Get a const reference to the cookies
         *
         * @return The cookie_bag object
         */
        const cookie_bag& getCookies() const;

        /**
         * Set request
         *
         * @param request_ The object of request
         */
        void setRequest(const request& request_);

        /**
         * Chekc if response is ok
         *
         * @return True if response is ok
         */
        inline bool isOk() const
        {
            return (m_code == 200);
        }

        inline bool isServerError() const
        {
            return (m_code >= 500 && m_code < 600);
        }

        inline bool isForbidden() const
        {
            return (m_code == 403);
        }

        inline bool isClientError() const
        {
            return (m_code >= 400 && m_code < 500);
        }

        inline bool isRedirection() const
        {
            return (m_code >= 300 && m_code < 400);
        }

        inline bool isInvalid() const
        {
            return (m_code < 100 || m_code >= 600);
        }

        inline bool isInformational() const
        {
            return (m_code >= 100 && m_code < 200);
        }

        inline bool isSuccessful() const
        {
            return (m_code >= 200 && m_code < 300);
        }

        inline bool isNotFound() const
        {
            return (m_code == 404);
        }

        inline bool isBadRequest() const
        {
            return (m_code == 400);
        }

        /**
         * Returns true if response is a type with no content
         * @return [description]
         */
        inline bool isEmpty() const
        {
            return (m_code == 201 || m_code == 204 || m_code == 304);
        }

        /**
         * Format the response content into a string
         * @return [description]
         */
        std::string toString() const;
    };

} // end of http namespace
} // end of reactive namespace
