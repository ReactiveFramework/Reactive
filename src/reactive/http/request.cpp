/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/request.hpp>

namespace reactive {
namespace http {

    int request::on_message_begin(http_parser* parser_)
    {
        //request& r = *(static_cast<request*>(parser_->data));
        //r.setComplete(false);
        //r.setHeadersComplete(false);
        return (0);
    }

    int request::on_message_complete(http_parser* parser_)
    {
        //request& r = *static_cast<request*>(parser_->data);
        //r.setComplete(true);

        // Force the parser to stop after the request is parsed so clients
        // can process the request (or response).  This is to properly
        // handle HTTP/1.1 pipelined requests.
        //http_parser_pause(parser_, 1);

        return (0);
    }

    int request::on_header_field(http_parser* parser_, const char* data_, std::size_t size_)
    {
        request& r = *static_cast<request*>(parser_->data);

        if (!r.getCurrentValue().empty())
        {
            process_header(r, r.getCurrentField(), r.getCurrentValue());

            r.getCurrentField().clear();
            r.getCurrentValue().clear();
        }

        r.getCurrentField().append(data_, size_);

        return (0);
    }

    int request::on_header_value(http_parser* parser_, const char* data_, std::size_t size_)
    {
        request& r = *static_cast<request*>(parser_->data);
        r.getCurrentValue().append(data_, size_);
        return (0);
    }

    int request::on_headers_complete(http_parser* parser_)
    {
        request& r = *(static_cast<request*>(parser_->data));

        if (!r.getCurrentValue().empty())
        {
            process_header(r, r.getCurrentField(), r.getCurrentValue());
            r.getCurrentField().clear();
            r.getCurrentValue().clear();
        }

        //r.setHeadersComplete(true);

        // Force the parser to stop after the headers are parsed so clients
        // can process the request (or response).  This is to properly
        // handle HTTP/1.1 pipelined requests.
        //http_parser_pause(parser_, 1);

        return (0);
    }

    int request::on_url(http_parser* parser_, const char* data_, std::size_t size_)
    {
        std::string path;
        std::string query;
        bool is_path = true;

        for (std::size_t i = 0; i < size_; ++i)
        {
            char chr = data_[i];

            if (is_path)
            {
                if (chr == '?')
                {
                    is_path = false;
                    continue;
                }

                path += chr;
            }
            else
            {
                query += chr;
            }
        }

        request& r = *static_cast<request*>(parser_->data);

        r.getUrl().setPath(path);

        if (!query.empty())
        {
            r.getUrl().setQuery(query);
        }

        return (0);
    }

    int request::on_body(http_parser* parser_, const char* data_, std::size_t size_)
    {
        static_cast<request*>(parser_->data)->getContent().append(data_, size_);
        return (0);
    }

    void request::process_header(request& request_, const std::string& field_, const std::string& value_)
    {
        if (field_ == "Host")
        {
            request_.getUrl().setHost(value_);
        }
        else if (field_ == "User-Agent")
        {
            request_.setUserAgent(value_);
        }
        else if (field_ == "Cookie")
        {
            //@TODO convert this code in state machine parser
            std::vector<std::string> cookies;
            reactive::string::split(REACTIVE_HTTP_COOKIE_SEPARATOR, value_, cookies);

            for (std::size_t i = 0; i < cookies.size(); ++i)
            {
                std::vector<std::string> cookie_string;
                reactive::string::split("=", cookies.at(i), cookie_string);

                cookie_t cookie;
                cookie.name = reactive::uri::decode(cookie_string[0]);
                cookie.value = reactive::uri::decode(cookie_string[1]);
                cookie_string.clear();

                request_.getCookies().add(cookie);
            }
            cookies.clear();
        }
        else if (field_ == "X-Forwarded-For")
        {
            process_ip(request_, value_);
        }
        else if (field_ == "X-Client")
        {
            process_ip(request_, value_);
        }
        else if (field_ == "Content-Type")
        {
            request_.setContentType(value_);
        }

        request_.getHeaders().add(field_, value_);
    }

    void request::process_ip(request& request_, const std::string& ip_)
    {
        request_.info.by_proxy          = true;
        request_.info.proxy_ip_version  = request_.info.ip_version;
        request_.info.proxy_ip          = request_.info.ip;
        request_.info.proxy_port        = request_.info.port;

        std::size_t pos = ip_.find(",");

        request_.info.ip = ip_;

        if (pos != std::string::npos)
        {
            request_.info.ip = ip_.substr(0, pos);
        }

        try
        {
            boost::asio::ip::address ip_version = boost::asio::ip::address::from_string(request_.info.ip);

            if (ip_version.is_v4())
            {
                request_.info.ip_version = reactive::net::ip::IPV4;
            }
            else if (ip_version.is_v6())
            {
                request_.info.ip_version = reactive::net::ip::IPV6;
            }

        }
        catch (std::exception& e)
        {
            request_.info.ip_version = reactive::net::ip::UNDEFINED;
            //request_.info.by_proxy   = false;
        }

        // In this process we have no port information
        request_.info.port.clear();
    }

    request::request()
    {
        reset();

        memset(&m_settings, 0, sizeof(m_settings));

        // Setting state machine callbacks
        m_settings.on_message_begin    = &request::on_message_begin;
        m_settings.on_message_complete = &request::on_message_complete;
        m_settings.on_header_field     = &request::on_header_field;
        m_settings.on_header_value     = &request::on_header_value;
        m_settings.on_headers_complete = &request::on_headers_complete;
        m_settings.on_url              = &request::on_url;
        m_settings.on_body             = &request::on_body;

        memset(&m_parser, 0, sizeof(m_parser));
        http_parser_init(&m_parser, HTTP_REQUEST);
        m_parser.data = this;
    }

    request::~request()
    {
        // clear headers list
        m_headers.clear();

        // clear cookies list
        m_cookies.clear();
    }

    void request::reset()
    {
        info.reset();

        m_useragent = REACTIVE_HTTP_REQUEST_USER_AGENT;
        m_version   = protocol::VERSION_11;
        m_method    = protocol::METHOD_GET;

        // Resetting content and its type
        m_content       = "";
        m_content_type  = "";

        // unused undocumented variables
        //m_complete = false;
        //m_headers_complete = false;

        m_query.clear();
        m_body.clear();
    }


    std::size_t request::parse(const char* data_, std::size_t size_)
    {
        std::size_t parsed;

        if (size_ >= 0)
        {
            parsed = http_parser_execute(&m_parser, &m_settings, data_, size_);

            //if (parsed < size_)
            // LIMIT in reading is 80x1024
        }


        //const http_errno errno = static_cast<http_errno>(m_parser.http_errno);

        // The 'on_message_complete' and 'on_headers_complete' callbacks fail
        // on purpose to force the parser to stop between pipelined requests.
        // This allows the clients to reliably detect the end of headers and
        // the end of the message.  Make sure the parser is always unpaused
        // for the next call to 'feed'.
        /*if (herrno == HPE_PAUSED)
        {
            http_parser_pause(&m_parser, 0);
        }*/

        /*if (used < size_)
        {
            if (herrno == HPE_PAUSED)
            {
                // Make sure the byte that triggered the pause
                // after the headers is properly processed.
                if (!m_complete)
                {
                    used += http_parser_execute(&m_parser, &m_settings, data_+used, 1);
                }
            }
            else
            {
                throw (error(herrno));
            }
        }*/

        m_method  = std::string(http_method_str((http_method)m_parser.method));
        m_version = std::to_string(m_parser.http_major) + "." + std::to_string(m_parser.http_minor);

        if (!m_url.getQuery().empty())
        {
            m_query.parse(m_url.getQuery());
        }

        if (m_content_type == "application/x-www-form-urlencoded")
        {
            m_body.parse(m_content);
        }

        return (parsed);
    }

    bool request::shouldKeepAlive() const
    {
        return (http_should_keep_alive(const_cast<http_parser*>(&m_parser)) != 0);
    }

    const header_bag& request::getHeaders() const
    {
        return m_headers;
    }

    header_bag& request::getHeaders()
    {
        return m_headers;
    }

    const cookie_bag& request::getCookies() const
    {
        return m_cookies;
    }

    cookie_bag& request::getCookies()
    {
        return m_cookies;
    }

    void request::setMethod(const std::string& method_)
    {
        m_method = method_;
    }

    const std::string& request::getMethod() const
    {
        return m_method;
    }

    void request::setVersion(const std::string& version_)
    {
        m_version = version_;
    }

    const std::string& request::getVersion() const
    {
        return m_version;
    }

    void request::setUserAgent(const std::string& useragent_)
    {
        m_useragent = useragent_;
    }

    const std::string& request::getUserAgent() const
    {
        return m_useragent;
    }

    void request::setUrl(reactive::uri::url& url_)
    {
        m_url = url_;
    }

    void request::setUrl(const std::string& url_)
    {
        m_url = reactive::uri::url(url_);
    }

    const reactive::uri::url& request::getUrl() const
    {
        return m_url;
    }

    reactive::uri::url& request::getUrl()
    {
        return m_url;
    }

    void request::setContentType(const std::string& type_)
    {
        std::size_t pos = type_.find(";");

        if (pos != std::string::npos)
        {
            m_content_type = type_.substr(0, pos);
        }
        else
        {
            m_content_type = type_;
        }

        m_headers.add("Content-Type", m_content_type);
    }

    /**
     * Get content type
     *
     * @return The string of content type
     */
    const std::string& request::getContentType() const
    {
        return m_content_type;
    }

    void request::setContent(const std::string& content_)
    {
        m_content = content_;
    }

    /**
     * Get request content
     *
     * @return The string of request content
     */
    const std::string& request::getContent() const
    {
        return m_content;
    }

    /**
     * Get request content
     *
     * @return The string of request content that can be modified
     */
    std::string& request::getContent()
    {
        return m_content;
    }

    reactive::uri::query request::getData() const
    {
        return m_body;
    }

    reactive::json::value request::getJson() const
    {
        reactive::json::value retval;

        if (m_content_type == "application/json")
        {
            retval = reactive::json::decode(m_content);
        }

        return std::move(retval);
    }

    const bool request::hasQueryArgument(const std::string& key_) const
    {
        return m_query.has(key_);
    }

    const bool request::hasBodyArgument(const std::string& key_) const
    {
        return m_body.has(key_);
    }

    bool request::isXmlHttpRequest() const
    {
        if (m_headers.has("X-Requested-With") && m_headers.get("X-Requested-With").value == "XMLHttpRequest")
        {
            return true;
        }

        return false;
    }

    std::string request::toString() const
    {
        header_bag headers = m_headers;
        headers.add("Host", m_url.toString(reactive::uri::url::HOST | reactive::uri::url::PORT));

        if (!headers.has("Cache-Control"))
        {
            headers.add("Cache-Control", "max-age=0");
        }

        if (!headers.has("Accept"))
        {
            headers.add("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        }

        if (!headers.has("User-Agent"))
        {
            headers.add("User-Agent", m_useragent);
        }

        if (!headers.has("Accept-Charset"))
        {
            headers.add("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.3");
        }

        std::string reqstr = m_method
            + " "
            + m_url.toString(reactive::uri::url::PATH | reactive::uri::url::QUERY)
            + " HTTP/"
            + m_version
            + protocol::CRLF;

        // --- Constructing the cookie string
        std::string full_cookies;
        std::size_t count_cookie = m_cookies.size();
        for (std::size_t i = 0; i < count_cookie; ++i)
        {
            std::string cookie;

            cookie.append(reactive::uri::encode(m_cookies.at(i).name));
            cookie.append("=");
            cookie.append(reactive::uri::encode(m_cookies.at(i).value));

            if (i != (count_cookie - 1))
            {
                cookie.append(REACTIVE_HTTP_COOKIE_SEPARATOR);
            }

            full_cookies.append(cookie);
        }

        // --- Using the cookie string to build the header
        if (!full_cookies.empty())
        {
            headers.add("Cookie", full_cookies);
        }

        // In HTTP/1.1 the default connection type is Keep-Alive
        // while it is not fully supported in HTTP/1.0 it does not matter.
        //
        // Anyway this default header is set during connection and not here
        //headers.add("Connection", "Keep-Alive");

        reqstr.append(headers.toString() + protocol::CRLF);

        if (!m_content.empty())
        {
            reqstr.append(m_content);
        }

        return reqstr;
    }

    std::string request::toLog() const
    {
        std::string output;

        if (info.port.empty())
        {
            output += info.ip;
        }
        else
        {
            output += info.ip + ":" + info.port;
        }

        output += "<>";

        if (info.by_proxy)
        {
            output += info.proxy_ip + ":" + info.proxy_port;
            output += "<>";
        }

        output += info.local_ip + ":" + info.local_port;
        output += ", ";
        output += m_method + " ";
        output += reactive::string::substitute("%", "%%", m_url.getPath());

        if (!m_url.getQuery().empty())
        {
            output += "?" + reactive::string::substitute("%", "%%", m_url.getQuery());
        }

        output += " HTTP/" + m_version;
        output += ", " + (m_headers.has("User-Agent") ? m_headers.get("User-Agent").value : m_useragent);

        return output;
    }

} // end of http namespace
} // end of reactive namespace
