#include <baba/time_utils.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "=== Time Utils Advanced Example ===" << std::endl << std::endl;
    
    // Basic time operations
    std::cout << "=== Basic Time Operations ===" << std::endl;
    auto current_time = baba::core::time_utils::now();
    std::cout << "Current time: " << baba::core::time_utils::format_time(current_time) << std::endl;
    std::cout << "Current timestamp (ms): " << baba::core::time_utils::now_millis() << std::endl;
    std::cout << "Current timestamp (s): " << baba::core::time_utils::now_seconds() << std::endl;
    std::cout << std::endl;
    
    // Time formatting
    std::cout << "=== Time Formatting ===" << std::endl;
    std::cout << "Default format: " << baba::core::time_utils::format_now() << std::endl;
    std::cout << "ISO format: " << baba::core::time_utils::format_now("%Y-%m-%dT%H:%M:%S") << std::endl;
    std::cout << "US format: " << baba::core::time_utils::format_now("%m/%d/%Y %I:%M:%S %p") << std::endl;
    std::cout << "Date only: " << baba::core::time_utils::format_now("%A, %B %d, %Y") << std::endl;
    std::cout << "Time only: " << baba::core::time_utils::format_now("%H:%M:%S") << std::endl;
    std::cout << std::endl;
    
    // Date parsing
    std::cout << "=== Date Parsing ===" << std::endl;
    std::string iso_date = "2024-01-15 14:30:45";
    auto parsed_time = baba::core::time_utils::parse_iso_date(iso_date);
    std::cout << "Parsed '" << iso_date << "': " << baba::core::time_utils::format_time(parsed_time) << std::endl;
    
    // Test with different formats (note: parse_iso_date expects specific format)
    std::string iso_date2 = "2023-12-25 09:15:30";
    auto parsed_time2 = baba::core::time_utils::parse_iso_date(iso_date2);
    std::cout << "Parsed '" << iso_date2 << "': " << baba::core::time_utils::format_time(parsed_time2) << std::endl;
    std::cout << std::endl;
    
    // Time differences
    std::cout << "=== Time Differences ===" << std::endl;
    auto start_time = baba::core::time_utils::now();
    
    // Simulate some work
    baba::core::time_utils::sleep_millis(150);
    
    auto end_time = baba::core::time_utils::now();
    auto duration = baba::core::time_utils::time_diff(start_time, end_time);
    
    std::cout << "Start time: " << baba::core::time_utils::format_time(start_time) << std::endl;
    std::cout << "End time: " << baba::core::time_utils::format_time(end_time) << std::endl;
    std::cout << "Duration (ms): " << baba::core::time_utils::to_millis(duration) << std::endl;
    std::cout << "Duration (seconds): " << baba::core::time_utils::to_seconds(duration) << std::endl;
    std::cout << std::endl;
    
    // Duration conversions
    std::cout << "=== Duration Conversions ===" << std::endl;
    auto long_duration = baba::core::time_utils::time_diff(parsed_time, current_time);
    std::cout << "Time since " << iso_date << ":" << std::endl;
    std::cout << "  Milliseconds: " << baba::core::time_utils::to_millis(long_duration) << std::endl;
    std::cout << "  Seconds: " << baba::core::time_utils::to_seconds(long_duration) << std::endl;
    std::cout << "  Minutes: " << baba::core::time_utils::to_minutes(long_duration) << std::endl;
    std::cout << "  Hours: " << baba::core::time_utils::to_hours(long_duration) << std::endl;
    std::cout << std::endl;
    
    // Sleep functions
    std::cout << "=== Sleep Functions ===" << std::endl;
    std::cout << "Sleeping for 100ms..." << std::endl;
    auto sleep_start = baba::core::time_utils::now();
    baba::core::time_utils::sleep_millis(100);
    auto sleep_end = baba::core::time_utils::now();
    auto sleep_duration = baba::core::time_utils::time_diff(sleep_start, sleep_end);
    std::cout << "Actual sleep time: " << baba::core::time_utils::to_millis(sleep_duration) << "ms" << std::endl;
    
    std::cout << "Sleeping for 0.05 seconds..." << std::endl;
    sleep_start = baba::core::time_utils::now();
    baba::core::time_utils::sleep_seconds(0.05);
    sleep_end = baba::core::time_utils::now();
    sleep_duration = baba::core::time_utils::time_diff(sleep_start, sleep_end);
    std::cout << "Actual sleep time: " << baba::core::time_utils::to_seconds(sleep_duration) << "s" << std::endl;
    std::cout << std::endl;
    
    // Timer class usage
    std::cout << "=== Timer Class Usage ===" << std::endl;
    {
        baba::core::time_utils::Timer timer;
        std::cout << "Timer started..." << std::endl;
        
        // Simulate some work
        baba::core::time_utils::sleep_millis(75);
        std::cout << "After 75ms sleep - Elapsed: " << timer.elapsed_millis() << "ms" << std::endl;
        
        baba::core::time_utils::sleep_millis(25);
        std::cout << "After another 25ms sleep - Elapsed: " << timer.elapsed_millis() << "ms" << std::endl;
        std::cout << "Elapsed seconds: " << timer.elapsed_seconds() << "s" << std::endl;
        
        // Reset timer
        timer.reset();
        std::cout << "Timer reset - Elapsed: " << timer.elapsed_millis() << "ms" << std::endl;
        
        baba::core::time_utils::sleep_millis(50);
        std::cout << "After reset + 50ms - Elapsed: " << timer.elapsed_millis() << "ms" << std::endl;
    }
    std::cout << std::endl;
    
    // Scoped timer usage
    std::cout << "=== Scoped Timer Usage ===" << std::endl;
    {
        auto scoped_timer = baba::core::time_utils::make_scoped_timer("Database Operation");
        std::cout << "Performing database operation..." << std::endl;
        baba::core::time_utils::sleep_millis(120);
        std::cout << "Database operation completed" << std::endl;
        // Timer will automatically log when it goes out of scope
    }
    std::cout << std::endl;
    
    {
        auto scoped_timer = baba::core::time_utils::make_scoped_timer("File Processing");
        std::cout << "Processing files..." << std::endl;
        baba::core::time_utils::sleep_millis(80);
        std::cout << "File processing completed" << std::endl;
        // Timer will automatically log when it goes out of scope
    }
    std::cout << std::endl;
    
    // Macro usage (if available)
    std::cout << "=== Macro Usage Example ===" << std::endl;
    {
        BABA_TIME_SCOPE("Complex Calculation");
        std::cout << "Performing complex calculation..." << std::endl;
        
        // Simulate complex work
        for (int i = 0; i < 3; ++i) {
            baba::core::time_utils::sleep_millis(30);
            std::cout << "  Step " << (i + 1) << " completed" << std::endl;
        }
        std::cout << "Complex calculation finished" << std::endl;
        // Scoped timer will log automatically
    }
    std::cout << std::endl;
    
    // Practical example: Benchmarking operations
    std::cout << "=== Practical Example: Benchmarking Operations ===" << std::endl;
    
    // Benchmark string operations
    {
        auto timer = baba::core::time_utils::make_scoped_timer("String Operations Benchmark");
        std::string result;
        for (int i = 0; i < 10000; ++i) {
            result += "test" + std::to_string(i) + " ";
        }
        std::cout << "Concatenated " << 10000 << " strings (result length: " << result.length() << ")" << std::endl;
    }
    
    // Benchmark with manual timing
    std::cout << "\nManual timing example:" << std::endl;
    baba::core::time_utils::Timer manual_timer;
    
    // Simulate different operations
    std::vector<int> numbers;
    for (int i = 0; i < 100000; ++i) {
        numbers.push_back(i * 2);
    }
    auto operation1_time = manual_timer.elapsed_millis();
    std::cout << "Vector population: " << operation1_time << "ms" << std::endl;
    
    // Sort the vector
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    auto operation2_time = manual_timer.elapsed_millis() - operation1_time;
    std::cout << "Vector sorting: " << operation2_time << "ms" << std::endl;
    
    std::cout << "Total time: " << manual_timer.elapsed_millis() << "ms" << std::endl;
    std::cout << std::endl;
    
    // Performance comparison
    std::cout << "=== Performance Comparison ===" << std::endl;
    const int iterations = 1000;
    
    // Method 1: Using Timer class
    baba::core::time_utils::Timer perf_timer;
    for (int i = 0; i < iterations; ++i) {
        volatile int dummy = i * i; // Prevent optimization
        (void)dummy; // Suppress unused variable warning
    }
    auto method1_time = perf_timer.elapsed_millis();
    
    // Method 2: Using now() and time_diff
    auto method2_start = baba::core::time_utils::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int dummy = i * i; // Prevent optimization
        (void)dummy; // Suppress unused variable warning
    }
    auto method2_end = baba::core::time_utils::now();
    auto method2_time = baba::core::time_utils::to_millis(baba::core::time_utils::time_diff(method2_start, method2_end));
    
    std::cout << "Performance test (" << iterations << " iterations):" << std::endl;
    std::cout << "Method 1 (Timer class): " << method1_time << "ms" << std::endl;
    std::cout << "Method 2 (now/time_diff): " << method2_time << "ms" << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Time Utils Example Complete ===" << std::endl;
    return 0;
}
