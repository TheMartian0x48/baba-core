/**
 * @file logger.cpp
 * @brief Implementation of the C++20 logging framework
 */

#include "baba/logger/logger.hpp"

#include <sys/resource.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <ranges>
#include <sstream>
#include <thread>

namespace baba::logger {

struct LogEntry;
class AsyncLogger;

// Global async logger instance (thread-safe singleton)
static std::unique_ptr<AsyncLogger> g_async_logger;

// Global mutex for thread-safe access
static std::mutex g_global_mutex;

// Default global logger configuration
Logger g_default_logger;

std::string log_level_to_string(LogLevel level) {
    switch (level) {
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

/**
 * @struct LogEntry
 * @brief Internal structure representing a complete log entry for async processing
 *
 */
struct LogEntry {
    bool file_enabled;                ///< Whether to output to file
    bool include_performance_metrics; ///< Whether to include performance metrics
    bool include_thread_id;           ///< Whether to include thread ID in output
    bool include_timestamp;           ///< Whether to include timestamp in output
    bool stdout_enabled;              ///< Whether to output to stdout/stderr
    bool include_location;            ///< Whether to include source location in output
    LogLevel level;                   ///< Severity level of the log message
    std::thread::id thread_id;        ///< ID of the thread that created this entry
    std::chrono::system_clock::time_point timestamp; ///< When the log entry was created
    std::map<std::string, std::string> context;      ///< Key-value context pairs
    int line_number;                                 ///< Line number in source code (if enabled)
    std::string src_file;                            ///< Source file name (if enabled)
    std::string file_path;                           ///< Path to output file if file_enabled
    std::string logger_name;                         ///< Name of the logger that created this entry
    std::string message;                             ///< The actual log message content
    std::string message_prefix;                      ///< The actual log message content
    std::string message_suffix;                      ///< The actual log message content
    std::string timestamp_format;                    ///< Format string for timestamp display
    /**
     * @brief Create a LogEntry from logger configuration and message details
     * @param logger Pointer to logger configuration (nullptr uses global default)
     * @param level Log level for this entry
     * @param message The formatted message content
     * @return Complete LogEntry ready for processing
     *
     * @details Captures all necessary information from the logger configuration
     *          at the time of logging to ensure consistent behavior even if the
     *          logger configuration changes before the message is processed.
     *
     * The function:
     * 1. Determines the effective logger (custom or global default)
     * 2. Captures current timestamp and thread information
     * 3. Copies all relevant configuration flags and settings
     * 4. Creates a complete LogEntry for async processing
     *
     * @note This function captures the logger state at call time
     * @see LogEntry for structure details
     */
    static LogEntry create_log_entry(const Logger *logger,
        LogLevel level,
        const std::string &message,
        const char *file,
        const int line = 0,
        const std::string &prefix = "",
        const std::string &suffix = "") {
        const Logger *effective_logger = logger ? logger : &g_default_logger;

        LogEntry entry;
        entry.level = level;
        entry.message = message;
        entry.message_prefix = prefix;
        entry.message_suffix = suffix;
        entry.logger_name = effective_logger->name;
        entry.context = effective_logger->context;
        entry.timestamp = std::chrono::system_clock::now();
        entry.thread_id = std::this_thread::get_id();
        entry.include_timestamp = effective_logger->include_timestamp;
        entry.include_thread_id = effective_logger->include_thread_id;
        entry.include_performance_metrics = effective_logger->include_performance_metrics;
        entry.timestamp_format = effective_logger->timestamp_format;
        entry.stdout_enabled = effective_logger->stdout_enabled;
        entry.file_enabled = effective_logger->file_enabled;
        entry.file_path = effective_logger->file_path;
        entry.include_location = effective_logger->include_location;
        entry.line_number = line;
        entry.src_file = effective_logger->include_location ? file : "";

        return entry;
    }
};

/**
 * @struct PerformanceMetrics
 * @brief Helper structure for collecting and formatting system performance metrics
 *
 * @note Performance metric collection may impact logging performance
 * @see Logger::include_performance_metrics
 */
struct PerformanceMetrics {
    size_t memory_usage_mb = 0;     ///< Current memory usage in megabytes
    double cpu_usage_percent = 0.0; ///< Current CPU usage percentage (currently unused)

    /**
     * @brief Collect current system performance metrics
     * @return PerformanceMetrics structure with current system information
     *
     * @details Uses system calls to gather performance information:
     *          - Memory usage via getrusage() system call
     *          - CPU usage is currently not implemented (returns 0.0)
     *
     * @note This function may block briefly due to system calls
     * @warning Frequent calls may impact application performance
     */
    static PerformanceMetrics get_current() {
        PerformanceMetrics metrics;

        // Get memory usage using getrusage system call
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            metrics.memory_usage_mb = usage.ru_maxrss / 1024; // Convert KB to MB on Linux
        }

        // CPU usage calculation is complex and not currently implemented
        // Would require tracking CPU time over intervals for accurate measurement
        metrics.cpu_usage_percent = 0.0;

        return metrics;
    }
};

/**
 * @class AsyncLogger
 * @brief Thread-safe asynchronous logger implementation with background processing
 *
 * The AsyncLogger class provides high-performance logging by decoupling log message
 * creation from output processing. Messages are queued and processed by a dedicated
 * background worker thread, minimizing blocking of application threads.
 *
 * Key features:
 * - Non-blocking message enqueueing for calling threads
 * - Background worker thread for message processing and output
 * - File stream caching for improved I/O performance
 * - Graceful shutdown with message queue flushing
 * - Thread-safe operations with proper synchronization
 *
 * @note This is an internal implementation class, not part of the public API
 * @warning Only one instance should exist per application (managed globally)
 */
class AsyncLogger {
  private:
    std::queue<LogEntry> log_queue;           ///< Queue of pending log entries
    std::mutex queue_mutex;                   ///< Mutex protecting the log queue
    std::condition_variable queue_cv;         ///< Condition variable for worker thread signaling
    std::thread worker_thread;                ///< Background worker thread
    std::atomic<bool> shotdown{false};        ///< Atomic flag indicating shutdown state
    std::atomic<bool> flush_requested{false}; ///< Atomic flag for flush requests

    /// @brief Cache of open file streams for improved performance
    /// @details Keeps file streams open to avoid repeated open/close operations
    std::map<std::string, std::unique_ptr<std::ofstream>> file_streams_;
    std::mutex file_streams_mutex_; ///< Mutex protecting file streams cache

  public:
    AsyncLogger() : worker_thread(&AsyncLogger::worker_loop, this) {}
    ~AsyncLogger() { shutdown(); }

    /**
     * @brief Add a log entry to the processing queue (non-blocking)
     * @param entry The log entry to be processed
     *
     * @details Adds the log entry to the internal queue and notifies the worker
     *          thread. This operation is designed to be fast and non-blocking
     *          for the calling thread.
     *
     * @note Thread-safe operation
     * @note Does not block the calling thread (unless queue is full)
     */
    void enqueue(const LogEntry &entry) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            log_queue.push(entry);
        }
        queue_cv.notify_one();
    }

    /**
     * @brief Force immediate processing of all queued log entries
     * @details Requests the worker thread to flush all pending entries and
     *          blocks until the queue is empty. Useful for ensuring all
     *          messages are written before critical operations.
     *
     * @note This operation blocks until all queued entries are processed
     * @note Thread-safe operation
     * @warning May impact performance due to blocking nature
     */
    void flush() {
        flush_requested = true;
        queue_cv.notify_one();

        // Wait for queue to be empty
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [this] { return log_queue.empty(); });
    }

    /**
     * @brief Initiate graceful shutdown of the async logger
     * @details Sets the shutdown flag, processes all remaining queued entries,
     *          and joins the worker thread. This ensures no log messages are lost.
     *
     * @note Blocks until worker thread completes and joins
     * @note Safe to call multiple times (subsequent calls are no-ops)
     * @note All remaining queued messages will be processed before shutdown
     */
    void shutdown() {
        if (!shotdown.exchange(true)) {
            queue_cv.notify_one();
            if (worker_thread.joinable()) {
                worker_thread.join();
            }
        }
    }

  private:
    /**
     * @brief Main worker thread loop for processing log entries
     * @details Continuously processes log entries from the queue until shutdown.
     *          Handles both normal processing and flush requests while maintaining
     *          proper synchronization with the main thread.
     *
     * The worker loop:
     * 1. Waits for log entries, flush requests, or shutdown signals
     * 2. Processes all available entries in batches for efficiency
     * 3. Handles flush requests by forcing stream flushes
     * 4. Ensures all remaining entries are processed during shutdown
     */
    void worker_loop() {
        while (!shotdown) {
            std::unique_lock<std::mutex> lock(queue_mutex);

            queue_cv.wait(
                lock, [this] { return !log_queue.empty() || shotdown || flush_requested; });

            while (!log_queue.empty()) {
                LogEntry entry = log_queue.front();
                log_queue.pop();
                lock.unlock();

                process_log_entry(entry);

                lock.lock();
            }

            if (flush_requested) {
                flush_all_streams();
                flush_requested = false;
                queue_cv.notify_all();
            }
        }

        std::lock_guard<std::mutex> lock(queue_mutex);
        while (!log_queue.empty()) {
            process_log_entry(log_queue.front());
            log_queue.pop();
        }
        flush_all_streams();
    }

    /**
     * @brief Process a single log entry by formatting and outputting it
     * @param entry The log entry to process
     *
     * @details Formats the log entry according to its configuration and outputs
     *          it to the appropriate targets (stdout/stderr and/or file).
     *          Error-level and above messages are sent to stderr, others to stdout.
     *
     */
    void process_log_entry(const LogEntry &entry) {
        std::string formatted_message = format_log_entry(entry);

        if (entry.stdout_enabled) {
            if (entry.level >= LogLevel::ERROR) {
                std::cerr << formatted_message << std::endl;
            } else {
                std::cout << formatted_message << std::endl;
            }
        }

        if (entry.file_enabled && !entry.file_path.empty()) {
            write_to_file(entry.file_path, formatted_message);
        }
    }

    /**
     * @brief Format a log entry into a human-readable string
     * @param entry The log entry to format
     * @return Formatted log message string
     *
     * @details Creates a formatted log message including:
     *          - Timestamp with millisecond precision (if enabled)
     *          - Log level in brackets
     *          - Logger name (if specified)
     *          - Thread ID (if enabled)
     *          - Performance metrics (if enabled)
     *          - The actual message content
     *          - Context key-value pairs (if any)
     *
     * Example output format:
     * [2025-01-07 19:47:58.123] [INFO] [MyLogger] [thread:140234] [perf: mem=45MB] Message content
     * {key=value}
     */
    std::string format_log_entry(const LogEntry &entry) {
        std::ostringstream oss;
        if (entry.include_location) {
            oss << "[" << std::setw(4) << entry.line_number << ":" << entry.src_file << "]";
        }

        if (entry.include_timestamp) {
            auto time_t = std::chrono::system_clock::to_time_t(entry.timestamp);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                          entry.timestamp.time_since_epoch()) %
                      1000;

            oss << "[";
            oss << std::put_time(std::localtime(&time_t), entry.timestamp_format.c_str());
            oss << "." << std::setfill('0') << std::setw(3) << ms.count();
            oss << "]";
        }

        oss << " [" << log_level_to_string(entry.level) << "]";

        if (!entry.logger_name.empty()) {
            oss << " [" << entry.logger_name << "]";
        }

        if (entry.include_thread_id) {
            oss << " [thread:" << entry.thread_id << "]";
        }

        if (entry.include_performance_metrics) {
            auto metrics = PerformanceMetrics::get_current();
            oss << " [perf: mem=" << metrics.memory_usage_mb << "MB";
            if (metrics.cpu_usage_percent > 0) {
                oss << " cpu=" << std::fixed << std::setprecision(1) << metrics.cpu_usage_percent
                    << "%";
            }
            oss << "]";
        }

        oss << " " << entry.message_prefix << entry.message << entry.message_suffix;

        if (!entry.context.empty()) {
            // TODO(TheMartian0x48): Implement context formatting
            //  oss << " {";
            //  // Use C++20 ranges for more expressive context formatting
            //  auto context_strings = entry.context | std::views::transform([](const auto &pair) {
            //      return pair.first + "=" + pair.second;
            //  });
            //
            //  bool first = true;
            //  for (const auto &context_str : context_strings) {
            //      if (!first)
            //          oss << ", ";
            //      oss << context_str;
            //      first = false;
            //  }
            //  oss << "}";
        }

        return oss.str();
    }

    /**
     * @brief Write a message to a file with stream caching
     * @param file_path Path to the output file
     * @param message Message to write to the file
     *
     * @details Manages file streams efficiently by caching open streams.
     *          Creates new files if they don't exist, appends to existing files.
     *          Handles file opening errors gracefully by logging to stderr.
     *
     * @note Thread-safe operation protected by file_streams_mutex_
     * @note Files are opened in append mode to preserve existing content
     * @warning Directory must exist or file creation will fail
     */
    void write_to_file(const std::string &file_path, const std::string &message) {
        std::lock_guard<std::mutex> lock(file_streams_mutex_);

        auto it = file_streams_.find(file_path);
        if (it == file_streams_.end()) {
            auto stream = std::make_unique<std::ofstream>(file_path, std::ios::app);
            if (stream->is_open()) {
                file_streams_[file_path] = std::move(stream);
                it = file_streams_.find(file_path);
            } else {
                std::cerr << "Failed to open log file: " << file_path << std::endl;
                return;
            }
        }

        if (it != file_streams_.end() && it->second->is_open()) {
            *(it->second) << message << std::endl;
        }
    }

    /**
     * @brief Flush all output streams to ensure data is written
     * @details Forces all buffered output to be written immediately.
     *          Flushes both standard streams (cout, cerr) and all cached file streams.
     *
     * @note Thread-safe operation protected by file_streams_mutex_
     * @note Called during flush requests and shutdown to prevent data loss
     * @warning May impact performance due to forced I/O operations
     */
    void flush_all_streams() {
        std::lock_guard<std::mutex> lock(file_streams_mutex_);

        std::cout.flush();
        std::cerr.flush();

        for (auto &[path, stream] : file_streams_) {
            if (stream && stream->is_open()) {
                stream->flush();
            }
        }
    }
};

/**
 * @brief Ensure the global async logger is initialized (thread-safe)
 * @details Creates the global AsyncLogger instance if it doesn't exist.
 *          Uses lazy initialization with double-checked locking pattern
 *          for thread safety and performance.
 *
 * @note Thread-safe operation protected by g_global_mutex
 * @note Called automatically when async logging is needed
 * @see g_async_logger for the global instance
 */
void ensure_async_logger() {
    std::lock_guard<std::mutex> lock(g_global_mutex);
    if (!g_async_logger) {
        g_async_logger = std::make_unique<AsyncLogger>();
    }
}

/**
 * @brief Core internal logging implementation called by template functions
 * @param logger Pointer to logger configuration (nullptr uses global default)
 * @param level Log level for this message
 * @param message The already-evaluated message string
 * @param file Source file name
 * @param line Source line number
 *
 * @details This is the core logging function that:
 *          1. Creates a complete log entry with current context
 *          2. Routes to async or sync processing based on configuration
 *
 * @note Log level filtering is done in template functions for performance
 * @note Message evaluation is done in template functions (lazy evaluation)
 * @note Thread-safe operation with proper synchronization
 */
void internal_log_impl(const Logger *logger,
    LogLevel level,
    const std::string &message,
    const char *file,
    const int line) {
    const Logger *effective_logger = logger ? logger : &g_default_logger;

    LogEntry entry = LogEntry::create_log_entry(effective_logger, level, message, file, line);

    if (effective_logger->async_enabled) {
        ensure_async_logger();
        g_async_logger->enqueue(entry);
    } else {
        AsyncLogger temp_logger;
        temp_logger.enqueue(entry);
    }
}

} // namespace baba::logger
