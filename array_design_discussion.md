# Array Design Discussion

**File**: `include/baba/ds/array.hpp`  
**Status**: Design Complete - Ready for Implementation

## Concepts Definition

```cpp
#include <concepts>
#include <type_traits>

template<typename T>
concept PrimitiveComparable = std::is_arithmetic_v<T> || std::is_pointer_v<T>;
```

## Function Signatures

### C-Style Functions (17 functions)

#### Factory & Lifecycle
```cpp
template<typename T>
[[nodiscard]] constexpr Array<T> make(Arena* arena, u32 size);

template<typename T>
constexpr void kill(Array<T>& arr) noexcept;

template<typename T>
[[nodiscard]] constexpr Array<T> copy(const Array<T>& source, Arena* arena);
```

#### Multi-array Operations
```cpp
template<typename T>
[[nodiscard]] constexpr Array<T> merge(const Array<T>& a, const Array<T>& b, Arena* arena);

template<typename T>
constexpr void split(const Array<T>& arr, u32 index, Array<T>& left, Array<T>& right);

template<typename T>
constexpr void swap(Array<T>& a, Array<T>& b) noexcept;
```

#### Algorithms (Primitive Types)
```cpp
template<PrimitiveComparable T>
constexpr void sort(Array<T>& arr);

template<PrimitiveComparable T>
[[nodiscard]] constexpr bool binary_search(const Array<T>& arr, const T& value);

template<PrimitiveComparable T>
[[nodiscard]] constexpr u32 lower_bound(const Array<T>& arr, const T& value);

template<PrimitiveComparable T>
[[nodiscard]] constexpr u32 upper_bound(const Array<T>& arr, const T& value);
```

#### Algorithms (Non-primitive Types)
```cpp
template<typename T, typename CompareFn>
constexpr void sort(Array<T>& arr, CompareFn compare) requires (!PrimitiveComparable<T>);

template<typename T, typename CompareFn>
[[nodiscard]] constexpr bool binary_search(const Array<T>& arr, const T& value, CompareFn compare) requires (!PrimitiveComparable<T>);

template<typename T, typename CompareFn>
[[nodiscard]] constexpr u32 lower_bound(const Array<T>& arr, const T& value, CompareFn compare) requires (!PrimitiveComparable<T>);

template<typename T, typename CompareFn>
[[nodiscard]] constexpr u32 upper_bound(const Array<T>& arr, const T& value, CompareFn compare) requires (!PrimitiveComparable<T>);
```

#### Utility Functions
```cpp
template<typename T>
constexpr void reverse(Array<T>& arr) noexcept;

template<typename T, typename Function>
[[nodiscard]] constexpr Array<T> map(const Array<T>& arr, Function func, Arena* arena);

template<typename T, typename Predicate>
[[nodiscard]] constexpr Array<T> filter(const Array<T>& arr, Predicate pred, Arena* arena);
```

### Struct Methods (50+ functions)

```cpp
template<typename T>
struct Array {
    Arena* arena;
    T* data;
    u32 size;
    
    // Properties
    [[nodiscard]] constexpr bool empty() const noexcept;
    // direct access to size
    [[nodiscard]] constexpr u32 size() const noexcept;
    // direct access to data
    [[nodiscard]] constexpr T* data() noexcept;
    [[nodiscard]] constexpr const T* data() const noexcept;
    [[nodiscard]] constexpr T front(u32 index) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr T end(u32 index) const requires PrimitiveComparable<T>;
    
    // Modification
    constexpr void update(u32 index, const T& value);
    constexpr void update_safe(u32 index, const T& value);
    constexpr void clear() noexcept;
    constexpr void append(const T& value);
    constexpr void remove_at(u32 index);
    constexpr void fill(const T& value);
    constexpr void fill_range(u32 start, u32 end, const T& value);
    
    // Search (Primitive types)
    [[nodiscard]] constexpr i32 find(const T& value) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr i32 rfind(const T& value) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool contains(const T& value) const noexcept requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr u32 count(const T& value) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr Array<u32> find_all(const T& value) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr Array<u32> rfind_all(const T& value) const requires PrimitiveComparable<T>;
    
    // Search (Non-primitive types)
    template<typename EqualFn>
    [[nodiscard]] constexpr i32 find(const T& value, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr i32 rfind(const T& value, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr bool contains(const T& value, EqualFn equals) const noexcept requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr u32 count(const T& value, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr Array<u32> find_all(const T& value, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr Array<u32> rfind_all(const T& value, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    // Advanced search
    template<typename Predicate>
    [[nodiscard]] constexpr i32 find_if(Predicate pred) const;
    
    // Comparisons (Primitive types)
    [[nodiscard]] constexpr bool equals(const Array<T>& other) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr i32 compare(const Array<T>& other) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool less_than(const Array<T>& other) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool greater_than(const Array<T>& other) const requires PrimitiveComparable<T>;
    
    // Comparisons (Non-primitive types)
    template<typename EqualFn>
    [[nodiscard]] constexpr bool equals(const Array<T>& other, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr i32 compare(const Array<T>& other, CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr bool less_than(const Array<T>& other, CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr bool greater_than(const Array<T>& other, CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    // Pattern matching (Primitive types)
    [[nodiscard]] constexpr bool starts_with(const Array<T>& prefix) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool ends_with(const Array<T>& suffix) const requires PrimitiveComparable<T>;
    
    // Pattern matching (Non-primitive types)
    template<typename EqualFn>
    [[nodiscard]] constexpr bool starts_with(const Array<T>& prefix, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    template<typename EqualFn>
    [[nodiscard]] constexpr bool ends_with(const Array<T>& suffix, EqualFn equals) const requires (!PrimitiveComparable<T>);
    
    // Queries (Primitive types)
    [[nodiscard]] constexpr bool is_sorted() const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool is_sorted_reverse() const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool is_sorted_until(u32 index) const requires PrimitiveComparable<T>;
    [[nodiscard]] constexpr bool is_sorted_reverse_until(u32 index) const requires PrimitiveComparable<T>;
    
    // Queries (Non-primitive types)
    template<typename CompareFn>
    [[nodiscard]] constexpr bool is_sorted(CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr bool is_sorted_reverse(CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr bool is_sorted_until(u32 index, CompareFn compare) const requires (!PrimitiveComparable<T>);
    
    template<typename CompareFn>
    [[nodiscard]] constexpr bool is_sorted_reverse_until(u32 index, CompareFn compare) const requires (!PrimitiveComparable<T>);
};
```

## Design Decisions

**Memory Management**: Arena-style allocation (N creation + single destruction)  
**API Style**: Hybrid - C-style for creation/algorithms, struct methods for instance operations  
**Template Design**: Generic version without alignment parameter  
**Safety Strategy**: Tiered approach with explicit safe variants  
**Comparison Strategy**: Dual interface - automatic for primitives, explicit functions for custom types  
**C++20 Features**: constexpr, [[nodiscard]], concepts, requires clauses

## Usage Examples

### Primitive Types (Automatic)
```cpp
Array<int> ints = make<int>(&arena, 100);
Array<float> floats = make<float>(&arena, 100);
Array<char*> pointers = make<char*>(&arena, 100);

// Simple calls - no compare functions needed
bool sorted1 = ints.is_sorted();
bool found1 = ints.contains(42);
sort(floats);
bool result = binary_search(pointers, ptr);
```

### Non-Primitive Types (Explicit Functions)
```cpp
struct Person {
    char name[32];
    int age;
};

Array<Person> people = make<Person>(&arena, 100);

// Must provide comparison/equality functions
bool sorted = people.is_sorted([](const Person& a, const Person& b) {
    return a.age < b.age;
});

bool found = people.contains(person, [](const Person& a, const Person& b) {
    return strcmp(a.name, b.name) == 0;
});

sort(people, [](const Person& a, const Person& b) {
    return a.age < b.age;
});
```

### Mixed Usage
```cpp
// Core functions work for all types
if (!people.empty()) {
    Person& first = people.front();
    people.update(0, new_person);
    people.fill_range(10, 20, default_person);
}

// Predicate-based functions work for all types
i32 adult_pos = people.find_if([](const Person& p) {
    return p.age >= 18;
});
```

## Implementation Notes

**Performance**: constexpr enables compile-time evaluation  
**Safety**: [[nodiscard]] prevents ignoring return values  
**Type Safety**: Concepts prevent incompatible template instantiation  
**Optimization**: [[likely]]/[[unlikely]] can be added for branch prediction  
**Arena Integration**: Arena struct should contain instance data  

**Total Functions**: 67+ (17 C-style + 50+ struct methods)

## Implementation Checklist

### C-Style Functions (17 functions)

#### Factory & Lifecycle
- [ ] `Array<T> make(Arena* arena, u32 size)`
- [ ] `void kill(Array<T>& arr) noexcept`
- [ ] `Array<T> copy(const Array<T>& source, Arena* arena)`

#### Multi-array Operations
- [ ] `Array<T> merge(const Array<T>& a, const Array<T>& b, Arena* arena)`
- [ ] `void split(const Array<T>& arr, u32 index, Array<T>& left, Array<T>& right)`
- [ ] `void swap(Array<T>& a, Array<T>& b) noexcept`

#### Algorithms (Primitive)
- [ ] `void sort(Array<T>& arr)` (PrimitiveComparable)
- [ ] `bool binary_search(const Array<T>& arr, const T& value)` (PrimitiveComparable)
- [ ] `u32 lower_bound(const Array<T>& arr, const T& value)` (PrimitiveComparable)
- [ ] `u32 upper_bound(const Array<T>& arr, const T& value)` (PrimitiveComparable)

#### Algorithms (Non-primitive)
- [ ] `void sort(Array<T>& arr, CompareFn compare)` (!PrimitiveComparable)
- [ ] `bool binary_search(const Array<T>& arr, const T& value, CompareFn compare)` (!PrimitiveComparable)
- [ ] `u32 lower_bound(const Array<T>& arr, const T& value, CompareFn compare)` (!PrimitiveComparable)
- [ ] `u32 upper_bound(const Array<T>& arr, const T& value, CompareFn compare)` (!PrimitiveComparable)

#### Utility
- [ ] `void reverse(Array<T>& arr) noexcept`
- [ ] `Array<T> map(const Array<T>& arr, Function func, Arena* arena)`
- [ ] `Array<T> filter(const Array<T>& arr, Predicate pred, Arena* arena)`

### Struct Methods (50+ functions)

#### Properties
- [x] `bool empty() const noexcept` (implemented as `is_empty`)
- [ ] `u32 size() const noexcept`
- [ ] `T* data() noexcept`
- [ ] `const T* data() const noexcept`

#### Element Access
- [x] `T& at(u32 index)` (implemented as `get_at`)
- [x] `T& at_safe(u32 index)`
- [x] `T& front()`
- [x] `T& back()`

#### Modification
- [x] `void update(u32 index, const T& value)`
- [x] `void update_safe(u32 index, const T& value)`
- [ ] `void clear() noexcept`
- [x] `void append(const T& value)`
- [x] `void remove_at(u32 index)`
- [x] `void fill(const T& value)`
- [x] `void fill_range(u32 start, u32 end, const T& value)`

#### Search (Primitive + Non-primitive overloads)
- [ ] `i32 find(const T& value)` / `i32 find(const T& value, EqualFn equals)`
- [ ] `i32 rfind(const T& value)` / `i32 rfind(const T& value, EqualFn equals)`
- [ ] `bool contains(const T& value)` / `bool contains(const T& value, EqualFn equals)`
- [ ] `u32 count(const T& value)` / `u32 count(const T& value, EqualFn equals)`
- [ ] `Array<u32> find_all(const T& value)` / `Array<u32> find_all(const T& value, EqualFn equals)`
- [ ] `Array<u32> rfind_all(const T& value)` / `Array<u32> rfind_all(const T& value, EqualFn equals)`
- [ ] `i32 find_if(Predicate pred)`

#### Comparisons (Primitive + Non-primitive overloads)
- [ ] `bool equals(const Array<T>& other)` / `bool equals(const Array<T>& other, EqualFn equals)`
- [ ] `i32 compare(const Array<T>& other)` / `i32 compare(const Array<T>& other, CompareFn compare)`
- [ ] `bool less_than(const Array<T>& other)` / `bool less_than(const Array<T>& other, CompareFn compare)`
- [ ] `bool greater_than(const Array<T>& other)` / `bool greater_than(const Array<T>& other, CompareFn compare)`

#### Pattern Matching (Primitive + Non-primitive overloads)
- [ ] `bool starts_with(const Array<T>& prefix)` / `bool starts_with(const Array<T>& prefix, EqualFn equals)`
- [ ] `bool ends_with(const Array<T>& suffix)` / `bool ends_with(const Array<T>& suffix, EqualFn equals)`

#### Queries (Primitive + Non-primitive overloads)
- [ ] `bool is_sorted()` / `bool is_sorted(CompareFn compare)`
- [ ] `bool is_sorted_reverse()` / `bool is_sorted_reverse(CompareFn compare)`
- [ ] `bool is_sorted_until(u32 index)` / `bool is_sorted_until(u32 index, CompareFn compare)`
- [ ] `bool is_sorted_reverse_until(u32 index)` / `bool is_sorted_reverse_until(u32 index, CompareFn compare)`

### Implementation Progress
**Completed**: 2/67+ functions (3%)  
**Remaining**: 65+ functions (97%)

### Priority Order
1. **Phase 1 (Core)**: 9 functions - Essential for basic functionality
2. **Phase 2 (Common)**: 12 functions - Frequently used operations  
3. **Phase 3 (Advanced)**: 46+ functions - Specialized features

