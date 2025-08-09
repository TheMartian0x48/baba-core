#ifndef BABA_DS_ARRAY
#define BABA_DS_ARRAY
/**
# Array

| Status |  function (alphabatically sorted) | description |
|--------|-----------------------------------|-------------|
| 🫵🏽 | append | append a value to the array |
| 🫵🏽 | binary_search | perform binary search on the array |
| 🫵🏽 | clear | clear the array |
| 🫵🏽 | compare | compare two arrays |
| 🫵🏽 | copy | copy the array |
| 🫵🏽 | equal | check if two arrays are equal |
| 🫵🏽 | filter | filter the array with a predicate |
| 🫵🏽 | find | find the first occurrence of a value in the array |
| 🫵🏽 | find_all | find all occurrences of a value in the array |
| 🫵🏽 | find_all_reverse | find all occurrences of a value in the array in reverse order |
| 🫵🏽 | find_reverse | find the last occurrence of a value in the array |
| 🫵🏽 | get_at | get the value at the given index |
| 🫵🏽 | get_at_safe| get the value at the given index but valide range on index|
| 🫵🏽 | get_data | get the data pointer of the array |
| 🫵🏽 | get_size | get the size of the array |
| 🫵🏽 | greater_than | check if the first array is greater than the second array |
| ✅ | is_empty | check if the array is empty |
| 🫵🏽 | is_sorted | check if the array is sorted |
| 🫵🏽 | is_sorted_reverse | check if the array is sorted in reverse order |
| 🫵🏽 | is_sorted_reverse_until | check if the array is sorted in reverse order until a given index |
| 🫵🏽 | is_sorted_until | check if the array is sorted until a given index |
| 🫵🏽 | is_valid |  validate whether arena and data is not null|
| 🫵🏽 | kill     | free the array and reset its size |
| 🫵🏽 | less_than | check if the first array is less than the second array |
| 🫵🏽 | lower_bound | find the lower bound of a value in the array |
| 🫵🏽 | make     | create an array with the given size |
| 🫵🏽 | map | map the array with a function |
| 🫵🏽 | merge | merge two arrays |
| 🫵🏽 | reverse | reverse the array |
| 🫵🏽 | sort | sort the array |
| 🫵🏽 | split | split the array into two arrays |
| 🫵🏽 | swap | swap two arrays |
| 🫵🏽 | update_at | update the value at the given index |
| 🫵🏽 | upper_bound | find the upper bound of a value in the array |
| 🫵🏽 |remove_at | remove the value at the given index |
 */

#include <baba/alias/alias.hpp>
#include <baba/core/memory.hpp>
#include <stdexcept>

namespace baba::ds::array
{

using namespace baba::alias;

template <typename T> struct Array {
    core::Arena* arena; // do not change.init at array::make
    T* data;
    u32 size;
    u32 capacity;

    /** 
     * check if array is empty or not 
     */
    [[nodiscard]]
    constexpr bool empty() const noexcept
    {
        return size == 0;
    }

    [[nodiscard]]
    constexpr u32 get_size() const noexcept
    {
        return size;
    }

    [[nodiscard]]
    constexpr u32 get_capacity() const noexcept
    {
        return capacity;
    }

    [[nodiscard]]
    constexpr T* get_data() noexcept
    {
        return data;
    }

    [[nodiscard]]
    constexpr T front() const noexcept
        requires PrimitiveComparable<T>
    {
        return data[0];
    }

    [[nodiscard]]
    constexpr T* front() const noexcept
        requires(!PrimitiveComparable<T>)
    {
        return data[0];
    }

    [[nodiscard]]
    constexpr T front_safe() const
        requires PrimitiveComparable<T>
    {
        if (size == 0) {
            throw std::out_of_range("Array is empty, cannot access front element.");
        }
        return data[0];
    }

    [[nodiscard]]
    constexpr T* front_safe() const
        requires(!PrimitiveComparable<T>)
    {
        if (size == 0) {
            throw std::out_of_range("Array is empty, cannot access front element.");
        }
        return data[0];
    }

    [[nodiscard]]
    constexpr T back() const noexcept
        requires PrimitiveComparable<T>
    {
        return data[size - 1];
    }

    [[nodiscard]]
    constexpr T* back() const noexcept
        requires(!PrimitiveComparable<T>)
    {
        return data[size - 1];
    }

    [[nodiscard]]
    constexpr T back_safe() const
        requires PrimitiveComparable<T>
    {
        if (size == 0) {
            throw std::out_of_range("Array is empty, cannot access back element.");
        }
        return data[size - 1];
    }

    [[nodiscard]]
    constexpr T* back_safe() const
        requires(!PrimitiveComparable<T>)
    {
        if (size == 0) {
            throw std::out_of_range("Array is empty, cannot access back element.");
        }
        return data[size - 1];
    }

    constexpr void clear() noexcept { throw std::runtime_error("clear() is not implemented yet."); }

    constexpr void append(const T value)
        requires PrimitiveComparable<T>
    {
        data[size++] = *value;
    }

    constexpr void append_safe(const T value)
        requires PrimitiveComparable<T>
    {
        if (size >= capacity) {
            throw std::out_of_range("Array capacity exceeded, cannot append value.");
        }
        data[size++] = value;
    }

    constexpr void append(const T* value)
        requires(!PrimitiveComparable<T>)
    {
        data[size++] = *value;
    }

    constexpr void append_safe(const T* value)
        requires(!PrimitiveComparable<T>)
    {
        if (size >= capacity) {
            throw std::out_of_range("Array capacity exceeded, cannot append value.");
        }
        data[size++] = *value;
    }

    constexpr void update(i32 at, const T value)
        requires PrimitiveComparable<T>
    {
        data[at] = *value;
    }

    constexpr void update_safe(i32 at, const T value)
        requires PrimitiveComparable<T>
    {
        if (at < 0 || at >= size) {
            throw std::out_of_range("Index out of range, cannot update element.");
        }
        data[at] = value;
    }

    constexpr void update(i32 at, const T* value)
        requires(!PrimitiveComparable<T>)
    {
        data[at] = *value;
    }

    constexpr void update_safe(i32 at, const T* value)
        requires(!PrimitiveComparable<T>)
    {
        if (at < 0 || at >= size) {
            throw std::out_of_range("Index out of range, cannot update element.");
        }
        data[at] = *value;
    }

    constexpr void remove(u32 index)
    {
        // or just copy [index + 1, size) to [index, size - 1]
        for (u32 i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

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
    constexpr void fill(const T value)
        requires PrimitiveComparable<T>
    {
        for (u32 i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    constexpr void fill(const T* value)
        requires(!PrimitiveComparable<T>)
    {
        for (u32 i = 0; i < size; ++i) {
            data[i] = *value;
        }
    }
    constexpr void fill_range(u32 start, u32 end, const T value)
        requires PrimitiveComparable<T>
    {
        for (u32 i = start; i < end; ++i) {
            data[i] = value;
        }
    }

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

    constexpr void fill_range(u32 start, u32 end, const T* value)
        requires(!PrimitiveComparable<T>)
    {
        if (start >= size || end > size || start >= end) {
            throw std::out_of_range("Invalid range for fill_range.");
        }
        for (u32 i = start; i < end; ++i) {
            data[i] = *value;
        }
    }

    constexpr void fill_range_safe(u32 start, u32 end, const T* value)
        requires(!PrimitiveComparable<T>)
    {
        for (u32 i = start; i < end; ++i) {
            data[i] = *value;
        }
    }

    [[nodiscard]]
    constexpr bool equals(const Array<T>& other) const noexcept
        requires PrimitiveComparable<T>
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
    }

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr bool equals(const Array<T>& other, Fn equals) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (equals(&data[i], &other.data[i]) == false) {
                return false;
            }
        }
    }

    [[nodiscard]]
    constexpr bool less_than(const Array<T>& other) const noexcept
        requires PrimitiveComparable<T>
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (data[i] < other.data[i]) {
                return false;
            }
        }
    }

    template <LessThanFn<T> Fn>
    [[nodiscard]]
    constexpr bool less_than(const Array<T>& other, Fn less_than_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (less_than_fn(&data[i], &other.data[i]) == false) {
                return false;
            }
        }
    }

    [[nodiscard]]
    constexpr bool greater_than(const Array<T>& other) const noexcept
        requires PrimitiveComparable<T>
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (data[i] > other.data[i]) {
                return false;
            }
        }
    }

    template <GreaterThanFn<T> Fn>
    [[nodiscard]]
    constexpr bool greater_than(const Array<T>& other, Fn greater_than_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        if (size != other.size) {
            return false;
        }
        for (u32 i = 0; i < size; ++i) {
            if (greater_than_fn(&data[i], &other.data[i]) == false) {
                return false;
            }
        }
    }

    [[nodiscard]]
    constexpr int compare(const Array<T>& other) const noexcept
        requires PrimitiveComparable<T>
    {
        if (size != other.size) {
            return size < other.size ? -1 : 1;
        }
        for (u32 i = 0; i < size; ++i) {
            if (data[i] < other.data[i]) {
                return -1;
            } else if (data[i] > other.data[i]) {
                return 1;
            }
        }
        return 0;
    }

    template <CompareFn<T> Fn>
    [[nodiscard]]
    constexpr int compare(const Array<T>& other, Fn compare_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        if (size != other.size) {
            return size < other.size ? -1 : 1;
        }
        for (u32 i = 0; i < size; ++i) {
            int result = compare_fn(&data[i], &other.data[i]);
            if (result != 0) {
                return result;
            }
        }
        return 0;
    }

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

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr bool starts_with(const Array<T>* other, Fn equal_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        if (other == nullptr || other->size > size) {
            return false;
        }
        for (u32 i = 0; i < other->size; ++i) {
            if (!equal_fn(&data[i], &other->data[i])) {
                return false;
            }
        }
        return true;
    }

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
            if (!equal_fn(&data[start_index + i], &other->data[i])) {
                return false;
            }
        }
        return true;
    }

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

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr i32 find(Fn equal_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        for (u32 i = 1; i < size; ++i) {
            if (equal_fn(&data[i - 1], &data[i])) {
                return i;
            }
        }
        return -1;
    }

    [[nodiscard]]
    constexpr i32 find_reverse(T x) const noexcept
        requires PrimitiveComparable<T>
    {
        for (u32 i = size - 1; i >= 0; --i) {
            if (data[i] == x) {
                return i;
            }
        }
        return -1;
    }

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr i32 find_reverse(Fn equal_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        for (u32 i = size - 1; i >= 0; --i) {
            if (equal_fn(&data[i - 1], &data[i])) {
                return i;
            }
        }
        return -1;
    }

    [[nodiscard]]
    constexpr bool contains(T x) const noexcept
        requires PrimitiveComparable<T>
    {
        for (u32 i = size - 1; i >= 0; --i) {
            if (data[i] == x) {
                return true;
            }
        }
        return false;
    }

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr bool contains(Fn equal_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        for (u32 i = size - 1; i >= 0; --i) {
            if (equal_fn(&data[i - 1], &data[i])) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]]
    constexpr i32 counts(T x) const noexcept
        requires PrimitiveComparable<T>
    {
        i32 count = 0;
        for (u32 i = size - 1; i >= 0; --i) {
            if (data[i] == x) {
                count++;
            }
        }
        return count;
    }

    template <EqualFn<T> Fn>
    [[nodiscard]]
    constexpr i32 counts(Fn equal_fn) const noexcept
        requires(!PrimitiveComparable<T>)
    {
        i32 count = 0;
        for (u32 i = size - 1; i >= 0; --i) {
            if (equal_fn(&data[i - 1], &data[i])) {
                count++;
            }
        }
        return count;
    }
};

template <typename T> Array<T> make(core::Arena* arena, u32 size) {}

template <typename T> Array<T> kill(u32 size) {}

} // namespace baba::ds::array

#endif
