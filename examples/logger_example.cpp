#include <baba/logger.hpp>
#include <baba/ds.hpp>
#include <baba/core.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::cout << "=== Baba-Core Library Example ===" << std::endl << std::endl;

    // Logger example (new namespace: baba::logger)
    std::cout << "=== Logger Example ===" << std::endl;
    baba::logger::info([]() { return "Hello from baba::logger!"; });
    std::cout << std::endl;

    // Data structures example (new namespace: baba::ds)
    std::cout << "=== Data Structures Example ===" << std::endl;
    
    // Aligned string
    baba::ds::string str("Hello");
    str += " World!";
    std::cout << "Aligned string: " << str.c_str() << std::endl;
    
    // Stack
    baba::ds::stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::cout << "Stack top: " << stack.top() << std::endl;
    
    // Queue
    baba::ds::queue<int> queue;
    queue.push(10);
    queue.push(20);
    queue.push(30);
    std::cout << "Queue front: " << queue.front() << std::endl;
    std::cout << std::endl;

    // Core utilities example (new namespace: baba::core)
    std::cout << "=== Core Utilities Example ===" << std::endl;
    void* aligned_ptr = baba::core::aligned_alloc(64, 16);
    if (aligned_ptr) {
        std::cout << "Allocated 64 bytes with 16-byte alignment" << std::endl;
        std::cout << "Pointer is aligned: " << baba::core::is_aligned(aligned_ptr, 16) << std::endl;
        baba::core::aligned_free(aligned_ptr);
    }

    return 0;
}
