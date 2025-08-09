#pragma once

/**
 * @file queue.hpp
 * @brief Basic aligned queue implementation
 */

#include "baba/core/memory.hpp"
#include <cstddef>

namespace baba::ds {

/**
 * @brief Basic queue implementation with aligned memory
 * @tparam T Type of elements to store
 * @tparam Alignment Memory alignment requirement (default 16 bytes)
 */
template<typename T, size_t Alignment = 16>
class queue {
private:
    T* data_;
    size_t front_;
    size_t rear_;
    size_t size_;
    size_t capacity_;

    void grow();

public:
    // Constructors
    queue();
    explicit queue(size_t initial_capacity);
    queue(const queue& other);
    queue(queue&& other) noexcept;

    // Destructor
    ~queue();

    // Assignment operators
    queue& operator=(const queue& other);
    queue& operator=(queue&& other) noexcept;

    // Element access
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // Capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    // Modifiers
    void push(const T& item);
    void push(T&& item);
    void pop();
    void clear() noexcept;
    void reserve(size_t new_capacity);
};

// Implementation
template<typename T, size_t Alignment>
queue<T, Alignment>::queue() : data_(nullptr), front_(0), rear_(0), size_(0), capacity_(0) {}

template<typename T, size_t Alignment>
queue<T, Alignment>::queue(size_t initial_capacity) : data_(nullptr), front_(0), rear_(0), size_(0), capacity_(0) {
    reserve(initial_capacity);
}

template<typename T, size_t Alignment>
queue<T, Alignment>::queue(const queue& other) : data_(nullptr), front_(0), rear_(0), size_(0), capacity_(0) {
    reserve(other.capacity_);
    size_t idx = other.front_;
    for (size_t i = 0; i < other.size_; ++i) {
        new (data_ + rear_) T(other.data_[idx]);
        rear_ = (rear_ + 1) % capacity_;
        idx = (idx + 1) % other.capacity_;
    }
    size_ = other.size_;
}

template<typename T, size_t Alignment>
queue<T, Alignment>::queue(queue&& other) noexcept 
    : data_(other.data_), front_(other.front_), rear_(other.rear_), 
      size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.front_ = 0;
    other.rear_ = 0;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T, size_t Alignment>
queue<T, Alignment>::~queue() {
    clear();
    if (data_) {
        baba::core::aligned_free(data_);
    }
}

template<typename T, size_t Alignment>
queue<T, Alignment>& queue<T, Alignment>::operator=(const queue& other) {
    if (this != &other) {
        clear();
        reserve(other.capacity_);
        front_ = 0;
        rear_ = 0;
        
        size_t idx = other.front_;
        for (size_t i = 0; i < other.size_; ++i) {
            new (data_ + rear_) T(other.data_[idx]);
            rear_ = (rear_ + 1) % capacity_;
            idx = (idx + 1) % other.capacity_;
        }
        size_ = other.size_;
    }
    return *this;
}

template<typename T, size_t Alignment>
queue<T, Alignment>& queue<T, Alignment>::operator=(queue&& other) noexcept {
    if (this != &other) {
        clear();
        if (data_) {
            baba::core::aligned_free(data_);
        }
        data_ = other.data_;
        front_ = other.front_;
        rear_ = other.rear_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.front_ = 0;
        other.rear_ = 0;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T, size_t Alignment>
T& queue<T, Alignment>::front() {
    return data_[front_];
}

template<typename T, size_t Alignment>
const T& queue<T, Alignment>::front() const {
    return data_[front_];
}

template<typename T, size_t Alignment>
T& queue<T, Alignment>::back() {
    size_t back_idx = (rear_ + capacity_ - 1) % capacity_;
    return data_[back_idx];
}

template<typename T, size_t Alignment>
const T& queue<T, Alignment>::back() const {
    size_t back_idx = (rear_ + capacity_ - 1) % capacity_;
    return data_[back_idx];
}

template<typename T, size_t Alignment>
bool queue<T, Alignment>::empty() const noexcept {
    return size_ == 0;
}

template<typename T, size_t Alignment>
size_t queue<T, Alignment>::size() const noexcept {
    return size_;
}

template<typename T, size_t Alignment>
size_t queue<T, Alignment>::capacity() const noexcept {
    return capacity_;
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::push(const T& item) {
    if (size_ >= capacity_) {
        grow();
    }
    new (data_ + rear_) T(item);
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::push(T&& item) {
    if (size_ >= capacity_) {
        grow();
    }
    new (data_ + rear_) T(std::move(item));
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::pop() {
    if (size_ > 0) {
        data_[front_].~T();
        front_ = (front_ + 1) % capacity_;
        --size_;
    }
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::clear() noexcept {
    while (size_ > 0) {
        data_[front_].~T();
        front_ = (front_ + 1) % capacity_;
        --size_;
    }
    front_ = 0;
    rear_ = 0;
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        T* new_data = static_cast<T*>(baba::core::aligned_alloc(new_capacity * sizeof(T), Alignment));
        if (new_data) {
            // Move existing elements to new buffer
            size_t idx = front_;
            for (size_t i = 0; i < size_; ++i) {
                new (new_data + i) T(std::move(data_[idx]));
                data_[idx].~T();
                idx = (idx + 1) % capacity_;
            }
            
            if (data_) {
                baba::core::aligned_free(data_);
            }
            
            data_ = new_data;
            front_ = 0;
            rear_ = size_;
            capacity_ = new_capacity;
        }
    }
}

template<typename T, size_t Alignment>
void queue<T, Alignment>::grow() {
    size_t new_capacity = capacity_ == 0 ? 4 : capacity_ * 2;
    reserve(new_capacity);
}

} // namespace baba::ds
