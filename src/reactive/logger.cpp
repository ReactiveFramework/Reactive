/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string>
#include <reactive/logger.hpp>

namespace reactive {

    std::string logger::identification  = "";
    int logger::level                   = LOG_INFO;

    void logger::write(int priority_, const std::string& message_, va_list list_)
    {
        setlogmask(LOG_UPTO(level));

        if (identification.empty())
        {
            openlog(NULL, LOG_CONS|LOG_PID|LOG_NDELAY, LOG_USER);
        }
        else
        {
            openlog(identification.c_str(), LOG_CONS|LOG_PID|LOG_NDELAY, LOG_USER);
        }

        vsyslog(priority_, message_.c_str(), list_);

        closelog();
    }

    void logger::setId(const std::string& identification_)
    {
        identification = identification_;
    }

    void logger::setLevel(int priority_)
    {
        level = priority_;
    }

    void logger::write(int priority_, const std::string& message_, ...)
    {
        va_list list;
        va_start(list, message_);
        write(priority_, message_, list);
        va_end(list);
    }

} // end of reactive namespace
