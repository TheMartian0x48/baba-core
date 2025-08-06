#include <baba/core.hpp>
#include <iostream>

int main() {
    std::cout << "=== Baba Core Library Demo ===" << std::endl;
    std::cout << "Library Version: " << baba::core::get_version() << std::endl << std::endl;
    
    // String utilities demo
    std::cout << "=== String Utilities ===" << std::endl;
    std::string text = "  Hello, World!  ";
    std::cout << "Original: '" << text << "'" << std::endl;
    std::cout << "Trimmed: '" << baba::core::string_utils::trim(text) << "'" << std::endl;
    
    auto parts = baba::core::string_utils::split("apple,banana,cherry", ',');
    std::cout << "Split 'apple,banana,cherry': ";
    for (const auto& part : parts) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl;
    
    std::cout << "Uppercase 'hello': " << baba::core::string_utils::to_upper("hello") << std::endl;
    std::cout << std::endl;
    
    // File utilities demo
    std::cout << "=== File Utilities ===" << std::endl;
    const std::string filename = "demo_file.txt";
    const std::string content = "Hello from Baba Core Library!";
    
    if (baba::core::file_utils::write_file(filename, content)) {
        std::cout << "Successfully wrote to " << filename << std::endl;
        
        if (baba::core::file_utils::file_exists(filename)) {
            std::string read_content = baba::core::file_utils::read_file(filename);
            std::cout << "File content: " << read_content << std::endl;
            
            // Clean up
            baba::core::file_utils::remove_file(filename);
            std::cout << "Cleaned up demo file" << std::endl;
        }
    }
    std::cout << std::endl;
    
    // Logging demo
    std::cout << "=== Logging Demo ===" << std::endl;
    baba::core::Logger::set_global_level(baba::core::LogLevel::DEBUG);
    baba::core::Logger::enable_console(true);
    
    baba::core::Logger::debug("This is a debug message");
    baba::core::Logger::info("Application started successfully");
    baba::core::Logger::warn("This is a warning message");
    baba::core::Logger::error("This is an error message");
    std::cout << std::endl;
    
    // Time utilities demo
    std::cout << "=== Time Utilities ===" << std::endl;
    auto now = baba::core::time_utils::now();
    std::cout << "Current time: " << baba::core::time_utils::format_time(now) << std::endl;
    std::cout << "Timestamp (ms): " << baba::core::time_utils::now_millis() << std::endl;
    
    // Timer demo
    {
        auto timer = baba::core::time_utils::Timer();
        baba::core::time_utils::sleep_millis(100); // Sleep for 100ms
        std::cout << "Timer elapsed: " << timer.elapsed_millis() << " ms" << std::endl;
    }
    
    // Scoped timer demo
    {
        auto scoped_timer = baba::core::time_utils::make_scoped_timer("Demo Operation");
        baba::core::time_utils::sleep_millis(50); // Sleep for 50ms
        // Timer will log elapsed time when it goes out of scope
    }
    
    std::cout << std::endl << "=== Demo Complete ===" << std::endl;
    return 0;
}
