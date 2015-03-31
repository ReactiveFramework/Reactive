/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <cstdarg>
#include <string>
#include <syslog.h>

namespace reactive {

    class logger
    {
    private:
        static std::string identification;
        static int level;

        logger() {}

        /**
         * Write a message to syslog
         *
         * @param priority_ priority LOG_DEBUG/LOG_INFO/LOG_NOTICE/LOG_WARNING/LOG_ERR...
         * @param message_  message string or format string
         * @param list_     VA list - list of arguments
         */
        static void write(int priority_, const std::string& message_, va_list list_);

    public:
        /**
         * Set identification
         *
         * @param identification_ The identification for syslog messages.
         */
        static void setId(const std::string& identification_);

        /**
         * Logging messages which are less severe than priority_ will be ignored.
         *
         * @param priority_ priority LOG_DEBUG/LOG_INFO/LOG_NOTICE/LOG_WARNING/LOG_ERR...
         */
        static void setLevel(int priority_);

        /**
         * Write a message to syslog with any priority
         *
         * @param priority_ priority LOG_DEBUG/LOG_INFO/LOG_NOTICE/LOG_WARNING/LOG_ERR...
         * @param message_  message string or format string
         * @param ...       More arguments if a format is used
         */
        static void write(int priority_, const std::string& message_, ...);

        #define MAKE_PRIORITY_METHOD(name, priority)        \
        static void name(const std::string& message_, ...)  \
        {                                                   \
            va_list list;                                   \
            va_start(list, message_);                       \
            write(priority, message_, list);                \
            va_end(list);                                   \
        }

        MAKE_PRIORITY_METHOD(emergency,   LOG_EMERG);
        MAKE_PRIORITY_METHOD(alert,       LOG_ALERT);
        MAKE_PRIORITY_METHOD(critical,    LOG_CRIT);
        MAKE_PRIORITY_METHOD(error,       LOG_ERR);
        MAKE_PRIORITY_METHOD(warning,     LOG_WARNING);
        MAKE_PRIORITY_METHOD(notice,      LOG_NOTICE);
        MAKE_PRIORITY_METHOD(info,        LOG_INFO);
        MAKE_PRIORITY_METHOD(debug,       LOG_DEBUG);

        #undef MAKE_PRIORITY_METHOD
    };

} // end of reactive namespace
