#pragma once

/**
 * @file aligned_string.hpp
 * @brief Basic aligned string implementation
 */
#include <baba/alias.hpp>
#include <baba/memory.hpp>
#include <cstdlib>

using namespace baba::alias;

namespace baba::ds
{
    struct String {
        char* data;
        u32   len;
        u32   capacity;

        u32   length() const { return len; }
        i32   compare(const String* str)
        {
            u32 l = len > str->len ? str->len : len;
            for (u32 i = 0; i < l; i++) {
                if (data[i] < str->data[i])
                    return -1;
                else if (data[i] > str->data[i])
                    return 1;
            }
            if (len < str->len) {
                return -1;
            } else if (len > str->len) {
                return 1;
            }
            return 0;
        }
    };
} // namespace baba::ds
