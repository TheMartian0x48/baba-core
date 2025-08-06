#include "baba/file_utils.hpp"
#include <fstream>
#include <sstream>

namespace baba::core::file_utils {

    std::string read_file(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return "";
        }
        
        std::ostringstream content;
        content << file.rdbuf();
        return content.str();
    }

    bool write_file(const std::string& filepath, const std::string& content) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return file.good();
    }

    bool append_file(const std::string& filepath, const std::string& content) {
        std::ofstream file(filepath, std::ios::app);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return file.good();
    }

    std::vector<std::string> read_lines(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return lines;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        return lines;
    }

    bool write_lines(const std::string& filepath, const std::vector<std::string>& lines) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& line : lines) {
            file << line << '\n';
        }
        
        return file.good();
    }

    bool file_exists(const std::string& filepath) {
        return std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath);
    }

    bool directory_exists(const std::string& dirpath) {
        return std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath);
    }

    bool create_directory(const std::string& dirpath) {
        std::error_code ec;
        return std::filesystem::create_directories(dirpath, ec);
    }

    bool remove_file(const std::string& filepath) {
        std::error_code ec;
        return std::filesystem::remove(filepath, ec);
    }

    bool remove_directory(const std::string& dirpath) {
        std::error_code ec;
        return std::filesystem::remove_all(dirpath, ec) > 0;
    }

    bool copy_file(const std::string& source, const std::string& destination) {
        std::error_code ec;
        return std::filesystem::copy_file(source, destination, ec);
    }

    bool move_file(const std::string& source, const std::string& destination) {
        std::error_code ec;
        std::filesystem::rename(source, destination, ec);
        return !ec;
    }

    std::size_t get_file_size(const std::string& filepath) {
        std::error_code ec;
        auto size = std::filesystem::file_size(filepath, ec);
        return ec ? 0 : size;
    }

    std::string get_extension(const std::string& filepath) {
        return std::filesystem::path(filepath).extension().string();
    }

    std::string get_filename(const std::string& filepath) {
        return std::filesystem::path(filepath).filename().string();
    }

    std::string get_directory(const std::string& filepath) {
        return std::filesystem::path(filepath).parent_path().string();
    }

    std::vector<std::string> list_files(const std::string& dirpath, bool recursive) {
        std::vector<std::string> files;
        std::error_code ec;
        
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dirpath, ec)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(dirpath, ec)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
        }
        
        return files;
    }

    std::vector<std::string> list_directories(const std::string& dirpath, bool recursive) {
        std::vector<std::string> directories;
        std::error_code ec;
        
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dirpath, ec)) {
                if (entry.is_directory()) {
                    directories.push_back(entry.path().string());
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(dirpath, ec)) {
                if (entry.is_directory()) {
                    directories.push_back(entry.path().string());
                }
            }
        }
        
        return directories;
    }

    std::string get_current_directory() {
        std::error_code ec;
        auto path = std::filesystem::current_path(ec);
        return ec ? "" : path.string();
    }

    bool set_current_directory(const std::string& dirpath) {
        std::error_code ec;
        std::filesystem::current_path(dirpath, ec);
        return !ec;
    }

    std::string get_absolute_path(const std::string& filepath) {
        std::error_code ec;
        auto path = std::filesystem::absolute(filepath, ec);
        return ec ? filepath : path.string();
    }

} // namespace baba::core::file_utils
