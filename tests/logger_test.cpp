#include <gtest/gtest.h>
#include <baba/logger.hpp>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace baba::core;

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset logger to default state
        Logger::instance().set_level(LogLevel::INFO);
        Logger::instance().set_console_output(true);
        Logger::instance().set_file_output(""); // Disable file output
        
        // Create test directory for file logging tests
        test_dir = "test_logger";
        std::filesystem::create_directory(test_dir);
        test_log_file = test_dir + "/test.log";
    }
    
    void TearDown() override {
        // Clean up test files
        std::filesystem::remove_all(test_dir);
        
        // Reset logger state
        Logger::instance().set_file_output("");
        Logger::instance().set_console_output(true);
        Logger::instance().set_level(LogLevel::INFO);
    }
    
    std::string test_dir;
    std::string test_log_file;
};

// LogLevel enum tests
TEST_F(LoggerTest, LogLevelToString) {
    EXPECT_EQ(to_string(LogLevel::DEBUG), "DEBUG");
    EXPECT_EQ(to_string(LogLevel::INFO), "INFO");
    EXPECT_EQ(to_string(LogLevel::WARN), "WARN");
    EXPECT_EQ(to_string(LogLevel::ERROR), "ERROR");
}

// Logger singleton tests
TEST_F(LoggerTest, SingletonInstance) {
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();
    
    // Should be the same instance
    EXPECT_EQ(&logger1, &logger2);
}

// Log level tests
TEST_F(LoggerTest, SetAndGetLogLevel) {
    Logger& logger = Logger::instance();
    
    logger.set_level(LogLevel::DEBUG);
    EXPECT_EQ(logger.get_level(), LogLevel::DEBUG);
    
    logger.set_level(LogLevel::WARN);
    EXPECT_EQ(logger.get_level(), LogLevel::WARN);
    
    logger.set_level(LogLevel::ERROR);
    EXPECT_EQ(logger.get_level(), LogLevel::ERROR);
}

// Console output tests
TEST_F(LoggerTest, SetConsoleOutput) {
    Logger& logger = Logger::instance();
    
    logger.set_console_output(false);
    // Note: We can't easily test console output in unit tests
    // This test mainly ensures the method doesn't crash
    
    logger.set_console_output(true);
    // Restore default state
}

// File output tests
TEST_F(LoggerTest, SetFileOutputSuccess) {
    Logger& logger = Logger::instance();
    
    bool result = logger.set_file_output(test_log_file);
    EXPECT_TRUE(result);
    
    // Log something to create the file
    logger.info("Test message");
    logger.flush();
    
    // Check if file was created
    EXPECT_TRUE(std::filesystem::exists(test_log_file));
}

TEST_F(LoggerTest, SetFileOutputInvalidPath) {
    Logger& logger = Logger::instance();
    
    bool result = logger.set_file_output("/invalid/path/test.log");
    EXPECT_FALSE(result);
}

TEST_F(LoggerTest, DisableFileOutput) {
    Logger& logger = Logger::instance();
    
    // Enable file output first
    logger.set_file_output(test_log_file);
    
    // Disable file output
    bool result = logger.set_file_output("");
    EXPECT_TRUE(result);
}

// Logging methods tests
TEST_F(LoggerTest, LoggingMethods) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    logger.set_level(LogLevel::DEBUG);
    
    // Test all logging methods
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warn("Warning message");
    logger.error("Error message");
    
    logger.flush();
    
    // Read the log file and check content
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("DEBUG") != std::string::npos);
    EXPECT_TRUE(content.find("INFO") != std::string::npos);
    EXPECT_TRUE(content.find("WARN") != std::string::npos);
    EXPECT_TRUE(content.find("ERROR") != std::string::npos);
    
    EXPECT_TRUE(content.find("Debug message") != std::string::npos);
    EXPECT_TRUE(content.find("Info message") != std::string::npos);
    EXPECT_TRUE(content.find("Warning message") != std::string::npos);
    EXPECT_TRUE(content.find("Error message") != std::string::npos);
}

// Log level filtering tests
TEST_F(LoggerTest, LogLevelFiltering) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    logger.set_level(LogLevel::WARN); // Only WARN and ERROR should be logged
    
    logger.debug("Debug message - should not appear");
    logger.info("Info message - should not appear");
    logger.warn("Warning message - should appear");
    logger.error("Error message - should appear");
    
    logger.flush();
    
    // Read the log file
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Should not contain DEBUG or INFO messages
    EXPECT_TRUE(content.find("Debug message") == std::string::npos);
    EXPECT_TRUE(content.find("Info message") == std::string::npos);
    
    // Should contain WARN and ERROR messages
    EXPECT_TRUE(content.find("Warning message") != std::string::npos);
    EXPECT_TRUE(content.find("Error message") != std::string::npos);
}

// Generic log method tests
TEST_F(LoggerTest, GenericLogMethod) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    logger.set_level(LogLevel::DEBUG);
    
    logger.log(LogLevel::DEBUG, "Generic debug message");
    logger.log(LogLevel::INFO, "Generic info message");
    logger.log(LogLevel::WARN, "Generic warn message");
    logger.log(LogLevel::ERROR, "Generic error message");
    
    logger.flush();
    
    // Read the log file
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Generic debug message") != std::string::npos);
    EXPECT_TRUE(content.find("Generic info message") != std::string::npos);
    EXPECT_TRUE(content.find("Generic warn message") != std::string::npos);
    EXPECT_TRUE(content.find("Generic error message") != std::string::npos);
}

// Message formatting tests
TEST_F(LoggerTest, MessageFormatting) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    logger.set_level(LogLevel::INFO);
    
    logger.info("Test message");
    logger.flush();
    
    // Read the log file
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Check that the message contains timestamp, log level, and message
    EXPECT_TRUE(content.find("[") != std::string::npos); // Timestamp brackets
    EXPECT_TRUE(content.find("INFO") != std::string::npos); // Log level
    EXPECT_TRUE(content.find("Test message") != std::string::npos); // Message
    
    // Check timestamp format (basic check for date-time pattern)
    EXPECT_TRUE(content.find("-") != std::string::npos); // Date separators
    EXPECT_TRUE(content.find(":") != std::string::npos); // Time separators
    EXPECT_TRUE(content.find(".") != std::string::npos); // Milliseconds separator
}

// Flush method tests
TEST_F(LoggerTest, FlushMethod) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    
    logger.info("Message before flush");
    
    // File might not contain the message yet
    logger.flush();
    
    // After flush, file should definitely contain the message
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Message before flush") != std::string::npos);
}

// Multiple messages test
TEST_F(LoggerTest, MultipleMessages) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    logger.set_level(LogLevel::DEBUG);
    
    // Log multiple messages
    for (int i = 0; i < 10; ++i) {
        logger.info("Message " + std::to_string(i));
    }
    
    logger.flush();
    
    // Read the log file
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Check that all messages are present
    for (int i = 0; i < 10; ++i) {
        std::string expected = "Message " + std::to_string(i);
        EXPECT_TRUE(content.find(expected) != std::string::npos);
    }
}

// Empty message test
TEST_F(LoggerTest, EmptyMessage) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    
    logger.info("");
    logger.flush();
    
    // Should not crash and should create a log entry
    EXPECT_TRUE(std::filesystem::exists(test_log_file));
    
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Should contain the log level even with empty message
    EXPECT_TRUE(content.find("INFO") != std::string::npos);
}

// Long message test
TEST_F(LoggerTest, LongMessage) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    
    std::string long_message(1000, 'A'); // 1000 characters
    logger.info(long_message);
    logger.flush();
    
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find(long_message) != std::string::npos);
}

// Special characters test
TEST_F(LoggerTest, SpecialCharacters) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    
    std::string special_message = "Message with special chars: !@#$%^&*()[]{}|\\:;\"'<>,.?/~`";
    logger.info(special_message);
    logger.flush();
    
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find(special_message) != std::string::npos);
}

// Newline handling test
TEST_F(LoggerTest, NewlineHandling) {
    Logger& logger = Logger::instance();
    logger.set_file_output(test_log_file);
    
    std::string message_with_newlines = "Line 1\nLine 2\nLine 3";
    logger.info(message_with_newlines);
    logger.flush();
    
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Line 1") != std::string::npos);
    EXPECT_TRUE(content.find("Line 2") != std::string::npos);
    EXPECT_TRUE(content.find("Line 3") != std::string::npos);
}

// File append test
TEST_F(LoggerTest, FileAppendMode) {
    Logger& logger = Logger::instance();
    
    // First logging session
    logger.set_file_output(test_log_file);
    logger.info("First message");
    logger.flush();
    logger.set_file_output(""); // Close file
    
    // Second logging session (should append)
    logger.set_file_output(test_log_file);
    logger.info("Second message");
    logger.flush();
    
    std::ifstream file(test_log_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Both messages should be present
    EXPECT_TRUE(content.find("First message") != std::string::npos);
    EXPECT_TRUE(content.find("Second message") != std::string::npos);
}
