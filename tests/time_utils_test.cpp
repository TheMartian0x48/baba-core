#include <gtest/gtest.h>
#include <baba/time_utils.hpp>
#include <thread>
#include <chrono>

using namespace baba::core::time_utils;

class TimeUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Record start time for tests
        start_time = now();
    }
    
    TimePoint start_time;
};

// Basic time functions tests
TEST_F(TimeUtilsTest, NowFunction) {
    TimePoint time1 = now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    TimePoint time2 = now();
    
    // time2 should be after time1
    EXPECT_GT(time2, time1);
}

TEST_F(TimeUtilsTest, NowMillis) {
    std::int64_t millis1 = now_millis();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::int64_t millis2 = now_millis();
    
    // Should have at least 10ms difference
    EXPECT_GE(millis2 - millis1, 10);
}

TEST_F(TimeUtilsTest, NowSeconds) {
    std::int64_t seconds1 = now_seconds();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::int64_t seconds2 = now_seconds();
    
    // Should be same second or next second
    EXPECT_GE(seconds2, seconds1);
    EXPECT_LE(seconds2 - seconds1, 1);
}

// Time formatting tests
TEST_F(TimeUtilsTest, FormatTime) {
    TimePoint test_time = now();
    
    // Test basic format
    std::string formatted = format_time(test_time, "%Y-%m-%d");
    EXPECT_FALSE(formatted.empty());
    EXPECT_EQ(formatted.length(), 10); // YYYY-MM-DD format
    
    // Test time format
    std::string time_formatted = format_time(test_time, "%H:%M:%S");
    EXPECT_FALSE(time_formatted.empty());
    EXPECT_EQ(time_formatted.length(), 8); // HH:MM:SS format
}

TEST_F(TimeUtilsTest, FormatNow) {
    std::string formatted = format_now("%Y-%m-%d %H:%M:%S");
    EXPECT_FALSE(formatted.empty());
    EXPECT_EQ(formatted.length(), 19); // YYYY-MM-DD HH:MM:SS format
}

// ISO date parsing tests
TEST_F(TimeUtilsTest, ParseIsoDateValid) {
    std::string iso_string = "2023-12-25 15:30:45";
    TimePoint parsed = parse_iso_date(iso_string);
    
    // Should not be epoch (default return for failure)
    TimePoint epoch{};
    EXPECT_NE(parsed, epoch);
    
    // Format back and check (basic validation)
    std::string formatted = format_time(parsed, "%Y-%m-%d %H:%M:%S");
    EXPECT_EQ(formatted, iso_string);
}

TEST_F(TimeUtilsTest, ParseIsoDateInvalid) {
    std::string invalid_string = "invalid-date-format";
    TimePoint parsed = parse_iso_date(invalid_string);
    
    // Should return epoch for invalid input
    TimePoint epoch{};
    EXPECT_EQ(parsed, epoch);
}

// Duration operations tests
TEST_F(TimeUtilsTest, TimeDiff) {
    TimePoint time1 = now();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    TimePoint time2 = now();
    
    Duration diff = time_diff(time1, time2);
    
    // Should be positive duration
    EXPECT_GT(diff.count(), 0);
}

TEST_F(TimeUtilsTest, ToMillis) {
    Duration duration = std::chrono::milliseconds(1500);
    std::int64_t millis = to_millis(duration);
    
    EXPECT_EQ(millis, 1500);
}

TEST_F(TimeUtilsTest, ToSeconds) {
    Duration duration = std::chrono::milliseconds(2500);
    double seconds = to_seconds(duration);
    
    EXPECT_DOUBLE_EQ(seconds, 2.5);
}

TEST_F(TimeUtilsTest, ToMinutes) {
    Duration duration = std::chrono::seconds(150);
    double minutes = to_minutes(duration);
    
    EXPECT_DOUBLE_EQ(minutes, 2.5);
}

TEST_F(TimeUtilsTest, ToHours) {
    Duration duration = std::chrono::minutes(150);
    double hours = to_hours(duration);
    
    EXPECT_DOUBLE_EQ(hours, 2.5);
}

// Sleep functions tests
TEST_F(TimeUtilsTest, SleepMillis) {
    TimePoint before = now();
    sleep_millis(100);
    TimePoint after = now();
    
    Duration elapsed = time_diff(before, after);
    std::int64_t elapsed_millis = to_millis(elapsed);
    
    // Should have slept at least 100ms (allowing some tolerance)
    EXPECT_GE(elapsed_millis, 90);
    EXPECT_LE(elapsed_millis, 200); // Upper bound for reasonable execution
}

TEST_F(TimeUtilsTest, SleepSeconds) {
    TimePoint before = now();
    sleep_seconds(0.1); // 100ms
    TimePoint after = now();
    
    Duration elapsed = time_diff(before, after);
    double elapsed_seconds = to_seconds(elapsed);
    
    // Should have slept at least 0.1 seconds (allowing some tolerance)
    EXPECT_GE(elapsed_seconds, 0.09);
    EXPECT_LE(elapsed_seconds, 0.2); // Upper bound for reasonable execution
}

// Timer class tests
TEST_F(TimeUtilsTest, TimerBasicUsage) {
    Timer timer;
    
    // Should start with very small elapsed time
    Duration initial_elapsed = timer.elapsed();
    EXPECT_GE(to_millis(initial_elapsed), 0);
    EXPECT_LE(to_millis(initial_elapsed), 10); // Should be very small
    
    // Sleep and check elapsed time
    sleep_millis(50);
    Duration elapsed = timer.elapsed();
    std::int64_t elapsed_millis = timer.elapsed_millis();
    
    EXPECT_GE(elapsed_millis, 40);
    EXPECT_LE(elapsed_millis, 100);
    EXPECT_EQ(to_millis(elapsed), elapsed_millis);
}

TEST_F(TimeUtilsTest, TimerReset) {
    Timer timer;
    
    // Let some time pass
    sleep_millis(50);
    Duration elapsed_before_reset = timer.elapsed();
    
    // Reset timer
    timer.reset();
    Duration elapsed_after_reset = timer.elapsed();
    
    // After reset, elapsed time should be much smaller
    EXPECT_GT(to_millis(elapsed_before_reset), to_millis(elapsed_after_reset));
    EXPECT_LE(to_millis(elapsed_after_reset), 10);
}

TEST_F(TimeUtilsTest, TimerElapsedMethods) {
    Timer timer;
    sleep_millis(100);
    
    std::int64_t millis = timer.elapsed_millis();
    double seconds = timer.elapsed_seconds();
    double minutes = timer.elapsed_minutes();
    
    EXPECT_GE(millis, 90);
    EXPECT_LE(millis, 200);
    
    EXPECT_GE(seconds, 0.09);
    EXPECT_LE(seconds, 0.2);
    
    EXPECT_GE(minutes, 0.0015);
    EXPECT_LE(minutes, 0.004);
    
    // Check consistency between methods
    EXPECT_DOUBLE_EQ(seconds, millis / 1000.0);
    EXPECT_DOUBLE_EQ(minutes, seconds / 60.0);
}

// ScopedTimer class tests
TEST_F(TimeUtilsTest, ScopedTimerBasicUsage) {
    // This test mainly ensures ScopedTimer doesn't crash
    // The actual logging output is hard to test in unit tests
    {
        ScopedTimer scoped_timer("Test Operation");
        sleep_millis(10);
        // Timer should log when it goes out of scope
    }
    
    // If we reach here, ScopedTimer worked without crashing
    EXPECT_TRUE(true);
}

TEST_F(TimeUtilsTest, ScopedTimerElapsed) {
    ScopedTimer scoped_timer("Test Operation");
    sleep_millis(50);
    
    Duration elapsed = scoped_timer.elapsed();
    std::int64_t elapsed_millis = to_millis(elapsed);
    
    EXPECT_GE(elapsed_millis, 40);
    EXPECT_LE(elapsed_millis, 100);
}

TEST_F(TimeUtilsTest, ScopedTimerMultipleInstances) {
    // Test multiple scoped timers
    {
        ScopedTimer timer1("Operation 1");
        sleep_millis(20);
        
        {
            ScopedTimer timer2("Operation 2");
            sleep_millis(10);
        } // timer2 should log here
        
        sleep_millis(10);
    } // timer1 should log here
    
    EXPECT_TRUE(true); // If we reach here, no crashes occurred
}

// Edge cases and stress tests
TEST_F(TimeUtilsTest, ZeroDuration) {
    Duration zero_duration = std::chrono::milliseconds(0);
    
    EXPECT_EQ(to_millis(zero_duration), 0);
    EXPECT_DOUBLE_EQ(to_seconds(zero_duration), 0.0);
    EXPECT_DOUBLE_EQ(to_minutes(zero_duration), 0.0);
    EXPECT_DOUBLE_EQ(to_hours(zero_duration), 0.0);
}

TEST_F(TimeUtilsTest, LargeDuration) {
    Duration large_duration = std::chrono::hours(25); // More than a day
    
    double hours = to_hours(large_duration);
    double minutes = to_minutes(large_duration);
    double seconds = to_seconds(large_duration);
    
    EXPECT_DOUBLE_EQ(hours, 25.0);
    EXPECT_DOUBLE_EQ(minutes, 25.0 * 60.0);
    EXPECT_DOUBLE_EQ(seconds, 25.0 * 3600.0);
}

TEST_F(TimeUtilsTest, NegativeDuration) {
    TimePoint time1 = now();
    TimePoint time2 = time1 - std::chrono::milliseconds(100);
    
    Duration negative_duration = time_diff(time1, time2);
    
    // Should be negative
    EXPECT_LT(negative_duration.count(), 0);
    EXPECT_LT(to_millis(negative_duration), 0);
}

TEST_F(TimeUtilsTest, TimerPrecision) {
    Timer timer;
    
    // Test multiple short measurements
    std::vector<std::int64_t> measurements;
    
    for (int i = 0; i < 5; ++i) {
        timer.reset();
        sleep_millis(10);
        measurements.push_back(timer.elapsed_millis());
    }
    
    // All measurements should be reasonably close to 10ms
    for (auto measurement : measurements) {
        EXPECT_GE(measurement, 5);
        EXPECT_LE(measurement, 30);
    }
}

TEST_F(TimeUtilsTest, ConcurrentTimers) {
    // Test that multiple timers work independently
    Timer timer1;
    sleep_millis(20);
    
    Timer timer2;
    sleep_millis(30);
    
    std::int64_t elapsed1 = timer1.elapsed_millis();
    std::int64_t elapsed2 = timer2.elapsed_millis();
    
    // timer1 should have more elapsed time than timer2
    EXPECT_GT(elapsed1, elapsed2);
    EXPECT_GE(elapsed1, 45); // At least 20 + 30 - tolerance
    EXPECT_GE(elapsed2, 25); // At least 30 - tolerance
}

TEST_F(TimeUtilsTest, FormatTimeEdgeCases) {
    TimePoint test_time = now();
    
    // Empty format string
    std::string empty_format = format_time(test_time, "");
    EXPECT_TRUE(empty_format.empty());
    
    // Complex format string
    std::string complex_format = format_time(test_time, "%A, %B %d, %Y at %I:%M %p");
    EXPECT_FALSE(complex_format.empty());
    EXPECT_GT(complex_format.length(), 20);
}
