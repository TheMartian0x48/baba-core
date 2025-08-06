#include <baba/file_utils.hpp>
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== File Utils Advanced Example ===" << std::endl << std::endl;
    
    // Basic file operations
    std::cout << "=== Basic File Operations ===" << std::endl;
    const std::string test_file = "test_file.txt";
    const std::string content = "Hello, World!\nThis is a test file.\nLine 3 content.";
    
    // Write and read file
    if (baba::core::file_utils::write_file(test_file, content)) {
        std::cout << "✓ Successfully wrote to " << test_file << std::endl;
        
        if (baba::core::file_utils::file_exists(test_file)) {
            std::cout << "✓ File exists check passed" << std::endl;
            
            std::string read_content = baba::core::file_utils::read_file(test_file);
            std::cout << "File content:\n" << read_content << std::endl;
        }
    }
    std::cout << std::endl;
    
    // Append to file
    std::cout << "=== Append Operations ===" << std::endl;
    const std::string append_content = "\nAppended line 1\nAppended line 2";
    if (baba::core::file_utils::append_file(test_file, append_content)) {
        std::cout << "✓ Successfully appended to file" << std::endl;
        std::cout << "Updated content:\n" << baba::core::file_utils::read_file(test_file) << std::endl;
    }
    std::cout << std::endl;
    
    // Line-based operations
    std::cout << "=== Line-based Operations ===" << std::endl;
    auto lines = baba::core::file_utils::read_lines(test_file);
    std::cout << "File has " << lines.size() << " lines:" << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "  Line " << (i + 1) << ": '" << lines[i] << "'" << std::endl;
    }
    
    // Write lines to a new file
    const std::string lines_file = "lines_test.txt";
    std::vector<std::string> new_lines = {
        "First line",
        "Second line with numbers: 123",
        "Third line with symbols: !@#$%",
        "Last line"
    };
    
    if (baba::core::file_utils::write_lines(lines_file, new_lines)) {
        std::cout << "✓ Successfully wrote lines to " << lines_file << std::endl;
        std::cout << "Lines file content:\n" << baba::core::file_utils::read_file(lines_file) << std::endl;
    }
    std::cout << std::endl;
    
    // File information
    std::cout << "=== File Information ===" << std::endl;
    std::cout << "File size of " << test_file << ": " << baba::core::file_utils::get_file_size(test_file) << " bytes" << std::endl;
    std::cout << "Extension of " << test_file << ": '" << baba::core::file_utils::get_extension(test_file) << "'" << std::endl;
    std::cout << "Filename of " << test_file << ": '" << baba::core::file_utils::get_filename(test_file) << "'" << std::endl;
    std::cout << "Directory of " << test_file << ": '" << baba::core::file_utils::get_directory(test_file) << "'" << std::endl;
    
    // Test with a path
    const std::string path_example = "/home/user/documents/report.pdf";
    std::cout << "\nPath example: " << path_example << std::endl;
    std::cout << "Extension: '" << baba::core::file_utils::get_extension(path_example) << "'" << std::endl;
    std::cout << "Filename: '" << baba::core::file_utils::get_filename(path_example) << "'" << std::endl;
    std::cout << "Directory: '" << baba::core::file_utils::get_directory(path_example) << "'" << std::endl;
    std::cout << std::endl;
    
    // File operations
    std::cout << "=== File Operations ===" << std::endl;
    const std::string copy_target = "copied_file.txt";
    const std::string move_target = "moved_file.txt";
    
    // Copy file
    if (baba::core::file_utils::copy_file(test_file, copy_target)) {
        std::cout << "✓ Successfully copied " << test_file << " to " << copy_target << std::endl;
        std::cout << "Copy exists: " << (baba::core::file_utils::file_exists(copy_target) ? "true" : "false") << std::endl;
    }
    
    // Move file
    if (baba::core::file_utils::move_file(copy_target, move_target)) {
        std::cout << "✓ Successfully moved " << copy_target << " to " << move_target << std::endl;
        std::cout << "Original copy exists: " << (baba::core::file_utils::file_exists(copy_target) ? "true" : "false") << std::endl;
        std::cout << "Moved file exists: " << (baba::core::file_utils::file_exists(move_target) ? "true" : "false") << std::endl;
    }
    std::cout << std::endl;
    
    // Directory operations
    std::cout << "=== Directory Operations ===" << std::endl;
    const std::string test_dir = "test_directory";
    const std::string nested_dir = "test_directory/nested/deep";
    
    // Create directories
    if (baba::core::file_utils::create_directory(nested_dir)) {
        std::cout << "✓ Successfully created nested directory: " << nested_dir << std::endl;
        std::cout << "Directory exists: " << (baba::core::file_utils::directory_exists(test_dir) ? "true" : "false") << std::endl;
        std::cout << "Nested directory exists: " << (baba::core::file_utils::directory_exists(nested_dir) ? "true" : "false") << std::endl;
    }
    
    // Create some files in the directory
    baba::core::file_utils::write_file(test_dir + "/file1.txt", "Content of file 1");
    baba::core::file_utils::write_file(test_dir + "/file2.log", "Log content");
    baba::core::file_utils::write_file(test_dir + "/nested/file3.dat", "Data content");
    baba::core::file_utils::create_directory(test_dir + "/empty_subdir");
    std::cout << std::endl;
    
    // List directory contents
    std::cout << "=== Directory Listing ===" << std::endl;
    std::cout << "Files in " << test_dir << " (non-recursive):" << std::endl;
    auto files = baba::core::file_utils::list_files(test_dir, false);
    for (const auto& file : files) {
        std::cout << "  File: " << file << std::endl;
    }
    
    std::cout << "\nDirectories in " << test_dir << " (non-recursive):" << std::endl;
    auto dirs = baba::core::file_utils::list_directories(test_dir, false);
    for (const auto& dir : dirs) {
        std::cout << "  Directory: " << dir << std::endl;
    }
    
    std::cout << "\nAll files in " << test_dir << " (recursive):" << std::endl;
    auto all_files = baba::core::file_utils::list_files(test_dir, true);
    for (const auto& file : all_files) {
        std::cout << "  File: " << file << std::endl;
    }
    std::cout << std::endl;
    
    // Working directory operations
    std::cout << "=== Working Directory Operations ===" << std::endl;
    std::string current_dir = baba::core::file_utils::get_current_directory();
    std::cout << "Current working directory: " << current_dir << std::endl;
    
    // Get absolute paths
    std::cout << "Absolute path of '" << test_file << "': " << baba::core::file_utils::get_absolute_path(test_file) << std::endl;
    std::cout << "Absolute path of '" << test_dir << "': " << baba::core::file_utils::get_absolute_path(test_dir) << std::endl;
    std::cout << std::endl;
    
    // Practical example: File processing
    std::cout << "=== Practical Example: Log File Processing ===" << std::endl;
    const std::string log_dir = "logs";
    baba::core::file_utils::create_directory(log_dir);
    
    // Create some log files
    std::vector<std::string> log_entries = {
        "[2024-01-01 10:00:00] INFO: Application started",
        "[2024-01-01 10:01:15] DEBUG: Loading configuration",
        "[2024-01-01 10:01:20] INFO: Configuration loaded successfully",
        "[2024-01-01 10:02:30] WARN: High memory usage detected",
        "[2024-01-01 10:03:45] ERROR: Database connection failed"
    };
    
    baba::core::file_utils::write_lines(log_dir + "/app.log", log_entries);
    baba::core::file_utils::write_file(log_dir + "/error.log", "[2024-01-01 10:03:45] ERROR: Database connection failed\n[2024-01-01 10:04:00] ERROR: Retry failed");
    
    std::cout << "Created log files in " << log_dir << std::endl;
    auto log_files = baba::core::file_utils::list_files(log_dir);
    for (const auto& log_file : log_files) {
        std::cout << "Log file: " << log_file << " (size: " << baba::core::file_utils::get_file_size(log_file) << " bytes)" << std::endl;
    }
    std::cout << std::endl;
    
    // Cleanup
    std::cout << "=== Cleanup ===" << std::endl;
    std::vector<std::string> files_to_remove = {test_file, lines_file, move_target};
    for (const auto& file : files_to_remove) {
        if (baba::core::file_utils::file_exists(file)) {
            baba::core::file_utils::remove_file(file);
            std::cout << "✓ Removed " << file << std::endl;
        }
    }
    
    std::vector<std::string> dirs_to_remove = {test_dir, log_dir};
    for (const auto& dir : dirs_to_remove) {
        if (baba::core::file_utils::directory_exists(dir)) {
            baba::core::file_utils::remove_directory(dir);
            std::cout << "✓ Removed directory " << dir << std::endl;
        }
    }
    
    std::cout << std::endl << "=== File Utils Example Complete ===" << std::endl;
    return 0;
}
