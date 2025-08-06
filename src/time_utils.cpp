#include "baba/time_utils.hpp"
#include "baba/logger.hpp"
#include <thread>
#include <sstream>
#include <iomanip>

namespace baba::core::time_utils {

    TimePoint now() {
        return Clock::now();
    }

    std::int64_t now_millis() {
        auto now_time = now();
        auto duration = now_time.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    std::int64_t now_seconds() {
        auto now_time = now();
        auto duration = now_time.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }

    std::string format_time(const TimePoint& time_point, const std::string& format) {
        auto time_t = Clock::to_time_t(time_point);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), format.c_str());
        return oss.str();
    }

    std::string format_now(const std::string& format) {
        return format_time(now(), format);
    }

    TimePoint parse_iso_date(const std::string& iso_string) {
        std::tm tm = {};
        std::istringstream ss(iso_string);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        
        if (ss.fail()) {
            return TimePoint{}; // Return epoch on failure
        }
        
        auto time_t = std::mktime(&tm);
        return Clock::from_time_t(time_t);
    }

    Duration time_diff(const TimePoint& start, const TimePoint& end) {
        return end - start;
    }

    std::int64_t to_millis(const Duration& duration) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    double to_seconds(const Duration& duration) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

    double to_minutes(const Duration& duration) {
        return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60>>>(duration).count();
    }

    double to_hours(const Duration& duration) {
        return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<3600>>>(duration).count();
    }

    void sleep_millis(std::int64_t milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void sleep_seconds(double seconds) {
        auto duration = std::chrono::duration<double>(seconds);
        std::this_thread::sleep_for(duration);
    }

    // Timer implementation
    Timer::Timer() : start_time_(now()) {}

    void Timer::reset() {
        start_time_ = now();
    }

    Duration Timer::elapsed() const {
        return time_diff(start_time_, now());
    }

    std::int64_t Timer::elapsed_millis() const {
        return to_millis(elapsed());
    }

    double Timer::elapsed_seconds() const {
        return to_seconds(elapsed());
    }

    double Timer::elapsed_minutes() const {
        return to_minutes(elapsed());
    }

    // ScopedTimer implementation
    ScopedTimer::ScopedTimer(const std::string& name) 
        : name_(name), start_time_(now()) {}

    ScopedTimer::~ScopedTimer() {
        auto elapsed_time = elapsed();
        double seconds = to_seconds(elapsed_time);
        
        // Log the elapsed time using the logger
        Logger::instance().info_format("Timer '%s' elapsed: %.3f seconds", name_.c_str(), seconds);
    }

    Duration ScopedTimer::elapsed() const {
        return time_diff(start_time_, now());
    }

} // namespace baba::core::time_utils
