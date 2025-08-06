#pragma once

/**
 * @file core.hpp
 * @brief Main header file for the Baba Core Utilities Library
 * @version 1.0.0
 * @author Baba Core Team
 * 
 * This header includes all core utility modules for convenient access.
 * Include this file to access all core functionality.
 */

#include "string_utils.hpp"
#include "file_utils.hpp"
#include "logger.hpp"
#include "time_utils.hpp"

/**
 * @namespace baba::core
 * @brief Main namespace for all core utilities
 */
namespace baba::core {
    
    /**
     * @brief Library version information
     */
    struct Version {
        static constexpr int MAJOR = 1;
        static constexpr int MINOR = 0;
        static constexpr int PATCH = 0;
        static constexpr const char* STRING = "1.0.0";
    };
    
    /**
     * @brief Get the library version as a string
     * @return Version string in format "MAJOR.MINOR.PATCH"
     */
    inline const char* get_version() {
        return Version::STRING;
    }
    
} // namespace baba::core
