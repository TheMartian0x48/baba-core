#ifndef BABA_DS_ARRAY
#define BABA_DS_ARRAY

/**
 * @file array.hpp
 * @brief Dynamic array implementation with arena allocation
 */

#include "baba/ds/pair.hpp"
#include <baba/alias.hpp>
#include <baba/memory.hpp>
#include <stdexcept>

namespace baba::ds::array
{

    using namespace baba::alias;

    /**
    * @brief Dynamic array with arena allocation
    * @tparam T Element type
    */
    template <typename T> struct Array {
        memory::Arena* arena;    ///< Arena allocator (initialized by `make()`)
        T*             data;     ///< Pointer to array data
        u32            size;     ///< Current number of elements
        u32            capacity; ///< Maximum number of elements

        /**
        * @brief Check if array is empty
        * @return `true` if size is 0
        */
        [[nodiscard]]
        constexpr bool empty() const noexcept
        {
            return size == 0;
        }

        /**
        * @brief Get current array size
        * @return Number of elements
        */
        [[nodiscard]]
        constexpr u32 get_size() const noexcept
        {
            return size;
        }

        /**
        * @brief Get array capacity
        * @return Maximum number of elements
        */
        [[nodiscard]]
        constexpr u32 get_capacity() const noexcept
        {
            return capacity;
        }

        /**
        * @brief Get data pointer
        * @return Pointer to array data
        */
        [[nodiscard]]
        constexpr T* get_data() const noexcept
        {
            return data;
        }

        /**
        * @brief Get element at index (unsafe)
        * @param index Element position
        * @return Element at index
        */
        [[nodiscard]]
        constexpr T at(u32 index) const noexcept
            requires PrimitiveComparable<T>
        {
            return data[index];
        }

        /**
        * @brief Get element at index (unsafe)
        * @param index Element position
        * @return Element at index
        */
        [[nodiscard]]
        constexpr T at(u32 index) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            return data[index];
        }

        /**
        * @brief Get element at index (safe)
        * @param index Element position
        * @return Element at index
        * @throw `std::out_of_range` if index >= size
        */
        [[nodiscard]]
        constexpr T at_safe(u32 index) const
            requires PrimitiveComparable<T>
        {
            if (index >= size) {
                throw std::out_of_range("Index out of range, cannot access element.");
            }
            return data[index];
        }

        /**
        * @brief Get element at index (safe)
        * @param index Element position
        * @return Element at index
        * @throw `std::out_of_range` if index >= size
        */
        [[nodiscard]]
        constexpr T at_safe(u32 index) const
            requires(!PrimitiveComparable<T>)
        {
            if (index >= size) {
                throw std::out_of_range("Index out of range, cannot access element.");
            }
            return data[index];
        }

        /**
        * @brief Get first element (unsafe)
        * @return First element
        */
        [[nodiscard]]
        constexpr T begin() const noexcept
            requires PrimitiveComparable<T>
        {
            return data[0];
        }

        /**
     * @brief Get first element (unsafe)
     * @return First element
     */
        [[nodiscard]]
        constexpr T begin() const noexcept
            requires(!PrimitiveComparable<T>)
        {
            return data[0];
        }

        /**
     * @brief Get first element (safe)
     * @return First element
     * @throw `std::out_of_range` if array is empty
     */
        [[nodiscard]]
        constexpr T begin_safe() const
            requires PrimitiveComparable<T>
        {
            if (size == 0) {
                throw std::out_of_range("Array is empty, cannot access begin element.");
            }
            return data[0];
        }

        /**
     * @brief Get first element (safe)
     * @return First element
     * @throw `std::out_of_range` if array is empty
     */
        [[nodiscard]]
        constexpr T begin_safe() const
            requires(!PrimitiveComparable<T>)
        {
            if (size == 0) {
                throw std::out_of_range("Array is empty, cannot access begin element.");
            }
            return data[0];
        }

        /**
     * @brief Get last element (unsafe)
     * @return Last element
     */
        [[nodiscard]]
        constexpr T end() const noexcept
            requires PrimitiveComparable<T>
        {
            return data[size - 1];
        }

        /**
     * @brief Get last element (unsafe)
     * @return Last element
     */
        [[nodiscard]]
        constexpr T end() const noexcept
            requires(!PrimitiveComparable<T>)
        {
            return data[size - 1];
        }

        /**
     * @brief Get last element (safe)
     * @return Last element
     * @throw `std::out_of_range` if array is empty
     */
        [[nodiscard]]
        constexpr T end_safe() const
            requires PrimitiveComparable<T>
        {
            if (size == 0) {
                throw std::out_of_range("Array is empty, cannot access end element.");
            }
            return data[size - 1];
        }

        /**
     * @brief Get last element (safe)
     * @return Last element
     * @throw `std::out_of_range` if array is empty
     */
        [[nodiscard]]
        constexpr T end_safe() const
            requires(!PrimitiveComparable<T>)
        {
            if (size == 0) {
                throw std::out_of_range("Array is empty, cannot access end element.");
            }
            return data[size - 1];
        }

        /**
     * @brief Clear all elements
     * @note Not implemented yet
     */
        constexpr void clear() { throw std::runtime_error("clear() is not implemented yet."); }

        /**
     * @brief Append element (unsafe)
     * @param value Element to append
     */
        constexpr void append(const T value) noexcept
            requires PrimitiveComparable<T>
        {
            data[size++] = value;
        }

        /**
     * @brief Append element (safe)
     * @param value Element to append
     * @throw `std::out_of_range` if capacity exceeded
     */
        constexpr void append_safe(const T value)
            requires PrimitiveComparable<T>
        {
            if (size >= capacity) {
                throw std::out_of_range("Array capacity exceeded, cannot append value.");
            }
            data[size++] = value;
        }

        /**
     * @brief Append element (unsafe)
     * @param value Pointer to element to append
     */
        constexpr void append(const T* value) noexcept
            requires(!PrimitiveComparable<T>)
        {
            data[size++] = *value;
        }

        /**
     * @brief Append element (safe)
     * @param value Pointer to element to append
     * @throw `std::out_of_range` if capacity exceeded
     */
        constexpr void append_safe(const T* value)
            requires(!PrimitiveComparable<T>)
        {
            if (size >= capacity) {
                throw std::out_of_range("Array capacity exceeded, cannot append value.");
            }
            data[size++] = *value;
        }

        /**
        * @brief Update element at index (unsafe)
        * @param at Index position
        * @param value New value
        */
        constexpr void update(u32 at, const T value) noexcept
            requires PrimitiveComparable<T>
        {
            data[at] = value;
        }

        /**
        * @brief Update element at index (safe)
        * @param at Index position
        * @param value New value
        * @throw `std::out_of_range` if index >= size
        */
        constexpr void update_safe(u32 at, const T value)
            requires PrimitiveComparable<T>
        {
            if (at >= size) {
                throw std::out_of_range("Index out of range, cannot update element.");
            }
            data[at] = value;
        }

        /**
        * @brief Update element at index (unsafe)
        * @param at Index position
        * @param value Pointer to new value
        */
        constexpr void update(u32 at, const T* value) noexcept
            requires(!PrimitiveComparable<T>)
        {
            data[at] = *value;
        }

        /**
        * @brief Update element at index (safe)
        * @param at Index position
        * @param value Pointer to new value
        * @throw `std::out_of_range` if index >= size
        */
        constexpr void update_safe(u32 at, const T* value)
            requires(!PrimitiveComparable<T>)
        {
            if (at >= size) {
                throw std::out_of_range("Index out of range, cannot update element.");
            }
            data[at] = *value;
        }

        /**
        * @brief Remove element at index (unsafe)
        * @param index Index to remove
        */
        constexpr void remove(u32 index) noexcept
        {
            // or just copy [index + 1, size) to [index, size - 1]
            for (u32 i = index; i < size - 1; ++i) {
                data[i] = data[i + 1];
            }
            --size;
        }

        /**
        * @brief Remove element at index (safe)
        * @param index Index to remove
        * @throw `std::out_of_range` if index >= size
        */
        constexpr void remove_safe(u32 index)
        {
            if (index >= size) {
                throw std::out_of_range("Index out of range, cannot remove element.");
            }
            // or just copy [index + 1, size) to [index, size - 1]
            for (u32 i = index; i < size - 1; ++i) {
                data[i] = data[i + 1];
            }
            --size;
        }

        /**
        * @brief Fill all elements with value
        * @param value Value to fill with
        */
        constexpr void fill(const T value) noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = 0; i < size; ++i) {
                data[i] = value;
            }
        }

        /**
     * @brief Fill all elements with value
     * @param value Pointer to value to fill with
     */
        constexpr void fill(const T* value) noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = 0; i < size; ++i) {
                data[i] = *value;
            }
        }

        /**
     * @brief Fill range with value (unsafe)
     * @param start Start index
     * @param end End index (exclusive)
     * @param value Value to fill with
     */
        constexpr void fill_range(u32 start, u32 end, const T value) noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = start; i < end; ++i) {
                data[i] = value;
            }
        }

        /**
     * @brief Fill range with value (safe)
     * @param start Start index
     * @param end End index (exclusive)
     * @param value Value to fill with
     * @throw `std::out_of_range` if invalid range
     */
        constexpr void fill_range_safe(u32 start, u32 end, const T value)
            requires PrimitiveComparable<T>
        {
            if (start >= size || end > size || start >= end) {
                throw std::out_of_range("Invalid range for fill_range.");
            }
            for (u32 i = start; i < end; ++i) {
                data[i] = value;
            }
        }

        /**
     * @brief Fill range with value (unsafe)
     * @param start Start index
     * @param end End index (exclusive)
     * @param value Pointer to value to fill with
     */
        constexpr void fill_range(u32 start, u32 end, const T* value) noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = start; i < end; ++i) {
                data[i] = *value;
            }
        }

        /**
     * @brief Fill range with value (safe)
     * @param start Start index
     * @param end End index (exclusive)
     * @param value Pointer to value to fill with
     * @throw `std::out_of_range` if invalid range
     */
        constexpr void fill_range_safe(u32 start, u32 end, const T* value)
            requires(!PrimitiveComparable<T>)
        {
            if (start >= size || end > size || start >= end) {
                throw std::out_of_range("Invalid range for fill_range.");
            }
            for (u32 i = start; i < end; ++i) {
                data[i] = *value;
            }
        }

        /**
     * @brief Check if arrays are equal
     * @param other Array to compare with
     * @return `true` if arrays are equal
     */
        [[nodiscard]]
        constexpr bool equals(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            if (size != other->size) {
                return false;
            }
            for (u32 i = 0; i < size; ++i) {
                if (data[i] != other->data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if arrays are equal using custom function
     * @param other Array to compare with
     * @param equal_fn Custom equality function
     * @return `true` if arrays are equal
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr bool equals(const Array<T>* other, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            if (size != other->size) {
                return false;
            }
            for (u32 i = 0; i < size; ++i) {
                if (equal_fn(&data[i], &(other->data[i])) == false) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if this array is less than other
     * @param other Array to compare with
     * @return `true` if this array is lexicographically less
     */
        [[nodiscard]]
        constexpr bool less_than(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                if (data[i] < other->data[i]) {
                    return true;
                } else if (data[i] > other->data[i]) {
                    return false;
                }
            }
            return size < other->size;
        }

        /**
     * @brief Check if this array is less than other using custom function
     * @param other Array to compare with
     * @param compare_fn Custom comparison function
     * @return `true` if this array is lexicographically less
     */
        template <CompareFn<T> Fn>
        [[nodiscard]]
        constexpr bool less_than(const Array<T>* other, Fn compare_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                int cmp = compare_fn(&data[i], &(other->data[i]));
                if (cmp < 0) {
                    return true;
                } else if (cmp > 0) {
                    return false;
                }
            }
            return size < other->size;
        }

        /**
     * @brief Check if this array is greater than other
     * @param other Array to compare with
     * @return `true` if this array is lexicographically greater
     */
        [[nodiscard]]
        constexpr bool greater_than(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                if (data[i] > other->data[i]) {
                    return true;
                } else if (data[i] < other->data[i]) {
                    return false;
                }
            }
            return size > other->size;
        }

        /**
     * @brief Check if this array is greater than other using custom function
     * @param other Array to compare with
     * @param compare_fn Custom comparison function
     * @return `true` if this array is lexicographically greater
     */
        template <CompareFn<T> Fn>
        [[nodiscard]]
        constexpr bool greater_than(const Array<T>* other, Fn compare_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                int cmp = compare_fn(&data[i], &(other->data[i]));
                if (cmp < 0) {
                    return false;
                } else if (cmp > 0) {
                    return true;
                }
            }
            return size > other->size;
        }

        /**
     * @brief Compare arrays lexicographically
     * @param other Array to compare with
     * @return `-1` if less, `0` if equal, `1` if greater
     */
        [[nodiscard]]
        constexpr int compare(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                if (data[i] < other->data[i]) {
                    return -1;
                } else if (data[i] > other->data[i]) {
                    return 1;
                }
            }
            if (size == other->size) {
                return 0;
            } else if (size < other->size) {
                return -1;
            } else {
                return 1;
            }
        }

        /**
     * @brief Compare arrays using custom function
     * @param other Array to compare with
     * @param compare_fn Custom comparison function
     * @return `-1` if less, `0` if equal, `1` if greater
     */
        template <CompareFn<T> Fn>
        [[nodiscard]]
        constexpr int compare(const Array<T>* other, Fn compare_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            u32 upper_bound = size < other->size ? size : other->size;
            for (u32 i = 0; i < upper_bound; ++i) {
                int result = compare_fn(&data[i], &(other->data[i]));
                if (result != 0) {
                    return result;
                }
            }
            if (size == other->size) {
                return 0;
            } else if (size < other->size) {
                return -1;
            } else {
                return 1;
            }
        }

        /**
     * @brief Check if array starts with another array
     * @param other Array to check as prefix
     * @return `true` if this array starts with other
     */
        [[nodiscard]]
        constexpr bool starts_with(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            if (other == nullptr || other->size > size) {
                return false;
            }
            for (u32 i = 0; i < other->size; ++i) {
                if (data[i] != other->data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array starts with another array using custom function
     * @param other Array to check as prefix
     * @param equal_fn Custom equality function
     * @return `true` if this array starts with other
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr bool starts_with(const Array<T>* other, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            if (other == nullptr || other->size > size) {
                return false;
            }
            for (u32 i = 0; i < other->size; ++i) {
                if (!equal_fn(&data[i], &(other->data[i]))) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array ends with another array
     * @param other Array to check as suffix
     * @return `true` if this array ends with other
     */
        [[nodiscard]]
        constexpr bool ends_with(const Array<T>* other) const noexcept
            requires PrimitiveComparable<T>
        {
            if (other == nullptr || other->size > size) {
                return false;
            }
            u32 start_index = size - other->size;
            for (u32 i = 0; i < other->size; ++i) {
                if (data[start_index + i] != other->data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array ends with another array using custom function
     * @param other Array to check as suffix
     * @param equal_fn Custom equality function
     * @return `true` if this array ends with other
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr bool ends_with(const Array<T>* other, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            if (other == nullptr || other->size > size) {
                return false;
            }
            u32 start_index = size - other->size;
            for (u32 i = 0; i < other->size; ++i) {
                if (!equal_fn(&data[start_index + i], &(other->data[i]))) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array is sorted in ascending order
     * @return `true` if array is sorted
     */
        [[nodiscard]]
        constexpr bool sorted() const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = 1; i < size; ++i) {
                if (data[i - 1] > data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array is sorted using custom function
     * @param greater_than Custom greater-than function
     * @return `true` if array is sorted
     */
        template <GreaterThanFn<T> Fn>
        [[nodiscard]]
        constexpr bool sorted(Fn greater_than) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = 1; i < size; ++i) {
                if (greater_than(&data[i - 1], &data[i])) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array is sorted in descending order
     * @return `true` if array is reverse sorted
     */
        [[nodiscard]]
        constexpr bool sorted_reverse() const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = 1; i < size; ++i) {
                if (data[i] > data[i - 1]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if array is reverse sorted using custom function
     * @param greater_than_fn Custom greater-than function
     * @return `true` if array is reverse sorted
     */
        template <GreaterThanFn<T> Fn>
        [[nodiscard]]
        constexpr bool sorted_reverse(Fn greater_than_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = 1; i < size; ++i) {
                if (greater_than_fn(&data[i], &data[i - 1])) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if range is sorted in ascending order
     * @param start Start index
     * @param end End index (exclusive)
     * @return `true` if range is sorted
     */
        [[nodiscard]]
        constexpr bool sorted_range(u32 start, u32 end) const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = start + 1; i < end; ++i) {
                if (data[i - 1] > data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if range is sorted using custom function
     * @param start Start index
     * @param end End index (exclusive)
     * @param greater_than Custom greater-than function
     * @return `true` if range is sorted
     */
        template <GreaterThanFn<T> Fn>
        [[nodiscard]]
        constexpr bool sorted_range(u32 start, u32 end, Fn greater_than) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = start + 1; i < end; ++i) {
                if (greater_than(&data[i - 1], &data[i])) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if range is sorted in descending order
     * @param start Start index
     * @param end End index (exclusive)
     * @return `true` if range is reverse sorted
     */
        [[nodiscard]]
        constexpr bool sorted_range_reverse(u32 start, u32 end) const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = start + 1; i < end; ++i) {
                if (data[i] > data[i - 1]) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Check if range is reverse sorted using custom function
     * @param start Start index
     * @param end End index (exclusive)
     * @param greater_than_fn Custom greater-than function
     * @return `true` if range is reverse sorted
     */
        template <GreaterThanFn<T> Fn>
        [[nodiscard]]
        constexpr bool sorted_range_reverse(u32 start, u32 end, Fn greater_than_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = start + 1; i < end; ++i) {
                if (greater_than_fn(&data[i], &data[i - 1])) {
                    return false;
                }
            }
            return true;
        }

        /**
     * @brief Find first occurrence of value
     * @param x Value to find
     * @return Index of first occurrence or `-1` if not found
     */
        [[nodiscard]]
        constexpr i32 find(T x) const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = 0; i < size; ++i) {
                if (data[i] == x) {
                    return i;
                }
            }
            return -1;
        }

        /**
     * @brief Find first occurrence using custom function
     * @param x Value to find
     * @param equal_fn Custom equality function
     * @return Index of first occurrence or `-1` if not found
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr i32 find(const T* x, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = 0; i < size; ++i) {
                if (equal_fn(&data[i], x)) {
                    return i;
                }
            }
            return -1;
        }

        /**
     * @brief Find last occurrence of value
     * @param x Value to find
     * @return Index of last occurrence or `-1` if not found
     */
        [[nodiscard]]
        constexpr i32 find_reverse(T x) const noexcept
            requires PrimitiveComparable<T>
        {
            for (i32 i = size - 1; i >= 0; --i) {
                if (data[i] == x) {
                    return i;
                }
            }
            return -1;
        }

        /**
     * @brief Find last occurrence using custom function
     * @param x Value to find
     * @param equal_fn Custom equality function
     * @return Index of last occurrence or `-1` if not found
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr i32 find_reverse(const T* x, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (i32 i = size - 1; i >= 0; --i) {
                if (equal_fn(&data[i], x)) {
                    return i;
                }
            }
            return -1;
        }

        /**
     * @brief Check if array contains value
     * @param x Value to search for
     * @return `true` if value is found
     */
        [[nodiscard]]
        constexpr bool contains(T x) const noexcept
            requires PrimitiveComparable<T>
        {
            for (u32 i = 0; i < size; ++i) {
                if (data[i] == x) {
                    return true;
                }
            }
            return false;
        }

        /**
     * @brief Check if array contains value using custom function
     * @param x Value to search for
     * @param equal_fn Custom equality function
     * @return `true` if value is found
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr bool contains(const T* x, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            for (u32 i = 0; i < size; ++i) {
                if (equal_fn(&data[i], x)) {
                    return true;
                }
            }
            return false;
        }

        /**
     * @brief Count occurrences of value
     * @param x Value to count
     * @return Number of occurrences
     */
        [[nodiscard]]
        constexpr i32 counts(T x) const noexcept
            requires PrimitiveComparable<T>
        {
            i32 count = 0;
            for (u32 i = 0; i < size; ++i) {
                if (data[i] == x) {
                    count++;
                }
            }
            return count;
        }

        /**
     * @brief Count occurrences using custom function
     * @param x Value to count
     * @param equal_fn Custom equality function
     * @return Number of occurrences
     */
        template <EqualFn<T> Fn>
        [[nodiscard]]
        constexpr i32 counts(const T* x, Fn equal_fn) const noexcept
            requires(!PrimitiveComparable<T>)
        {
            i32 count = 0;
            for (u32 i = 0; i < size; ++i) {
                if (equal_fn(&data[i], x)) {
                    count++;
                }
            }
            return count;
        }
    };

    /**
    * @brief Create array with given capacity
    * @tparam T Element type
    * @param arena Arena allocator
    * @param size Initial capacity
    * @return New array instance
    * @note Not implemented yet
    */
    template <typename T> Array<T> make(baba::memory::Arena* arena, u32 size)
    {
        void* ptr = arena->alloc(size * sizeof(T));
        if (ptr == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
        return Array<T>{
            arena,
            static_cast<T*>(ptr),
            size,
            size,
        };
    }

    template <typename T> Array<T> make_with_capacity(baba::memory::Arena* arena, u32 size, u32 capacity)
    {
        void* ptr = arena->alloc(capacity * sizeof(T));
        if (ptr == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
        return Array<T>{
            arena,
            static_cast<T*>(ptr),
            size,
            capacity,
        };
    }

    /**
    * @brief Destroy array and free memory
    * @tparam T Element type
    * @param size Array size (unused parameter)
    * @return Empty array instance
    * @note Not implemented yet
    */
    template <typename T> void kill(Array<T>* arr)
    {
        if (arr == nullptr)
            return;
        arr->arena->dealloc(arr->data);
        arr->data     = nullptr;
        arr->size     = 0;
        arr->capacity = 0;
    }

    template <typename T> Array<T> copy(baba::memory::Arena* arena, const Array<T>* source)
    {
        void* ptr = arena->alloc(source->size * sizeof(T));
        if (ptr == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
        Array<T> arr = {
            arena,
            static_cast<T*>(ptr),
            source->size,
            source->size,
        };
        for (u32 i = 0; i < arr.size; i++) {
            arr.data[i] = source->data[i];
        }
        return arr;
    }

    template <typename T> Array<T> merge(baba::memory::Arena* arena, const Array<T>* a, const Array<T>* b)
    {
        u32   a_size{a->size}, b_size{b->size};
        u32   size{a_size + b_size};
        void* ptr{arena->alloc(size * sizeof(T))};
        if (ptr == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
        Array<T> arr = {
            arena,
            static_cast<T*>(ptr),
            size,
            size,
        };
        for (u32 i = 0; i < a_size; i++) {
            arr.data[i] = a->data[i];
        }
        for (u32 i = 0; i < b_size; i++) {
            arr.data[i + a_size] = b->data[i];
        }
        return arr;
    }

    template <typename T> ds::Pair<Array<T>, Array<T>> split(const Array<T>* arr, u32 index)
    {
        if (index > arr->size) {
            throw std::out_of_range("index is out of range");
        }
        u32  size{arr->size};
        u32  left_size{index + 1}, right_size{size - index - 1};
        auto left_ptr  = static_cast<T*>(arr->arena->alloc(left_size * sizeof(T)));
        auto right_ptr = static_cast<T*>(arr->arena->alloc(right_size * sizeof(T)));
        if (left_ptr == nullptr || right_ptr == nullptr) {
            arr->arena->dealloc(left_ptr);
            arr->arena->dealloc(right_ptr);
            throw std::runtime_error("Failed to allocate memory");
        }
        Array<T> left = {
            arr->arena,
            static_cast<T*>(left_ptr),
            left_size,
            left_size,
        };
        for (u32 i = 0; i <= index; i++) {
            left.data[i] = arr->at(i);
        }
        Array<T> right = {
            arr->arena,
            static_cast<T*>(right_ptr),
            right_size,
            right_size,
        };
        for (u32 i = index + 1; i < size; i++) {
            right.data[i - index - 1] = arr->at(i);
        }
        return {left, right};
    }

    template <typename T> void swap(Array<T>* a, Array<T>* b) noexcept
    {
        auto size     = a->size;
        auto capacity = a->capacity;
        auto arena    = a->arena;
        auto data     = a->data;

        a->size       = b->size;
        a->capacity   = b->capacity;
        a->arena      = b->arena;
        a->data       = b->data;

        b->size       = size;
        b->capacity   = capacity;
        b->arena      = arena;
        b->data       = data;
    }

    // template <typename T> void sort(Array<T>& arr)
    // {
    //     throw std::runtime_error("sort() is not implemented yet.");
    // }
    //
    // template <typename T> bool binary_search(const Array<T>& arr, const T& value)
    // {
    //     throw std::runtime_error("binary_search() is not implemented yet.");
    // }
    // template <typename T> u32 lower_bound(const Array<T>& arr, const T& value)
    // {
    //     throw std::runtime_error("lower_bound() is not implemented yet.");
    // }
    // template <typename T> u32 upper_bound(const Array<T>& arr, const T& value)
    // {
    //     throw std::runtime_error("upper_bound() is not implemented yet.");
    // }
    // template <typename T> void sort(Array<T>& arr, CompareFn compare)
    // {
    //     throw std::runtime_error("sort() with custom compare is not implemented yet.");
    // }
    // template <typename T> bool binary_search(const Array<T>& arr, const T& value, CompareFn compare)
    // {
    //     throw std::runtime_error("binary_search() with custom compare is not implemented yet.");
    // }
    // template <typename T> u32 lower_bound(const Array<T>& arr, const T& value, CompareFn compare)
    // {
    //     throw std::runtime_error("lower_bound() with custom compare is not implemented yet.");
    // }
    // template <typename T> u32 upper_bound(const Array<T>& arr, const T& value, CompareFn compare)
    // {
    //     throw std::runtime_error("upper_bound() with custom compare is not implemented yet.");
    // }
    //
    // template <typename T> void reverse(Array<T>& arr)
    // {
    //     throw std::runtime_error("reverse() is not implemented yet.");
    // }
    //
    // template <typename T> Array<T> map(const Array<T>& arr, Function func, core::Arena* arena)
    // {
    //     throw std::runtime_error("map() is not implemented yet.");
    // }
    //
    // template <typename T> Array<T> filter(const Array<T>& arr, Predicate pred, core::Arena* arena)
    // {
    //     throw std::runtime_error("filter() is not implemented yet.");
    // }

} // namespace baba::ds::array

#endif
