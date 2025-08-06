#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <memory>

/**
 * @file string_utils.hpp
 * @brief String manipulation and utility functions
 * @version 1.0.0
 */

namespace baba::core {

    /**
     * @brief String utility functions
     */
    namespace string_utils {
        
        /**
         * @brief Remove leading and trailing whitespace from a string
         * @param str The string to trim
         * @return Trimmed string
         */
        std::string trim(const std::string& str);
        
        /**
         * @brief Remove leading whitespace from a string
         * @param str The string to trim
         * @return Left-trimmed string
         */
        std::string ltrim(const std::string& str);
        
        /**
         * @brief Remove trailing whitespace from a string
         * @param str The string to trim
         * @return Right-trimmed string
         */
        std::string rtrim(const std::string& str);
        
        /**
         * @brief Split a string by delimiter
         * @param str The string to split
         * @param delimiter The delimiter character
         * @return Vector of split strings
         */
        std::vector<std::string> split(const std::string& str, char delimiter);
        
        /**
         * @brief Split a string by delimiter string
         * @param str The string to split
         * @param delimiter The delimiter string
         * @return Vector of split strings
         */
        std::vector<std::string> split(const std::string& str, const std::string& delimiter);
        
        /**
         * @brief Join strings with a delimiter
         * @param strings Vector of strings to join
         * @param delimiter The delimiter string
         * @return Joined string
         */
        std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
        
        /**
         * @brief Convert string to uppercase
         * @param str The string to convert
         * @return Uppercase string
         */
        std::string to_upper(const std::string& str);
        
        /**
         * @brief Convert string to lowercase
         * @param str The string to convert
         * @return Lowercase string
         */
        std::string to_lower(const std::string& str);
        
        /**
         * @brief Check if string starts with prefix
         * @param str The string to check
         * @param prefix The prefix to look for
         * @return True if string starts with prefix
         */
        bool starts_with(const std::string& str, const std::string& prefix);
        
        /**
         * @brief Check if string ends with suffix
         * @param str The string to check
         * @param suffix The suffix to look for
         * @return True if string ends with suffix
         */
        bool ends_with(const std::string& str, const std::string& suffix);
        
        /**
         * @brief Replace all occurrences of a substring
         * @param str The string to modify
         * @param from The substring to replace
         * @param to The replacement string
         * @return String with replacements made
         */
        std::string replace_all(const std::string& str, const std::string& from, const std::string& to);
        
        /**
         * @brief Check if string contains substring
         * @param str The string to search in
         * @param substring The substring to find
         * @return True if substring is found
         */
        bool contains(const std::string& str, const std::string& substring);
        
        /**
         * @brief Format string with arguments (simple printf-style)
         * @tparam Args Argument types
         * @param format Format string
         * @param args Arguments to format
         * @return Formatted string
         */
        template<typename... Args>
        std::string format(const std::string& format, Args... args) {
            int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
            if (size <= 0) {
                return "";
            }
            std::unique_ptr<char[]> buf(new char[size]);
            std::snprintf(buf.get(), size, format.c_str(), args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }
        
    } // namespace string_utils
    
} // namespace baba::core
