#pragma once

/**
 * @file aligned_string.hpp
 * @brief Basic aligned string implementation
 */
#include <baba/alias.hpp>
#include <baba/memory.hpp>
#include <cstdlib>

using namespace baba::alias;

namespace baba::ds::string
{

    template <u8 Alignment = 16> struct String {
        char* data;
        u32   size;
        u32   capacity;
    };

    template <u8 Alignment = 16> String<Alignment> make(u32 capacity)
    {
        String<Alignment> str;
        str.data     = baba::memory::aligned_alloc(capacity, Alignment);
        str.size     = 0;
        str.capacity = capacity;
        return str;
    }

    template <u8 Alignment = 16> void free(String<Alignment>& str)
    {
        if (str.data) {
            baba::memory::aligned_free(str.data);
            str.data = nullptr;
        }
        str.size     = 0;
        str.capacity = 0;
    }

} // namespace baba::ds::string
