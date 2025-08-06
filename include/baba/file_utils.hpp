#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

/**
 * @file file_utils.hpp
 * @brief File I/O and filesystem utility functions
 * @version 1.0.0
 */

namespace baba::core {

    /**
     * @brief File and filesystem utility functions
     */
    namespace file_utils {
        
        /**
         * @brief Read entire file content as string
         * @param filepath Path to the file
         * @return File content as string, empty if file doesn't exist or error
         */
        std::string read_file(const std::string& filepath);
        
        /**
         * @brief Write string content to file
         * @param filepath Path to the file
         * @param content Content to write
         * @return True if successful, false otherwise
         */
        bool write_file(const std::string& filepath, const std::string& content);
        
        /**
         * @brief Append string content to file
         * @param filepath Path to the file
         * @param content Content to append
         * @return True if successful, false otherwise
         */
        bool append_file(const std::string& filepath, const std::string& content);
        
        /**
         * @brief Read file content as lines
         * @param filepath Path to the file
         * @return Vector of lines, empty if file doesn't exist or error
         */
        std::vector<std::string> read_lines(const std::string& filepath);
        
        /**
         * @brief Write lines to file
         * @param filepath Path to the file
         * @param lines Vector of lines to write
         * @return True if successful, false otherwise
         */
        bool write_lines(const std::string& filepath, const std::vector<std::string>& lines);
        
        /**
         * @brief Check if file exists
         * @param filepath Path to the file
         * @return True if file exists, false otherwise
         */
        bool file_exists(const std::string& filepath);
        
        /**
         * @brief Check if directory exists
         * @param dirpath Path to the directory
         * @return True if directory exists, false otherwise
         */
        bool directory_exists(const std::string& dirpath);
        
        /**
         * @brief Create directory (including parent directories)
         * @param dirpath Path to the directory
         * @return True if successful or already exists, false otherwise
         */
        bool create_directory(const std::string& dirpath);
        
        /**
         * @brief Remove file
         * @param filepath Path to the file
         * @return True if successful, false otherwise
         */
        bool remove_file(const std::string& filepath);
        
        /**
         * @brief Remove directory (recursively)
         * @param dirpath Path to the directory
         * @return True if successful, false otherwise
         */
        bool remove_directory(const std::string& dirpath);
        
        /**
         * @brief Copy file
         * @param source Source file path
         * @param destination Destination file path
         * @return True if successful, false otherwise
         */
        bool copy_file(const std::string& source, const std::string& destination);
        
        /**
         * @brief Move/rename file
         * @param source Source file path
         * @param destination Destination file path
         * @return True if successful, false otherwise
         */
        bool move_file(const std::string& source, const std::string& destination);
        
        /**
         * @brief Get file size in bytes
         * @param filepath Path to the file
         * @return File size in bytes, 0 if file doesn't exist or error
         */
        std::size_t get_file_size(const std::string& filepath);
        
        /**
         * @brief Get file extension
         * @param filepath Path to the file
         * @return File extension (including dot), empty if no extension
         */
        std::string get_extension(const std::string& filepath);
        
        /**
         * @brief Get filename without path
         * @param filepath Path to the file
         * @return Filename without directory path
         */
        std::string get_filename(const std::string& filepath);
        
        /**
         * @brief Get directory path from filepath
         * @param filepath Path to the file
         * @return Directory path
         */
        std::string get_directory(const std::string& filepath);
        
        /**
         * @brief List files in directory
         * @param dirpath Path to the directory
         * @param recursive Whether to list recursively
         * @return Vector of file paths
         */
        std::vector<std::string> list_files(const std::string& dirpath, bool recursive = false);
        
        /**
         * @brief List directories in directory
         * @param dirpath Path to the directory
         * @param recursive Whether to list recursively
         * @return Vector of directory paths
         */
        std::vector<std::string> list_directories(const std::string& dirpath, bool recursive = false);
        
        /**
         * @brief Get current working directory
         * @return Current working directory path
         */
        std::string get_current_directory();
        
        /**
         * @brief Change current working directory
         * @param dirpath Path to the directory
         * @return True if successful, false otherwise
         */
        bool set_current_directory(const std::string& dirpath);
        
        /**
         * @brief Get absolute path
         * @param filepath Path to resolve
         * @return Absolute path
         */
        std::string get_absolute_path(const std::string& filepath);
        
    } // namespace file_utils
    
} // namespace baba::core
