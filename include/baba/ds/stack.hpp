#pragma once

// /**
//  * @file stack.hpp
//  * @brief Basic aligned stack implementation
//  */
//
// #include <baba/memory.hpp>
// #include <cstddef>
//
// namespace baba::ds
// {
//
//     /**
//  * @brief Basic stack implementation with aligned memory
//  * @tparam T Type of elements to store
//  * @tparam Alignment Memory alignment requirement (default 16 bytes)
//  */
//     template <typename T, size_t Alignment = 16> class stack
//     {
//       private:
//         T*     data_;
//         size_t size_;
//         size_t capacity_;
//
//         void   grow();
//
//       public:
//         // Constructors
//         stack();
//         explicit stack(size_t initial_capacity);
//         stack(const stack& other);
//         stack(stack&& other) noexcept;
//
//         // Destructor
//         ~stack();
//
//         // Assignment operators
//         stack& operator=(const stack& other);
//         stack& operator=(stack&& other) noexcept;
//
//         // Element access
//         T&       top();
//         const T& top() const;
//
//         // Capacity
//         bool   empty() const noexcept;
//         size_t size() const noexcept;
//         size_t capacity() const noexcept;
//
//         // Modifiers
//         void push(const T& item);
//         void push(T&& item);
//         void pop();
//         void clear() noexcept;
//         void reserve(size_t new_capacity);
//     };
//
//     // Implementation
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>::stack() : data_(nullptr), size_(0), capacity_(0)
//     {
//     }
//
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>::stack(size_t initial_capacity) : data_(nullptr), size_(0), capacity_(0)
//     {
//         reserve(initial_capacity);
//     }
//
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>::stack(const stack& other) : data_(nullptr), size_(0), capacity_(0)
//     {
//         reserve(other.capacity_);
//         for (size_t i = 0; i < other.size_; ++i) {
//             new (data_ + i) T(other.data_[i]);
//         }
//         size_ = other.size_;
//     }
//
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>::stack(stack&& other) noexcept
//         : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
//     {
//         other.data_     = nullptr;
//         other.size_     = 0;
//         other.capacity_ = 0;
//     }
//
//     template <typename T, size_t Alignment> stack<T, Alignment>::~stack()
//     {
//         clear();
//         if (data_) {
//             baba::memory::aligned_free(data_);
//         }
//     }
//
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>& stack<T, Alignment>::operator=(const stack& other)
//     {
//         if (this != &other) {
//             clear();
//             reserve(other.capacity_);
//             for (size_t i = 0; i < other.size_; ++i) {
//                 new (data_ + i) T(other.data_[i]);
//             }
//             size_ = other.size_;
//         }
//         return *this;
//     }
//
//     template <typename T, size_t Alignment>
//     stack<T, Alignment>& stack<T, Alignment>::operator=(stack&& other) noexcept
//     {
//         if (this != &other) {
//             clear();
//             if (data_) {
//                 baba::memory::aligned_free(data_);
//             }
//             data_           = other.data_;
//             size_           = other.size_;
//             capacity_       = other.capacity_;
//             other.data_     = nullptr;
//             other.size_     = 0;
//             other.capacity_ = 0;
//         }
//         return *this;
//     }
//
//     template <typename T, size_t Alignment> T& stack<T, Alignment>::top()
//     {
//         return data_[size_ - 1];
//     }
//
//     template <typename T, size_t Alignment> const T& stack<T, Alignment>::top() const
//     {
//         return data_[size_ - 1];
//     }
//
//     template <typename T, size_t Alignment> bool stack<T, Alignment>::empty() const noexcept
//     {
//         return size_ == 0;
//     }
//
//     template <typename T, size_t Alignment> size_t stack<T, Alignment>::size() const noexcept
//     {
//         return size_;
//     }
//
//     template <typename T, size_t Alignment> size_t stack<T, Alignment>::capacity() const noexcept
//     {
//         return capacity_;
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::push(const T& item)
//     {
//         if (size_ >= capacity_) {
//             grow();
//         }
//         new (data_ + size_) T(item);
//         ++size_;
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::push(T&& item)
//     {
//         if (size_ >= capacity_) {
//             grow();
//         }
//         new (data_ + size_) T(std::move(item));
//         ++size_;
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::pop()
//     {
//         if (size_ > 0) {
//             --size_;
//             data_[size_].~T();
//         }
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::clear() noexcept
//     {
//         for (size_t i = 0; i < size_; ++i) {
//             data_[i].~T();
//         }
//         size_ = 0;
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::reserve(size_t new_capacity)
//     {
//         if (new_capacity > capacity_) {
//             T* new_data =
//                 static_cast<T*>(baba::memory::aligned_alloc(new_capacity * sizeof(T), Alignment));
//             if (new_data) {
//                 // Move existing elements
//                 for (size_t i = 0; i < size_; ++i) {
//                     new (new_data + i) T(std::move(data_[i]));
//                     data_[i].~T();
//                 }
//
//                 if (data_) {
//                     baba::memory::aligned_free(data_);
//                 }
//
//                 data_     = new_data;
//                 capacity_ = new_capacity;
//             }
//         }
//     }
//
//     template <typename T, size_t Alignment> void stack<T, Alignment>::grow()
//     {
//         size_t new_capacity = capacity_ == 0 ? 4 : capacity_ * 2;
//         reserve(new_capacity);
//     }
//
// } // namespace baba::ds
