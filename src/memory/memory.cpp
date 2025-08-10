/**
 * @file memory.cpp
 * @brief Implementation of memory management system including aligned allocation and arena allocators
 */

#include "baba/memory/memory.hpp"

#include <cstdint>
#include <cstdlib>

namespace baba::memory
{

    void* aligned_alloc(size_t size, size_t alignment)
    {
        if (size == 0)
            return nullptr;

        // Ensure alignment is power of 2
        if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
            return nullptr;
        }

// Use system aligned_alloc if available (C11/C++17)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        return std::aligned_alloc(alignment, size);
#else
        // Fallback implementation using malloc + manual alignment
        void* ptr = std::malloc(size + alignment - 1 + sizeof(void*));
        if (!ptr)
            return nullptr;

        // Calculate aligned address
        uintptr_t addr         = reinterpret_cast<uintptr_t>(ptr);
        uintptr_t aligned_addr = (addr + sizeof(void*) + alignment - 1) & ~(alignment - 1);

        // Store original pointer before aligned address
        void** aligned_ptr = reinterpret_cast<void**>(aligned_addr);
        aligned_ptr[-1]    = ptr;

        return reinterpret_cast<void*>(aligned_addr);
#endif
    }

    void aligned_free(void* ptr)
    {
        if (!ptr)
            return;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        std::free(ptr);
#else
        // Retrieve original pointer and free it
        void** aligned_ptr = static_cast<void**>(ptr);
        std::free(aligned_ptr[-1]);
#endif
    }

    bool is_aligned(const void* ptr, size_t alignment)
    {
        if (!ptr || alignment == 0)
            return false;
        return (reinterpret_cast<uintptr_t>(ptr) % alignment) == 0;
    }

    // Note: All arena implementations are currently in the header file as inline functions
    // This is intentional for performance reasons, but could be moved here if needed

} // namespace baba::memory
