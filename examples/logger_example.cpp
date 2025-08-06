#include <baba/logger.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

int main() {
    std::cout << "=== Logger Advanced Example ===" << std::endl << std::endl;
    
    // Basic logging setup
    std::cout << "=== Basic Logging Setup ===" << std::endl;
    baba::core::Logger::set_global_level(baba::core::LogLevel::DEBUG);
    baba::core::Logger::enable_console(true);
    
    std::cout << "Logger configured with DEBUG level and console output enabled" << std::endl;
    std::cout << std::endl;
    
    // Basic logging methods
    std::cout << "=== Basic Logging Methods ===" << std::endl;
    baba::core::Logger::debug("This is a debug message - useful for development");
    baba::core::Logger::info("Application started successfully");
    baba::core::Logger::warn("This is a warning - something might need attention");
    baba::core::Logger::error("This is an error message - something went wrong");
    std::cout << std::endl;
    
    // Log level filtering
    std::cout << "=== Log Level Filtering ===" << std::endl;
    std::cout << "Setting log level to INFO (DEBUG messages will be filtered out)" << std::endl;
    baba::core::Logger::set_global_level(baba::core::LogLevel::INFO);
    
    baba::core::Logger::debug("This DEBUG message should NOT appear");
    baba::core::Logger::info("This INFO message should appear");
    baba::core::Logger::warn("This WARN message should appear");
    baba::core::Logger::error("This ERROR message should appear");
    
    std::cout << "\nSetting log level to ERROR (only ERROR messages will appear)" << std::endl;
    baba::core::Logger::set_global_level(baba::core::LogLevel::ERROR);
    
    baba::core::Logger::debug("This DEBUG message should NOT appear");
    baba::core::Logger::info("This INFO message should NOT appear");
    baba::core::Logger::warn("This WARN message should NOT appear");
    baba::core::Logger::error("This ERROR message should appear");
    
    // Reset to DEBUG for remaining examples
    baba::core::Logger::set_global_level(baba::core::LogLevel::DEBUG);
    std::cout << std::endl;
    
    // File logging
    std::cout << "=== File Logging ===" << std::endl;
    const std::string log_file = "application.log";
    if (baba::core::Logger::set_file(log_file)) {
        std::cout << "✓ File logging enabled: " << log_file << std::endl;
        
        baba::core::Logger::info("This message goes to both console and file");
        baba::core::Logger::warn("File logging is now active");
        baba::core::Logger::debug("Debug information saved to file");
        
        std::cout << "Messages have been written to " << log_file << std::endl;
    } else {
        std::cout << "✗ Failed to enable file logging" << std::endl;
    }
    std::cout << std::endl;
    
    // Instance-based logging
    std::cout << "=== Instance-based Logging ===" << std::endl;
    auto& logger = baba::core::Logger::instance();
    
    std::cout << "Current log level: " << static_cast<int>(logger.get_level()) << std::endl;
    
    logger.info("Using logger instance directly");
    logger.set_level(baba::core::LogLevel::WARN);
    logger.debug("This debug message should be filtered out");
    logger.warn("This warning should appear");
    
    // Reset level
    logger.set_level(baba::core::LogLevel::DEBUG);
    std::cout << std::endl;
    
    // Formatted logging
    std::cout << "=== Formatted Logging ===" << std::endl;
    std::string username = "Alice";
    int user_id = 12345;
    double balance = 1234.56;
    
    logger.info_format("User login: %s (ID: %d)", username.c_str(), user_id);
    logger.warn_format("Low balance warning for user %s: $%.2f", username.c_str(), balance);
    logger.error_format("Transaction failed for user %d: insufficient funds (balance: $%.2f)", user_id, balance);
    logger.debug_format("Debug info - User: %s, ID: %d, Balance: $%.2f", username.c_str(), user_id, balance);
    
    // More complex formatting
    int processed_items = 1250;
    double processing_time = 3.456;
    logger.info_format("Batch processing completed: %d items processed in %.3f seconds (%.1f items/sec)", 
                      processed_items, processing_time, processed_items / processing_time);
    std::cout << std::endl;
    
    // Practical example: Application lifecycle logging
    std::cout << "=== Practical Example: Application Lifecycle ===" << std::endl;
    
    // Simulate application startup
    baba::core::Logger::info("=== Application Startup ===");
    baba::core::Logger::debug("Loading configuration files...");
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    baba::core::Logger::info("Configuration loaded successfully");
    
    baba::core::Logger::debug("Initializing database connection...");
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    baba::core::Logger::info("Database connection established");
    
    baba::core::Logger::debug("Starting background services...");
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    baba::core::Logger::info("Background services started");
    
    baba::core::Logger::info("Application startup completed successfully");
    
    // Simulate some application work
    baba::core::Logger::info("=== Application Runtime ===");
    for (int i = 1; i <= 5; ++i) {
        logger.info_format("Processing request #%d", i);
        
        if (i == 3) {
            logger.warn_format("Request #%d took longer than expected", i);
        }
        
        if (i == 4) {
            logger.error_format("Request #%d failed - retrying...", i);
            logger.info_format("Request #%d retry successful", i);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    
    // Simulate application shutdown
    baba::core::Logger::info("=== Application Shutdown ===");
    baba::core::Logger::debug("Stopping background services...");
    baba::core::Logger::info("Background services stopped");
    
    baba::core::Logger::debug("Closing database connections...");
    baba::core::Logger::info("Database connections closed");
    
    baba::core::Logger::info("Application shutdown completed");
    std::cout << std::endl;
    
    // Thread safety demonstration
    std::cout << "=== Thread Safety Demonstration ===" << std::endl;
    std::cout << "Starting multiple threads that log simultaneously..." << std::endl;
    
    std::vector<std::thread> threads;
    
    // Create multiple threads that log messages
    for (int thread_id = 1; thread_id <= 3; ++thread_id) {
        threads.push_back(std::thread([thread_id]() {
            for (int msg = 1; msg <= 5; ++msg) {
                baba::core::Logger::instance().info_format("Thread %d - Message %d", thread_id, msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    baba::core::Logger::info("All threads completed - logging is thread-safe");
    std::cout << std::endl;
    
    // Different logging scenarios
    std::cout << "=== Different Logging Scenarios ===" << std::endl;
    
    // Error handling scenario
    baba::core::Logger::info("=== Error Handling Scenario ===");
    try {
        baba::core::Logger::debug("Attempting risky operation...");
        // Simulate an error condition
        bool operation_failed = true;
        if (operation_failed) {
            throw std::runtime_error("Simulated operation failure");
        }
    } catch (const std::exception& e) {
        logger.error_format("Operation failed with exception: %s", e.what());
        baba::core::Logger::info("Initiating error recovery procedures");
        baba::core::Logger::info("Error recovery completed successfully");
    }
    
    // Performance monitoring scenario
    baba::core::Logger::info("=== Performance Monitoring Scenario ===");
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    logger.info_format("Operation completed in %lld ms", duration.count());
    
    if (duration.count() > 50) {
        logger.warn_format("Operation took longer than expected: %lld ms (threshold: 50ms)", duration.count());
    }
    
    // Flush all logs
    std::cout << "\n=== Flushing Logs ===" << std::endl;
    baba::core::Logger::info("Flushing all log outputs...");
    logger.flush();
    std::cout << "All logs have been flushed to their destinations" << std::endl;
    std::cout << std::endl;
    
    // Console output control
    std::cout << "=== Console Output Control ===" << std::endl;
    std::cout << "Disabling console output (messages will only go to file)..." << std::endl;
    baba::core::Logger::enable_console(false);
    
    baba::core::Logger::info("This message should only appear in the log file");
    baba::core::Logger::warn("Console output is disabled");
    
    std::cout << "Re-enabling console output..." << std::endl;
    baba::core::Logger::enable_console(true);
    baba::core::Logger::info("Console output is now re-enabled");
    std::cout << std::endl;
    
    // Final summary
    baba::core::Logger::info("=== Logger Example Summary ===");
    baba::core::Logger::info("Demonstrated all major logging features:");
    baba::core::Logger::info("- Basic logging methods (debug, info, warn, error)");
    baba::core::Logger::info("- Log level filtering");
    baba::core::Logger::info("- File and console output");
    baba::core::Logger::info("- Formatted logging");
    baba::core::Logger::info("- Thread safety");
    baba::core::Logger::info("- Instance and static method usage");
    
    std::cout << std::endl << "=== Logger Example Complete ===" << std::endl;
    std::cout << "Check the '" << log_file << "' file for logged messages." << std::endl;
    
    return 0;
}
