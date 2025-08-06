#include <baba/core.hpp>
#include <iostream>
#include <vector>
#include <map>

int main() {
    std::cout << "=== Integration Example: Configuration File Processor ===" << std::endl << std::endl;
    
    // Setup logging
    baba::core::Logger::set_global_level(baba::core::LogLevel::DEBUG);
    baba::core::Logger::enable_console(true);
    baba::core::Logger::set_file("config_processor.log");
    
    baba::core::Logger::info("=== Configuration File Processor Started ===");
    baba::core::Logger::instance().info_format("Using Baba Core Library v%s", baba::core::get_version());
    
    // Create a sample configuration file
    std::cout << "=== Creating Sample Configuration File ===" << std::endl;
    const std::string config_file = "app_config.ini";
    
    std::vector<std::string> config_lines = {
        "# Application Configuration File",
        "# Generated on " + baba::core::time_utils::format_now(),
        "",
        "[database]",
        "host = localhost",
        "port = 5432",
        "username = app_user",
        "password = secret123",
        "database = myapp_db",
        "",
        "[server]",
        "listen_port = 8080",
        "max_connections = 100",
        "timeout = 30",
        "",
        "[logging]",
        "level = INFO",
        "file = /var/log/myapp.log",
        "rotate = true",
        "",
        "[features]",
        "enable_cache = true",
        "enable_metrics = false",
        "debug_mode = false"
    };
    
    auto timer = baba::core::time_utils::make_scoped_timer("Configuration File Creation");
    if (baba::core::file_utils::write_lines(config_file, config_lines)) {
        baba::core::Logger::instance().info_format("Created configuration file: %s", config_file.c_str());
        std::cout << "✓ Configuration file created successfully" << std::endl;
    } else {
        baba::core::Logger::error("Failed to create configuration file");
        return 1;
    }
    std::cout << std::endl;
    
    // Read and parse the configuration file
    std::cout << "=== Reading and Parsing Configuration ===" << std::endl;
    baba::core::Logger::info("Starting configuration parsing");
    
    if (!baba::core::file_utils::file_exists(config_file)) {
        baba::core::Logger::instance().error_format("Configuration file not found: %s", config_file.c_str());
        return 1;
    }
    
    auto lines = baba::core::file_utils::read_lines(config_file);
    baba::core::Logger::instance().info_format("Read %zu lines from configuration file", lines.size());
    
    // Parse configuration into sections and key-value pairs
    std::map<std::string, std::map<std::string, std::string>> config;
    std::string current_section;
    int line_number = 0;
    
    baba::core::time_utils::Timer parse_timer;
    
    for (const auto& line : lines) {
        line_number++;
        std::string trimmed_line = baba::core::string_utils::trim(line);
        
        // Skip empty lines and comments
        if (trimmed_line.empty() || baba::core::string_utils::starts_with(trimmed_line, "#")) {
            continue;
        }
        
        // Check for section headers [section_name]
        if (baba::core::string_utils::starts_with(trimmed_line, "[") && 
            baba::core::string_utils::ends_with(trimmed_line, "]")) {
            current_section = trimmed_line.substr(1, trimmed_line.length() - 2);
            current_section = baba::core::string_utils::trim(current_section);
            baba::core::Logger::instance().debug_format("Found section: [%s]", current_section.c_str());
            continue;
        }
        
        // Parse key-value pairs
        if (baba::core::string_utils::contains(trimmed_line, "=")) {
            auto parts = baba::core::string_utils::split(trimmed_line, '=');
            if (parts.size() >= 2) {
                std::string key = baba::core::string_utils::trim(parts[0]);
                std::string value = baba::core::string_utils::trim(parts[1]);
                
                if (current_section.empty()) {
                    baba::core::Logger::instance().warn_format("Key-value pair outside section at line %d: %s", 
                                                   line_number, trimmed_line.c_str());
                    continue;
                }
                
                config[current_section][key] = value;
                baba::core::Logger::instance().debug_format("Parsed: [%s] %s = %s", 
                                                current_section.c_str(), key.c_str(), value.c_str());
            }
        } else {
            baba::core::Logger::instance().warn_format("Invalid line format at line %d: %s", 
                                           line_number, trimmed_line.c_str());
        }
    }
    
    auto parse_time = parse_timer.elapsed_millis();
    baba::core::Logger::instance().info_format("Configuration parsing completed in %lld ms", parse_time);
    std::cout << "✓ Configuration parsed successfully" << std::endl;
    std::cout << std::endl;
    
    // Display parsed configuration
    std::cout << "=== Parsed Configuration ===" << std::endl;
    for (const auto& section : config) {
        std::cout << "[" << section.first << "]" << std::endl;
        for (const auto& kv : section.second) {
            std::cout << "  " << kv.first << " = " << kv.second << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Validate configuration
    std::cout << "=== Configuration Validation ===" << std::endl;
    baba::core::Logger::info("Starting configuration validation");
    
    bool validation_passed = true;
    
    // Validate database section
    if (config.find("database") != config.end()) {
        auto& db_config = config["database"];
        
        // Check required fields
        std::vector<std::string> required_db_fields = {"host", "port", "username", "database"};
        for (const auto& field : required_db_fields) {
            if (db_config.find(field) == db_config.end()) {
                baba::core::Logger::instance().error_format("Missing required database field: %s", field.c_str());
                validation_passed = false;
            }
        }
        
        // Validate port number
        if (db_config.find("port") != db_config.end()) {
            std::string port_str = db_config["port"];
            try {
                int port = std::stoi(port_str);
                if (port < 1 || port > 65535) {
                    baba::core::Logger::instance().error_format("Invalid port number: %d", port);
                    validation_passed = false;
                } else {
                    baba::core::Logger::instance().debug_format("Database port validation passed: %d", port);
                }
            } catch (const std::exception& e) {
                baba::core::Logger::instance().error_format("Invalid port format: %s", port_str.c_str());
                validation_passed = false;
            }
        }
    } else {
        baba::core::Logger::error("Missing database configuration section");
        validation_passed = false;
    }
    
    // Validate server section
    if (config.find("server") != config.end()) {
        auto& server_config = config["server"];
        
        if (server_config.find("listen_port") != server_config.end()) {
            try {
                int port = std::stoi(server_config["listen_port"]);
                if (port < 1024 || port > 65535) {
                    baba::core::Logger::instance().warn_format("Server port %d might require elevated privileges or be invalid", port);
                }
            } catch (const std::exception& e) {
                baba::core::Logger::instance().error_format("Invalid server port format: %s", 
                                                server_config["listen_port"].c_str());
                validation_passed = false;
            }
        }
    }
    
    if (validation_passed) {
        baba::core::Logger::info("Configuration validation passed");
        std::cout << "✓ Configuration validation passed" << std::endl;
    } else {
        baba::core::Logger::error("Configuration validation failed");
        std::cout << "✗ Configuration validation failed" << std::endl;
    }
    std::cout << std::endl;
    
    // Generate processed configuration report
    std::cout << "=== Generating Configuration Report ===" << std::endl;
    const std::string report_file = "config_report.txt";
    
    std::vector<std::string> report_lines;
    report_lines.push_back("Configuration Processing Report");
    report_lines.push_back("Generated: " + baba::core::time_utils::format_now());
    report_lines.push_back("Source File: " + config_file);
    report_lines.push_back("Library Version: " + std::string(baba::core::get_version()));
    report_lines.push_back("");
    
    // Summary statistics
    report_lines.push_back("=== Summary ===");
    report_lines.push_back(baba::core::string_utils::format("Total sections: %zu", config.size()));
    
    size_t total_keys = 0;
    for (const auto& section : config) {
        total_keys += section.second.size();
    }
    report_lines.push_back(baba::core::string_utils::format("Total configuration keys: %zu", total_keys));
    report_lines.push_back(baba::core::string_utils::format("Parsing time: %lld ms", parse_time));
    report_lines.push_back(baba::core::string_utils::format("Validation status: %s", 
                                                           validation_passed ? "PASSED" : "FAILED"));
    report_lines.push_back("");
    
    // Detailed configuration
    report_lines.push_back("=== Detailed Configuration ===");
    for (const auto& section : config) {
        report_lines.push_back(baba::core::string_utils::format("[%s]", section.first.c_str()));
        for (const auto& kv : section.second) {
            // Mask sensitive information
            std::string value = kv.second;
            if (baba::core::string_utils::contains(baba::core::string_utils::to_lower(kv.first), "password") ||
                baba::core::string_utils::contains(baba::core::string_utils::to_lower(kv.first), "secret")) {
                value = "***MASKED***";
            }
            report_lines.push_back(baba::core::string_utils::format("  %s = %s", kv.first.c_str(), value.c_str()));
        }
        report_lines.push_back("");
    }
    
    // Write report
    {
        auto report_timer = baba::core::time_utils::make_scoped_timer("Report Generation");
        if (baba::core::file_utils::write_lines(report_file, report_lines)) {
            baba::core::Logger::instance().info_format("Configuration report written to: %s", report_file.c_str());
            std::cout << "✓ Configuration report generated: " << report_file << std::endl;
        } else {
            baba::core::Logger::error("Failed to write configuration report");
        }
    }
    std::cout << std::endl;
    
    // Backup original configuration with timestamp
    std::cout << "=== Creating Configuration Backup ===" << std::endl;
    std::string timestamp = baba::core::string_utils::replace_all(
        baba::core::time_utils::format_now("%Y%m%d_%H%M%S"), " ", "_");
    std::string backup_file = baba::core::string_utils::format("backup_%s_%s", 
                                                              timestamp.c_str(), 
                                                              baba::core::file_utils::get_filename(config_file).c_str());
    
    if (baba::core::file_utils::copy_file(config_file, backup_file)) {
        baba::core::Logger::instance().info_format("Configuration backed up to: %s", backup_file.c_str());
        std::cout << "✓ Configuration backup created: " << backup_file << std::endl;
    } else {
        baba::core::Logger::error("Failed to create configuration backup");
    }
    std::cout << std::endl;
    
    // Clean up demonstration files
    std::cout << "=== Cleanup ===" << std::endl;
    std::vector<std::string> files_to_remove = {config_file, backup_file};
    
    for (const auto& file : files_to_remove) {
        if (baba::core::file_utils::file_exists(file)) {
            if (baba::core::file_utils::remove_file(file)) {
                baba::core::Logger::instance().debug_format("Removed file: %s", file.c_str());
                std::cout << "✓ Removed: " << file << std::endl;
            }
        }
    }
    
    // Final summary
    baba::core::Logger::info("=== Configuration Processing Summary ===");
    baba::core::Logger::info("Successfully demonstrated integration of all core utilities:");
    baba::core::Logger::info("- File I/O operations (read/write configuration files)");
    baba::core::Logger::info("- String processing (parsing, validation, formatting)");
    baba::core::Logger::info("- Time utilities (timestamps, performance timing)");
    baba::core::Logger::info("- Comprehensive logging throughout the process");
    
    std::cout << std::endl << "=== Integration Example Complete ===" << std::endl;
    std::cout << "Check 'config_processor.log' and '" << report_file << "' for detailed logs and reports." << std::endl;
    
    return 0;
}
