#include "baba/string_utils.hpp"
#include <algorithm>
#include <sstream>
#include <memory>

namespace baba::core::string_utils {

    std::string trim(const std::string& str) {
        return ltrim(rtrim(str));
    }

    std::string ltrim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }
        return std::string(start, str.end());
    }

    std::string rtrim(const std::string& str) {
        auto end = str.end();
        while (end != str.begin() && std::isspace(*(end - 1))) {
            end--;
        }
        return std::string(str.begin(), end);
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;
        
        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }
        
        return result;
    }

    std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> result;
        
        if (delimiter.empty()) {
            result.push_back(str);
            return result;
        }
        
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        
        result.push_back(str.substr(start));
        return result;
    }

    std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
        if (strings.empty()) {
            return "";
        }
        
        std::ostringstream result;
        result << strings[0];
        
        for (size_t i = 1; i < strings.size(); ++i) {
            result << delimiter << strings[i];
        }
        
        return result.str();
    }

    std::string to_upper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::string to_lower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    bool starts_with(const std::string& str, const std::string& prefix) {
        if (prefix.length() > str.length()) {
            return false;
        }
        return str.substr(0, prefix.length()) == prefix;
    }

    bool ends_with(const std::string& str, const std::string& suffix) {
        if (suffix.length() > str.length()) {
            return false;
        }
        return str.substr(str.length() - suffix.length()) == suffix;
    }

    std::string replace_all(const std::string& str, const std::string& from, const std::string& to) {
        if (from.empty()) {
            return str;
        }
        
        std::string result = str;
        size_t pos = 0;
        
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        
        return result;
    }

    bool contains(const std::string& str, const std::string& substring) {
        return str.find(substring) != std::string::npos;
    }

} // namespace baba::core::string_utils
