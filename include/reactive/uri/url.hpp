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

namespace reactive {
namespace uri {

    class url {
    private:
        /**
         * Scheme of url
         */
        std::string m_scheme;

        /**
         * Host of url
         */
        std::string m_host;

        /**
         * Port of url
         */
        unsigned short m_port;

        /**
         * User of url
         */
        std::string m_user;

        /**
         * Password of url
         */
        std::string m_password;

        /**
         * Path of url
         */
        std::string m_path;

        /**
         * Query of url
         */
        std::string m_query;

        /**
         * Fragment of url
         */
        std::string m_fragment;

        /**
         * An url is considered empty if no host is set
         *
         * The default scheme and port that will be used would be
         * http on port 80
         */
        bool m_is_empty;

        /**
         * Parse url
         *
         * @param url_ The string of url
         */
        void parse(const std::string& url_);

    public:

        static const unsigned short SCHEME      = 1<<0;
        static const unsigned short HOST        = 1<<1;
        static const unsigned short PORT        = 1<<2;
        static const unsigned short USER        = 1<<3;
        static const unsigned short PASSWORD    = 1<<4;
        static const unsigned short PATH        = 1<<5;
        static const unsigned short QUERY       = 1<<6;
        static const unsigned short FRAGMENT    = 1<<7;

        static const unsigned short HTTP_PORT   = 80;
        static const unsigned short HTTPS_PORT  = 443;

        url() :
            m_scheme("http"),
            m_port(HTTP_PORT),
            m_path("/"),
            m_is_empty(true)
        {
        }

        /**
         * Construct url object with url
         *
         * @param url_ Url string to set
         */
        url(const std::string& url_) : url()
        {
            setUrl(url_);
        }

        /**
         * Set url
         *
         * @param url_ Url string to set
         */
        void setUrl(const std::string& url_);

        /**
         * Set scheme of url
         *
         * @param scheme_ The string of scheme
         * @return void
         */
        void setScheme(const std::string& scheme_);

        /**
         * Get scheme (http/ftp/https...)
         *
         * @return The string of scheme
         */
        const std::string& getScheme() const;

        /**
         * Set host and turn
         *
         * @param host_ The string of host
         */
        void setHost(const std::string& host_);

        /**
         * Get host
         *
         * @return The string of host
         */
        const std::string& getHost() const;

        /**
         * Set port
         *
         * @param port_ Port number to set
         */
        void setPort(unsigned short port_);

        /**
         * Get port
         *
         * @return The port number
         */
        const unsigned short& getPort() const;

        /**
         * Set user
         *
         * @param user_ The string of user
         */
        void setUser(const std::string& user_);

        /**
         * Get user
         *
         * @return The string of user
         */
        const std::string& getUser() const;

        /**
         * Set password
         *
         * @param password_ The string of password
         */
        void setPassword(const std::string& password_);

        /**
         * Get password
         *
         * @return The string of password
         */
        const std::string& getPassword() const;

        /**
         * Set path
         *
         * @param path_ string of path
         */
        void setPath(const std::string& path_);

        /**
         * Get path
         *
         * @return The string of path
         */
        const std::string& getPath() const;

        /**
         * Set query
         *
         * @param query_ The string of query
         */
        void setQuery(const std::string& query_);

        /**
         * Get query
         *
         * @return The string of query
         */
        const std::string& getQuery() const;

        /**
         * Set fragment
         *
         * @param fragment_ The string of fragment
         */
        void setFragment(const std::string& fragment_);

        /**
         * Get fragment
         *
         * @return The string of fragment
         */
        const std::string& getFragment() const;

        bool isEmpty() const;

        /**
         * Build url
         *
         * @param  component_ The part of url: -1 build the whole URL
         * @return            The string of built url
         */
        std::string toString(int component_ = -1) const;
    };

} // end of uri namespace
} // end of reactive namespace
