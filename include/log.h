#pragma once

#define FMT_HEADER_ONLY

#include <fmt/format.h>

namespace cin
{
    namespace log
    {
        /**
         * Log level.
         */
        enum class Level
        {
            debug = 0,
            info,
            warning,
            error,
        };

        /**
         * Initialize logging.
         *
         * Sets the default log level to Level::info or a level passed as the
         * environment variable `CINEMO_LOG` (one of `debug`, `info`, `warning`,
         * `error`).
         */
        void init();

        namespace detail
        {
            void log(Level level, fmt::string_view format, fmt::format_args args);
        }

        template <typename S, typename... Args>
        void debug(const S& format, Args&&... args)
        {
            detail::log(Level::debug, format, fmt::make_args_checked<Args...>(format, args...));
        }

        template <typename S, typename... Args>
        void info(const S& format, Args&&... args)
        {
            detail::log(Level::info, format, fmt::make_args_checked<Args...>(format, args...));
        }

        template <typename S, typename... Args>
        void warn(const S& format, Args&&... args)
        {
            detail::log(Level::warning, format, fmt::make_args_checked<Args...>(format, args...));
        }

        template <typename S, typename... Args>
        void error(const S& format, Args&&... args)
        {
            detail::log(Level::error, format, fmt::make_args_checked<Args...>(format, args...));
        }
    }
}
