/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <reactive/uri/url.hpp>
#include <sstream>
#include <string>
#include <reactive/net/port.hpp>
#include <http_parser.h>
#include <stdexcept>

namespace reactive {
namespace uri {

    void url::parse(const std::string& url_)
    {
        http_parser_url u;

        int result = http_parser_parse_url(
            url_.c_str(),
            url_.length(),
            0,
            &u
        );

        if (result != 0)
        {
            throw std::invalid_argument("Invalid url");
        }

        std::string userinfo;
        std::size_t pos;

        for (uint16_t i = 0; i < UF_MAX; ++i)
        {
            if ((u.field_set & (1 << i)) == 0)
            {
                continue;
            }

            switch (i)
            {
                case UF_SCHEMA:
                    setScheme(url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    ));
                    break;
                case UF_HOST:
                    setHost(url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    ));
                    break;
                case UF_PORT:
                    setPort(u.port);
                    break;
                case UF_PATH:
                    setPath(url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    ));
                    break;
                case UF_QUERY:
                    setQuery(url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    ));
                    break;
                case UF_FRAGMENT:
                    setFragment(url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    ));
                    break;
                case UF_USERINFO:
                    userinfo = url_.substr(
                        u.field_data[i].off,
                        u.field_data[i].len
                    );

                    pos = userinfo.find(':');

                    if (pos == std::string::npos)
                    {
                        setUser(userinfo);
                    }
                    else
                    {
                        setUser(userinfo.substr(0, pos));
                        setPassword(userinfo.substr(pos + 1));
                    }

                    break;
                case UF_MAX:
                    break;
            }
        }
    }

    void url::setUrl(const std::string& url_)
    {
        if (!url_.empty())
        {
            parse(url_);
        }
    }

    void url::setScheme(const std::string& scheme_)
    {
        m_scheme = scheme_;

        if (scheme_ == "https")
        {
            m_port = HTTPS_PORT;
        }
        else
        {
            m_port = HTTP_PORT;
        }
    }

    const std::string& url::getScheme() const
    {
        return m_scheme;
    }

    void url::setHost(const std::string& host_)
    {
        m_host = host_;

        if (m_host.empty())
        {
            m_is_empty = true;
        }
        else
        {
            m_is_empty = false;
        }
    }

    const std::string& url::getHost() const
    {
        return m_host;
    }

    void url::setPort(unsigned short port_)
    {
        if (!reactive::net::port::is_valid(port_))
        {
            throw std::invalid_argument("Port is not valid.");
        }

        m_port = port_;
    }

    const unsigned short& url::getPort() const
    {
        return m_port;
    }

    void url::setUser(const std::string& user_)
    {
        m_user = user_;
    }

    const std::string& url::getUser() const
    {
        return m_user;
    }

    void url::setPassword(const std::string& password_)
    {
        m_password = password_;
    }

    const std::string& url::getPassword() const
    {
        return m_password;
    }

    void url::setPath(const std::string& path_)
    {
        if (path_.compare(0, 1, "/") == 0)
        {
            m_path = path_;
        }
        else
        {
            m_path = "/" + path_;
        }
    }

    const std::string& url::getPath() const
    {
        return m_path;
    }

    void url::setQuery(const std::string& query_)
    {
        m_query = query_;
    }

    const std::string& url::getQuery() const
    {
        return m_query;
    }

    void url::setFragment(const std::string& fragment_)
    {
        m_fragment = fragment_;
    }

    const std::string& url::getFragment() const
    {
        return m_fragment;
    }

    bool url::isEmpty() const
    {
        return m_is_empty;
    }

    std::string url::toString(int component_) const
    {
        std::string url = "";

        if ((component_ < 0) || (component_ > 0 && component_ & SCHEME))
        {
            url.append(m_scheme);

            if (component_ & HOST)
            {
                url.append("://");
            }
        }

        if ((component_ < 0) || (component_ > 0 && component_ & USER && component_ & PASSWORD))
        {
            if (m_user.size() > 0)
            {
                url.append(m_user);
                if (m_password.size() > 0)
                {
                    url.append(":" + m_password);
                }
                url.append("@");
            }
        }

        if ((component_ < 0) || (component_ > 0 && component_ & HOST))
        {
            url.append(m_host);
        }

        if ((component_ < 0) || (component_ > 0 && component_ & PORT))
        {
            if (m_port != HTTP_PORT && m_port != HTTPS_PORT)
            {
                std::stringstream port_ss;
                port_ss << m_port;
                url.append(":" + port_ss.str());
            }
        }

        if ((component_ < 0) || (component_ > 0 && component_ & PATH))
        {
            if (m_path.size() > 0)
            {
                url.append(m_path);
            }
        }

        if ((component_ < 0) || (component_ > 0 && component_ & QUERY))
        {
            if (m_query.size() > 0)
            {
                url.append("?" + m_query);
            }
        }

        if ((component_ < 0) || (component_ > 0 && component_ & FRAGMENT))
        {
            if (m_fragment.size() > 0)
            {
                url.append("#" + m_fragment);
            }
        }

        return url;
    }

} // end of uri namespace
} // end of reactive namespace
