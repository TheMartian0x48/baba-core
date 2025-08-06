#include <gtest/gtest.h>
#include <baba/string_utils.hpp>
#include <baba/file_utils.hpp>
#include <baba/logger.hpp>
#include <baba/time_utils.hpp>
#include <filesystem>

using namespace baba::core;

class CoreIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test directory
        test_dir = "test_integration";
        file_utils::create_directory(test_dir);
        
        // Setup logger for file output
        log_file = test_dir + "/integration.log";
        Logger::instance().set_file_output(log_file);
        Logger::instance().set_level(LogLevel::DEBUG);
    }
    
    void TearDown() override {
        // Clean up
        Logger::instance().set_file_output("");
        file_utils::remove_directory(test_dir);
    }
    
    std::string test_dir;
    std::string log_file;
};

// Test integration between string_utils and file_utils
TEST_F(CoreIntegrationTest, StringUtilsWithFileUtils) {
    using namespace string_utils;
    using namespace file_utils;
    
    // Create test data using string utilities
    std::vector<std::string> lines = {"apple", "banana", "cherry", "date"};
    std::string csv_content = join(lines, ",");
    
    // Write to file using file utilities
    std::string test_file = test_dir + "/fruits.csv";
    bool write_success = write_file(test_file, csv_content);
    EXPECT_TRUE(write_success);
    
    // Read back and parse using string utilities
    std::string read_content = read_file(test_file);
    EXPECT_EQ(read_content, csv_content);
    
    std::vector<std::string> parsed_lines = split(read_content, ',');
    EXPECT_EQ(parsed_lines, lines);
    
    // Transform data using string utilities
    std::vector<std::string> upper_lines;
    for (const auto& line : parsed_lines) {
        upper_lines.push_back(to_upper(line));
    }
    
    // Write transformed data back
    std::string upper_csv = join(upper_lines, ",");
    std::string upper_file = test_dir + "/fruits_upper.csv";
    write_file(upper_file, upper_csv);
    
    // Verify transformation
    std::string upper_content = read_file(upper_file);
    EXPECT_EQ(upper_content, "APPLE,BANANA,CHERRY,DATE");
}

// Test integration between logger and file_utils
TEST_F(CoreIntegrationTest, LoggerWithFileUtils) {
    using namespace file_utils;
    
    // Log various messages
    Logger::instance().info("Starting integration test");
    Logger::instance().debug("Debug information");
    Logger::instance().warn("Warning message");
    Logger::instance().error("Error message");
    Logger::instance().flush();
    
    // Verify log file was created
    EXPECT_TRUE(file_exists(log_file));
    
    // Read log content using file utilities
    std::string log_content = read_file(log_file);
    EXPECT_FALSE(log_content.empty());
    
    // Verify log messages are present
    EXPECT_TRUE(log_content.find("Starting integration test") != std::string::npos);
    EXPECT_TRUE(log_content.find("Debug information") != std::string::npos);
    EXPECT_TRUE(log_content.find("Warning message") != std::string::npos);
    EXPECT_TRUE(log_content.find("Error message") != std::string::npos);
    
    // Verify log levels are present
    EXPECT_TRUE(log_content.find("INFO") != std::string::npos);
    EXPECT_TRUE(log_content.find("DEBUG") != std::string::npos);
    EXPECT_TRUE(log_content.find("WARN") != std::string::npos);
    EXPECT_TRUE(log_content.find("ERROR") != std::string::npos);
    
    // Get file size using file utilities
    std::size_t log_size = get_file_size(log_file);
    EXPECT_GT(log_size, 0);
    EXPECT_EQ(log_size, log_content.length());
}

// Test integration between time_utils and logger
TEST_F(CoreIntegrationTest, TimeUtilsWithLogger) {
    using namespace time_utils;
    
    // Create a scoped timer that will log its duration
    {
        ScopedTimer timer("Integration Test Operation");
        
        // Simulate some work
        Logger::instance().info("Performing timed operation");
        sleep_millis(50);
        Logger::instance().info("Operation completed");
    } // Timer logs here
    
    Logger::instance().flush();
    
    // Read log and verify timer output is present
    std::string log_content = file_utils::read_file(log_file);
    EXPECT_TRUE(log_content.find("Integration Test Operation") != std::string::npos);
    EXPECT_TRUE(log_content.find("elapsed") != std::string::npos);
}

// Test integration between all components
TEST_F(CoreIntegrationTest, AllComponentsIntegration) {
    using namespace string_utils;
    using namespace file_utils;
    using namespace time_utils;
    
    Timer overall_timer;
    Logger::instance().info("Starting comprehensive integration test");
    
    // Phase 1: String processing with timing
    {
        ScopedTimer phase_timer("String Processing Phase");
        
        std::string raw_data = "  Hello,World,Test,Data  ";
        Logger::instance().debug("Raw data: " + raw_data);
        
        // Clean and process string
        std::string cleaned = trim(raw_data);
        std::vector<std::string> parts = split(cleaned, ',');
        
        // Transform each part
        std::vector<std::string> processed_parts;
        for (const auto& part : parts) {
            std::string processed = to_upper(trim(part));
            processed_parts.push_back(processed);
            Logger::instance().debug("Processed part: " + processed);
        }
        
        // Rejoin
        std::string final_data = join(processed_parts, " | ");
        Logger::instance().info("Final processed data: " + final_data);
        
        EXPECT_EQ(final_data, "HELLO | WORLD | TEST | DATA");
    }
    
    // Phase 2: File operations with logging
    {
        ScopedTimer phase_timer("File Operations Phase");
        
        std::string config_file = test_dir + "/config.txt";
        std::vector<std::string> config_lines = {
            "# Configuration File",
            "app_name=BabaCore",
            "version=1.0.0",
            "debug=true"
        };
        
        // Write configuration
        bool write_success = write_lines(config_file, config_lines);
        EXPECT_TRUE(write_success);
        Logger::instance().info("Configuration file written");
        
        // Read and parse configuration
        std::vector<std::string> read_lines_result = read_lines(config_file);
        EXPECT_EQ(read_lines_result.size(), config_lines.size());
        
        // Process configuration using string utilities
        std::map<std::string, std::string> config_map;
        for (const auto& line : read_lines_result) {
            if (!starts_with(trim(line), "#") && contains(line, "=")) {
                std::vector<std::string> kv = split(line, '=');
                if (kv.size() == 2) {
                    config_map[trim(kv[0])] = trim(kv[1]);
                }
            }
        }
        
        // Verify parsed configuration
        EXPECT_EQ(config_map["app_name"], "BabaCore");
        EXPECT_EQ(config_map["version"], "1.0.0");
        EXPECT_EQ(config_map["debug"], "true");
        
        Logger::instance().info("Configuration parsed successfully");
    }
    
    // Phase 3: Data processing pipeline
    {
        ScopedTimer phase_timer("Data Pipeline Phase");
        
        // Create sample data file
        std::string data_file = test_dir + "/sample_data.csv";
        std::vector<std::string> sample_data = {
            "name,age,city",
            "john doe,25,new york",
            "jane smith,30,los angeles",
            "bob johnson,35,chicago"
        };
        
        write_lines(data_file, sample_data);
        Logger::instance().info("Sample data file created");
        
        // Process data
        std::vector<std::string> processed_data;
        std::vector<std::string> lines = read_lines(data_file);
        
        for (size_t i = 0; i < lines.size(); ++i) {
            if (i == 0) {
                // Header - convert to uppercase
                processed_data.push_back(to_upper(lines[i]));
            } else {
                // Data rows - capitalize names
                std::vector<std::string> fields = split(lines[i], ',');
                if (fields.size() >= 3) {
                    // Capitalize name (first field)
                    std::vector<std::string> name_parts = split(trim(fields[0]), ' ');
                    std::vector<std::string> capitalized_parts;
                    for (const auto& part : name_parts) {
                        if (!part.empty()) {
                            std::string capitalized = to_lower(part);
                            capitalized[0] = std::toupper(capitalized[0]);
                            capitalized_parts.push_back(capitalized);
                        }
                    }
                    fields[0] = join(capitalized_parts, " ");
                    
                    processed_data.push_back(join(fields, ","));
                }
            }
        }
        
        // Write processed data
        std::string processed_file = test_dir + "/processed_data.csv";
        write_lines(processed_file, processed_data);
        
        Logger::instance().info("Data processing completed");
        
        // Verify processed data
        std::vector<std::string> final_lines = read_lines(processed_file);
        EXPECT_EQ(final_lines[0], "NAME,AGE,CITY");
        EXPECT_TRUE(starts_with(final_lines[1], "John Doe"));
        EXPECT_TRUE(starts_with(final_lines[2], "Jane Smith"));
        EXPECT_TRUE(starts_with(final_lines[3], "Bob Johnson"));
    }
    
    // Final timing and logging
    std::int64_t total_time = overall_timer.elapsed_millis();
    Logger::instance().info("Integration test completed in " + std::to_string(total_time) + "ms");
    Logger::instance().flush();
    
    // Verify comprehensive log
    std::string final_log = read_file(log_file);
    EXPECT_TRUE(contains(final_log, "Starting comprehensive integration test"));
    EXPECT_TRUE(contains(final_log, "String Processing Phase"));
    EXPECT_TRUE(contains(final_log, "File Operations Phase"));
    EXPECT_TRUE(contains(final_log, "Data Pipeline Phase"));
    EXPECT_TRUE(contains(final_log, "Integration test completed"));
    
    // Verify all components worked together
    EXPECT_GT(total_time, 0);
    EXPECT_TRUE(file_exists(test_dir + "/config.txt"));
    EXPECT_TRUE(file_exists(test_dir + "/sample_data.csv"));
    EXPECT_TRUE(file_exists(test_dir + "/processed_data.csv"));
}

// Test error handling across components
TEST_F(CoreIntegrationTest, ErrorHandlingIntegration) {
    using namespace string_utils;
    using namespace file_utils;
    
    Logger::instance().info("Testing error handling integration");
    
    // Test file operations with invalid paths
    bool invalid_write = write_file("/invalid/path/file.txt", "content");
    EXPECT_FALSE(invalid_write);
    Logger::instance().warn("Expected failure: invalid file write");
    
    // Test reading non-existent file
    std::string non_existent_content = read_file("non_existent_file.txt");
    EXPECT_TRUE(non_existent_content.empty());
    Logger::instance().warn("Expected empty content from non-existent file");
    
    // Test string operations with edge cases
    std::string empty_string = "";
    std::vector<std::string> empty_split = split(empty_string, ',');
    EXPECT_TRUE(empty_split.empty() || (empty_split.size() == 1 && empty_split[0].empty()));
    
    std::string empty_join = join(std::vector<std::string>{}, ",");
    EXPECT_TRUE(empty_join.empty());
    
    Logger::instance().info("Error handling tests completed");
    Logger::instance().flush();
    
    // Verify error messages were logged
    std::string log_content = read_file(log_file);
    EXPECT_TRUE(contains(log_content, "Expected failure"));
    EXPECT_TRUE(contains(log_content, "Expected empty content"));
}

// Test performance integration
TEST_F(CoreIntegrationTest, PerformanceIntegration) {
    using namespace string_utils;
    using namespace file_utils;
    using namespace time_utils;
    
    Timer perf_timer;
    Logger::instance().info("Starting performance integration test");
    
    // Large string processing
    {
        ScopedTimer string_perf("Large String Processing");
        
        std::vector<std::string> large_dataset;
        for (int i = 0; i < 1000; ++i) {
            large_dataset.push_back("item_" + std::to_string(i) + "_data");
        }
        
        std::string joined = join(large_dataset, ",");
        std::vector<std::string> split_back = split(joined, ',');
        
        EXPECT_EQ(split_back.size(), large_dataset.size());
        EXPECT_EQ(split_back, large_dataset);
    }
    
    // Large file operations
    {
        ScopedTimer file_perf("Large File Operations");
        
        std::string large_file = test_dir + "/large_file.txt";
        std::string large_content(10000, 'A'); // 10KB
        
        bool write_success = write_file(large_file, large_content);
        EXPECT_TRUE(write_success);
        
        std::string read_content = read_file(large_file);
        EXPECT_EQ(read_content, large_content);
        
        std::size_t file_size = get_file_size(large_file);
        EXPECT_EQ(file_size, large_content.length());
    }
    
    std::int64_t total_perf_time = perf_timer.elapsed_millis();
    Logger::instance().info("Performance test completed in " + std::to_string(total_perf_time) + "ms");
    Logger::instance().flush();
    
    // Performance should be reasonable (less than 1 second for these operations)
    EXPECT_LT(total_perf_time, 1000);
}
