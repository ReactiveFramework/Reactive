/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/http/response.hpp>

#include <reactive/string.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <reactive/version.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

namespace reactive {
namespace http {

    int response::on_message_begin(http_parser* parser_)
    {
        response& r = *(static_cast<response*>(parser_->data));
        r.setComplete(false);
        //r.setComplete(false);
        //r.setHeadersComplete(false);
        return (0);
    }

    int response::on_message_complete(http_parser* parser_)
    {
        response& r = *static_cast<response*>(parser_->data);
        //r.setComplete(true);

        r.setComplete(true);

        r.setVersion(std::to_string(parser_->http_major) + "." + std::to_string(parser_->http_minor));

        // Force the parser to stop after the request is parsed so clients
        // can process the request (or response).  This is to properly
        // handle HTTP/1.1 pipelined requests.
        //http_parser_pause(parser_, 1);



        return (0);
    }

    int response::on_header_field(http_parser* parser_, const char* data_, std::size_t size_)
    {
        response& r = *static_cast<response*>(parser_->data);

        if (!r.getCurrentValue().empty())
        {
            process_header(r, r.getCurrentField(), r.getCurrentValue());

            r.getCurrentField().clear();
            r.getCurrentValue().clear();
        }

        r.getCurrentField().append(data_, size_);

        return (0);
    }

    int response::on_header_value(http_parser* parser_, const char* data_, std::size_t size_)
    {
        response& r = *static_cast<response*>(parser_->data);
        r.getCurrentValue().append(data_, size_);
        return (0);
    }

    int response::on_headers_complete(http_parser* parser_)
    {
        response& r = *(static_cast<response*>(parser_->data));

        if (!r.getCurrentValue().empty())
        {
            process_header(r, r.getCurrentField(), r.getCurrentValue());
            r.getCurrentField().clear();
            r.getCurrentValue().clear();
        }

        r.setStatusCode(static_cast<status_t>(parser_->status_code));

        return (0);
    }

    int response::on_body(http_parser* parser_, const char* data_, std::size_t size_)
    {
        static_cast<response*>(parser_->data)->getContent().append(data_, size_);
        return (0);
    }

    void response::process_header(response& response_, const std::string& field_, const std::string& value_)
    {
        if (field_ == "Content-Type")
        {
            response_.setContentType(value_);
        }

        response_.getHeaders().add(field_, value_);
    }

    void response::setComplete(bool completed_)
    {
        m_is_complete = completed_;
    }

    bool response::isComplete()
    {
        return m_is_complete;
    }

    void response::reset()
    {
        // Resetting headers and cookies
        m_headers.clear();
        m_cookies.clear();

        // Basics
        setStatusCode(reactive::http::status::OK);
        setCharset("UTF-8");
        setContentType("text/html");

        m_request_is_head             = false;
        m_request_has_accept_encoding = false;
        m_is_complete                 = true;
        m_request_accept_encoding_header.value = "";

        m_content.clear();
    }


    std::size_t response::parse(const char* data_, std::size_t size_)
    {
        std::size_t parsed = 0;

        if (size_ > 0)
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

        //m_version = std::to_string(m_parser.http_major) + "." + std::to_string(m_parser.http_minor);

        /*if (m_content_type == "application/x-www-form-urlencoded")
        {
            m_body.parse(m_content);
        }*/

        return (parsed);
    }

    void response::setVersion(const std::string& version_)
    {
        m_version = version_;
    }

    const std::string& response::getVersion() const
    {
        return m_version;
    }

    void response::setContentType(const std::string& content_type_)
    {
        std::size_t pos = content_type_.find(";");

        if (pos != std::string::npos)
        {
            m_content_type = content_type_.substr(0, pos);
        }
        else
        {
            m_content_type = content_type_;
        }

        m_headers.add("Content-Type", m_content_type);
    }

    const std::string& response::getContentType() const
    {
        return m_content_type;
    }

    void response::setContent(const std::string& content_)
    {
        m_content = content_;
    }

    const std::string& response::getContent() const
    {
        return m_content;
    }

    std::string& response::getContent()
    {
        return m_content;
    }

    void response::setCharset(const std::string& charset_)
    {
        m_charset = charset_;
    }

    const std::string& response::getCharset() const
    {
        return m_charset;
    }

    void response::setStatusCode(const status_t& code_)
    {
        m_code = code_;
    }

    const status_t& response::getStatusCode() const
    {
        return m_code;
    }

    void response::addHeader(const std::string& name_, const std::string& value_, bool unique_)
    {
        m_headers.add(name_, value_, unique_);
    }

    const header_bag& response::getHeaders() const
    {
        return m_headers;
    }

    header_bag& response::getHeaders()
    {
        return m_headers;
    }

    void response::addCookie(const std::string& name_, const std::string& value_)
    {
        m_cookies.add(name_, value_);
    }

    void response::addCookie(const cookie_t& cookie_)
    {
        m_cookies.add(cookie_);
    }

    cookie_bag& response::getCookies()
    {
        return m_cookies;
    }

    const cookie_bag& response::getCookies() const
    {
        return m_cookies;
    }

    void response::setRequest(const request& request_)
    {
        m_request_is_head = request_.isHead();
        m_request_has_accept_encoding = request_.getHeaders().has("Accept-Encoding");

        if (m_request_has_accept_encoding)
        {
            m_request_accept_encoding_header = request_.getHeaders().get("Accept-Encoding");
        }
        // Only set the minimum needed information to avoid the full copy:
        // m_request = request_;
    }

    std::string response::toString() const
    {
        time_t rawtime;
        struct tm * timeinfo;
        char date[35];

        time(&rawtime);
        timeinfo = gmtime(&rawtime);

        strftime(date, 35, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

        header_bag headers_end = m_headers;
        header_bag headers_begin;

        std::string content = m_content;
        std::string output_str;

        output_str.append("HTTP/" +
            m_version + " " +
            std::to_string(m_code) + " " +
            status::to_string(m_code) +
            protocol::CRLF
        );

        if (!headers_end.has("Server"))
        {
            std::string server(REACTIVE_NAME);
            server.append("/");
            server.append(REACTIVE_VERSION);
            headers_begin.add("Server", server);
        }

        //boost::posix_time::ptime date(boost::posix_time::second_clock::universal_time());

        headers_begin.add("Date", date);

        // remove Content-Type if exists
        if (headers_end.has("Content-Type"))
        {
            headers_end.del("Content-Type");
        }

        headers_begin.add("Content-Type", m_content_type + "; charset=" + m_charset);

        // Select the content compression type when possible and needed
        if (
            (!isEmpty() || !content.empty() || !m_request_is_head)
            && m_request_has_accept_encoding
        )
        {
            if (m_request_accept_encoding_header.value.find("deflate") != std::string::npos)
            {
                std::istringstream uncompressed(m_content);
                std::stringstream compressed;

                boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
                out.push(boost::iostreams::zlib_compressor());
                out.push(uncompressed);

                boost::iostreams::copy(out, compressed);
                content = compressed.str();

                headers_begin.add("Content-Encoding", "deflate");
            }
            else if (m_request_accept_encoding_header.value.find("gzip") != std::string::npos)
            {
                std::istringstream uncompressed(m_content);
                std::stringstream compressed;

                boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
                out.push(boost::iostreams::gzip_compressor());
                out.push(uncompressed);

                boost::iostreams::copy(out, compressed);
                content = compressed.str();

                headers_begin.add("Content-Encoding", "gzip");
            }
            else if (m_request_accept_encoding_header.value.find("bzip2") != std::string::npos)
            {
                std::istringstream uncompressed(m_content);
                std::stringstream compressed;

                boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
                out.push(boost::iostreams::bzip2_compressor());
                out.push(uncompressed);

                boost::iostreams::copy(out, compressed);
                content = compressed.str();

                headers_begin.add("Content-Encoding", "bzip2");
            }

            if (headers_begin.has("Content-Encoding"))
            {
                headers_end.add("Vary", "Accept-Encoding");
            }
        }

        if (m_request_is_head)
        {
            content = "";
        }
        else
        {
            headers_begin.add("Content-Length", std::to_string(content.length()));
        }

        // check if cookies
        if (m_cookies.size() > 0)
        {
            for (std::size_t i = 0; i < m_cookies.size(); ++i)
            {
                headers_begin.add("Set-Cookie", cookie::to_string(m_cookies[i]), false);
            }
        }

        headers_begin.insert(
            headers_begin.end(),
            headers_end.begin(),
            headers_end.end()
        );

        /*headers.insert(
            headers.end(),
            m_headers.begin(),
            m_headers.end()
        );*/

        output_str.append(headers_begin.toString());

        headers_begin.clear();
        headers_end.clear();

        output_str.append(protocol::CRLF);

        if (!isEmpty() || !content.empty())
        {
            output_str.append(content);
        }

        return output_str;
    }

} // end of http namespace
} // end of reactive namespace
