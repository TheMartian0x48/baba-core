#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>

/**
 * @file logger.hpp
 * @brief Logging framework with multiple levels and outputs
 * @version 1.0.0
 */

namespace baba::core {

    /**
     * @brief Logging levels
     */
    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3
    };

    /**
     * @brief Convert log level to string
     * @param level The log level
     * @return String representation of log level
     */
    std::string to_string(LogLevel level);

    /**
     * @brief Thread-safe logger class
     */
    class Logger {
    public:
        /**
         * @brief Get the singleton logger instance
         * @return Reference to the logger instance
         */
        static Logger& instance();

        /**
         * @brief Set the minimum log level
         * @param level Minimum level to log
         */
        void set_level(LogLevel level);

        /**
         * @brief Get the current log level
         * @return Current minimum log level
         */
        LogLevel get_level() const;

        /**
         * @brief Enable/disable console output
         * @param enable True to enable console output
         */
        void set_console_output(bool enable);

        /**
         * @brief Set file output
         * @param filepath Path to log file, empty to disable file output
         * @return True if successful, false otherwise
         */
        bool set_file_output(const std::string& filepath);

        /**
         * @brief Log a message with specified level
         * @param level Log level
         * @param message Message to log
         */
        void log(LogLevel level, const std::string& message);


        /**
         * @brief Format and log a message
         * @tparam Args Argument types
         * @param level Log level
         * @param format Format string
         * @param args Arguments to format
         */
        template<typename... Args>
        void log_format(LogLevel level, const std::string& format, Args... args) {
            if (level < min_level_) return;
            
            int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
            if (size <= 0) return;
            
            std::unique_ptr<char[]> buf(new char[size]);
            std::snprintf(buf.get(), size, format.c_str(), args...);
            std::string message(buf.get(), buf.get() + size - 1);
            
            log(level, message);
        }

        /**
         * @brief Format and log a debug message
         * @tparam Args Argument types
         * @param format Format string
         * @param args Arguments to format
         */
        template<typename... Args>
        void debug_format(const std::string& format, Args... args) {
            log_format(LogLevel::DEBUG, format, args...);
        }

        /**
         * @brief Format and log an info message
         * @tparam Args Argument types
         * @param format Format string
         * @param args Arguments to format
         */
        template<typename... Args>
        void info_format(const std::string& format, Args... args) {
            log_format(LogLevel::INFO, format, args...);
        }

        /**
         * @brief Format and log a warning message
         * @tparam Args Argument types
         * @param format Format string
         * @param args Arguments to format
         */
        template<typename... Args>
        void warn_format(const std::string& format, Args... args) {
            log_format(LogLevel::WARN, format, args...);
        }

        /**
         * @brief Format and log an error message
         * @tparam Args Argument types
         * @param format Format string
         * @param args Arguments to format
         */
        template<typename... Args>
        void error_format(const std::string& format, Args... args) {
            log_format(LogLevel::ERROR, format, args...);
        }

        /**
         * @brief Flush all output streams
         */
        void flush();

        // Static convenience methods
        static void set_global_level(LogLevel level) { instance().set_level(level); }
        static void enable_console(bool enable = true) { instance().set_console_output(enable); }
        static bool set_file(const std::string& filepath) { return instance().set_file_output(filepath); }
        
        static void debug(const std::string& message) { instance().debug_impl(message); }
        static void info(const std::string& message) { instance().info_impl(message); }
        static void warn(const std::string& message) { instance().warn_impl(message); }
        static void error(const std::string& message) { instance().error_impl(message); }

    private:
        Logger();
        ~Logger();
        
        // Non-copyable
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        /**
         * @brief Log a debug message (private instance method)
         * @param message Message to log
         */
        void debug_impl(const std::string& message);

        /**
         * @brief Log an info message (private instance method)
         * @param message Message to log
         */
        void info_impl(const std::string& message);

        /**
         * @brief Log a warning message (private instance method)
         * @param message Message to log
         */
        void warn_impl(const std::string& message);

        /**
         * @brief Log an error message (private instance method)
         * @param message Message to log
         */
        void error_impl(const std::string& message);

        std::string format_message(LogLevel level, const std::string& message);
        std::string get_timestamp();

        LogLevel min_level_;
        bool console_output_;
        std::unique_ptr<std::ofstream> file_stream_;
        mutable std::mutex mutex_;
    };

} // namespace baba::core
