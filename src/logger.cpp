#include "baba/logger.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>

namespace baba::core {

    std::string to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    Logger::Logger() 
        : min_level_(LogLevel::INFO)
        , console_output_(true)
        , file_stream_(nullptr) {
    }

    Logger::~Logger() {
        if (file_stream_) {
            file_stream_->flush();
        }
    }

    Logger& Logger::instance() {
        static Logger instance;
        return instance;
    }

    void Logger::set_level(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        min_level_ = level;
    }

    LogLevel Logger::get_level() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return min_level_;
    }

    void Logger::set_console_output(bool enable) {
        std::lock_guard<std::mutex> lock(mutex_);
        console_output_ = enable;
    }

    bool Logger::set_file_output(const std::string& filepath) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (filepath.empty()) {
            file_stream_.reset();
            return true;
        }
        
        auto new_stream = std::make_unique<std::ofstream>(filepath, std::ios::app);
        if (!new_stream->is_open()) {
            return false;
        }
        
        file_stream_ = std::move(new_stream);
        return true;
    }

    void Logger::log(LogLevel level, const std::string& message) {
        if (level < min_level_) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        std::string formatted = format_message(level, message);
        
        if (console_output_) {
            if (level >= LogLevel::ERROR) {
                std::cerr << formatted << std::endl;
            } else {
                std::cout << formatted << std::endl;
            }
        }
        
        if (file_stream_ && file_stream_->is_open()) {
            *file_stream_ << formatted << std::endl;
            file_stream_->flush();
        }
    }

    void Logger::debug_impl(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }

    void Logger::info_impl(const std::string& message) {
        log(LogLevel::INFO, message);
    }

    void Logger::warn_impl(const std::string& message) {
        log(LogLevel::WARN, message);
    }

    void Logger::error_impl(const std::string& message) {
        log(LogLevel::ERROR, message);
    }

    void Logger::flush() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (console_output_) {
            std::cout.flush();
            std::cerr.flush();
        }
        
        if (file_stream_ && file_stream_->is_open()) {
            file_stream_->flush();
        }
    }

    std::string Logger::format_message(LogLevel level, const std::string& message) {
        std::ostringstream oss;
        oss << "[" << get_timestamp() << "] "
            << "[" << std::setw(5) << to_string(level) << "] "
            << message;
        return oss.str();
    }

    std::string Logger::get_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

} // namespace baba::core
