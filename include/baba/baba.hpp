#pragma once

/**
 * @file baba.hpp
 * @brief Master header for the baba-core library
 */

#include "core/core.hpp"
#include "ds/ds.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "alias.hpp"

namespace baba
{
    // Main baba namespace
    // Contains all baba-core functionality organized by purpose:
    // - baba::core: Low-level utilities
    // - baba::memory: Memory management and arena allocators
    // - baba::logger: Logging functionality
    // - baba::ds: High-performance data structures
} // namespace baba
