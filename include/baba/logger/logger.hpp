#pragma once

/**
 * @file logger.hpp
 * @brief Modern C++20 logging framework with lambda-based lazy evaluation
 */

#include <chrono>
#include <concepts>
#include <functional>
#include <map>
#include <string>
#include <string_view>

namespace baba::logger
{

    /// Log severity levels (TRACE < DEBUG < INFO < WARN < ERROR < FATAL)
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    std::string log_level_to_string(LogLevel level);

    template <typename T>
    concept MessageProvider =
        std::invocable<T> && std::convertible_to<std::invoke_result_t<T>, std::string>;

    template <typename T>
    concept StringLike =
        std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

    /// Logger configuration
    struct Logger {
        bool                               async_enabled{true};
        bool                               file_enabled{false};
        bool                               include_location{false};
        bool                               include_performance_metrics{false};
        bool                               include_thread_id{true};
        bool                               include_timestamp{true};
        bool                               stdout_enabled{true};
        LogLevel                           min_log_level{LogLevel::INFO};
        size_t                             async_buffer_size{1024};
        std::chrono::milliseconds          flush_interval{100};
        std::map<std::string, std::string> context;
        std::string                        file_path;
        std::string                        name;
        std::string                        timestamp_format{"%Y-%m-%d %H:%M:%S"};
    };

    extern Logger g_default_logger;

    // Global logging functions
    template <MessageProvider T>
    void log(LogLevel level, T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void trace(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void debug(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void info(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void warn(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void error(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void fatal(T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    // Custom logger functions
    template <MessageProvider T>
    void logger_log(Logger* logger,
        LogLevel            level,
        T&&                 msg_provider,
        const char*         file = __FILE__,
        int                 line = __LINE__);

    template <MessageProvider T>
    void logger_trace(Logger* logger,
        T&&                   msg_provider,
        const char*           file = __FILE__,
        int                   line = __LINE__);

    template <MessageProvider T>
    void logger_debug(Logger* logger,
        T&&                   msg_provider,
        const char*           file = __FILE__,
        int                   line = __LINE__);

    template <MessageProvider T>
    void
    logger_info(Logger* logger, T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void
    logger_warn(Logger* logger, T&& msg_provider, const char* file = __FILE__, int line = __LINE__);

    template <MessageProvider T>
    void logger_error(Logger* logger,
        T&&                   msg_provider,
        const char*           file = __FILE__,
        int                   line = __LINE__);

    template <MessageProvider T>
    void logger_fatal(Logger* logger,
        T&&                   msg_provider,
        const char*           file = __FILE__,
        int                   line = __LINE__);

    // Forward declaration for internal implementation
    void internal_log_impl(const Logger* logger,
        LogLevel                         level,
        const std::string&               message,
        const char*                      file,
        int                              line);

    template <MessageProvider T>
    void log(LogLevel level, T&& msg_provider, const char* file, int line)
    {
        // Get effective logger (global default if nullptr)
        extern Logger g_default_logger;
        const Logger* effective_logger = &g_default_logger;

        // Early log level filtering for performance
        if (level < effective_logger->min_log_level) {
            return;
        }

        // Invoke message provider only if logging is needed (lazy evaluation)
        std::string message = std::invoke(std::forward<T>(msg_provider));

        // Call core implementation
        internal_log_impl(nullptr, level, message, file, line);
    }

    template <MessageProvider T> void trace(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::TRACE, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T> void debug(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::DEBUG, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T> void info(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::INFO, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T> void warn(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::WARN, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T> void error(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::ERROR, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T> void fatal(T&& msg_provider, const char* file, int line)
    {
        log(LogLevel::FATAL, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_log(Logger* logger, LogLevel level, T&& msg_provider, const char* file, int line)
    {
        // Early log level filtering for performance
        const Logger* effective_logger = logger ? logger : &g_default_logger;
        if (level < effective_logger->min_log_level) {
            return;
        }

        // Invoke message provider only if logging is needed (lazy evaluation)
        std::string message = std::invoke(std::forward<T>(msg_provider));

        // Call core implementation
        internal_log_impl(logger, level, message, file, line);
    }

    template <MessageProvider T>
    void logger_trace(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::TRACE, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_debug(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::DEBUG, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_info(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::INFO, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_warn(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::WARN, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_error(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::ERROR, std::forward<T>(msg_provider), file, line);
    }

    template <MessageProvider T>
    void logger_fatal(Logger* logger, T&& msg_provider, const char* file, int line)
    {
        logger_log(logger, LogLevel::FATAL, std::forward<T>(msg_provider), file, line);
    }

} // namespace baba::logger
