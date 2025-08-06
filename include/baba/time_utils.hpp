#pragma once

#include <string>
#include <chrono>
#include <ctime>

/**
 * @file time_utils.hpp
 * @brief Time and date utility functions
 * @version 1.0.0
 */

namespace baba::core {

    /**
     * @brief Time and date utility functions
     */
    namespace time_utils {
        
        using Clock = std::chrono::system_clock;
        using TimePoint = std::chrono::system_clock::time_point;
        using Duration = std::chrono::system_clock::duration;
        
        /**
         * @brief Get current timestamp
         * @return Current time point
         */
        TimePoint now();
        
        /**
         * @brief Get current timestamp as milliseconds since epoch
         * @return Milliseconds since epoch
         */
        std::int64_t now_millis();
        
        /**
         * @brief Get current timestamp as seconds since epoch
         * @return Seconds since epoch
         */
        std::int64_t now_seconds();
        
        /**
         * @brief Format time point as string
         * @param time_point Time point to format
         * @param format Format string (strftime format)
         * @return Formatted time string
         */
        std::string format_time(const TimePoint& time_point, const std::string& format = "%Y-%m-%d %H:%M:%S");
        
        /**
         * @brief Format current time as string
         * @param format Format string (strftime format)
         * @return Formatted current time string
         */
        std::string format_now(const std::string& format = "%Y-%m-%d %H:%M:%S");
        
        /**
         * @brief Parse ISO 8601 date string
         * @param iso_string ISO 8601 formatted string (YYYY-MM-DD HH:MM:SS)
         * @return Time point, or epoch if parsing fails
         */
        TimePoint parse_iso_date(const std::string& iso_string);
        
        /**
         * @brief Calculate duration between two time points
         * @param start Start time point
         * @param end End time point
         * @return Duration between the time points
         */
        Duration time_diff(const TimePoint& start, const TimePoint& end);
        
        /**
         * @brief Convert duration to milliseconds
         * @param duration Duration to convert
         * @return Duration in milliseconds
         */
        std::int64_t to_millis(const Duration& duration);
        
        /**
         * @brief Convert duration to seconds
         * @param duration Duration to convert
         * @return Duration in seconds
         */
        double to_seconds(const Duration& duration);
        
        /**
         * @brief Convert duration to minutes
         * @param duration Duration to convert
         * @return Duration in minutes
         */
        double to_minutes(const Duration& duration);
        
        /**
         * @brief Convert duration to hours
         * @param duration Duration to convert
         * @return Duration in hours
         */
        double to_hours(const Duration& duration);
        
        /**
         * @brief Sleep for specified milliseconds
         * @param milliseconds Number of milliseconds to sleep
         */
        void sleep_millis(std::int64_t milliseconds);
        
        /**
         * @brief Sleep for specified seconds
         * @param seconds Number of seconds to sleep
         */
        void sleep_seconds(double seconds);
        
        /**
         * @brief Simple benchmark timer class
         */
        class Timer {
        public:
            /**
             * @brief Constructor - starts the timer
             */
            Timer();
            
            /**
             * @brief Reset the timer
             */
            void reset();
            
            /**
             * @brief Get elapsed time since construction or last reset
             * @return Elapsed duration
             */
            Duration elapsed() const;
            
            /**
             * @brief Get elapsed time in milliseconds
             * @return Elapsed time in milliseconds
             */
            std::int64_t elapsed_millis() const;
            
            /**
             * @brief Get elapsed time in seconds
             * @return Elapsed time in seconds
             */
            double elapsed_seconds() const;
            
            /**
             * @brief Get elapsed time in minutes
             * @return Elapsed time in minutes
             */
            double elapsed_minutes() const;
            
        private:
            TimePoint start_time_;
        };
        
        /**
         * @brief RAII benchmark timer that logs elapsed time on destruction
         */
        class ScopedTimer {
        public:
            /**
             * @brief Constructor
             * @param name Name for the timer (used in log message)
             */
            explicit ScopedTimer(const std::string& name);
            
            /**
             * @brief Destructor - logs elapsed time
             */
            ~ScopedTimer();
            
            /**
             * @brief Get elapsed time since construction
             * @return Elapsed duration
             */
            Duration elapsed() const;
            
        private:
            std::string name_;
            TimePoint start_time_;
        };
        
        /**
         * @brief Create a scoped timer with given name
         * @param name Name for the timer
         * @return ScopedTimer instance
         */
        inline ScopedTimer make_scoped_timer(const std::string& name) {
            return ScopedTimer(name);
        }
        
        // Convenience macros for scoped timing
        #define BABA_TIME_SCOPE(name) auto _timer = baba::core::time_utils::make_scoped_timer(name)
        #define BABA_TIME_FUNCTION() BABA_TIME_SCOPE(__FUNCTION__)
        
    } // namespace time_utils
    
} // namespace baba::core
