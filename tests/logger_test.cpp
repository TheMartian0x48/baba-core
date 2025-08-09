#include <gtest/gtest.h>
#include <baba/baba.hpp>



// Test the new namespace structure
TEST(BabaCoreTest, NamespaceStructure) {
    // Test baba::logger
    EXPECT_NO_THROW({
        baba::logger::info([]() { return "Test message"; });
    });

    // Test baba::ds::string
    EXPECT_NO_THROW({
        baba::ds::string str("Hello");
        str += " World";
        EXPECT_STREQ(str.c_str(), "Hello World");
        EXPECT_EQ(str.size(), 11);
        EXPECT_FALSE(str.empty());
    });

    // Test baba::ds::stack
    EXPECT_NO_THROW({
        baba::ds::stack<int> stack;
        EXPECT_TRUE(stack.empty());
        
        stack.push(1);
        stack.push(2);
        stack.push(3);
        
        EXPECT_FALSE(stack.empty());
        EXPECT_EQ(stack.size(), 3);
        EXPECT_EQ(stack.top(), 3);
        
        stack.pop();
        EXPECT_EQ(stack.top(), 2);
        EXPECT_EQ(stack.size(), 2);
    });

    // Test baba::ds::queue
    EXPECT_NO_THROW({
        baba::ds::queue<int> queue;
        EXPECT_TRUE(queue.empty());
        
        queue.push(10);
        queue.push(20);
        queue.push(30);
        
        EXPECT_FALSE(queue.empty());
        EXPECT_EQ(queue.size(), 3);
        EXPECT_EQ(queue.front(), 10);
        EXPECT_EQ(queue.back(), 30);
        
        queue.pop();
        EXPECT_EQ(queue.front(), 20);
        EXPECT_EQ(queue.size(), 2);
    });

    // Test baba::core memory utilities
    EXPECT_NO_THROW({
        void* ptr = baba::core::aligned_alloc(64, 16);
        EXPECT_NE(ptr, nullptr);
        EXPECT_TRUE(baba::core::is_aligned(ptr, 16));
        baba::core::aligned_free(ptr);
    });
}

// Test aligned string operations
TEST(AlignedStringTest, BasicOperations) {
    baba::ds::string str;
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0);
    
    str = "Hello";
    EXPECT_FALSE(str.empty());
    EXPECT_EQ(str.size(), 5);
    EXPECT_STREQ(str.c_str(), "Hello");
    
    str += " World!";
    EXPECT_EQ(str.size(), 12);
    EXPECT_STREQ(str.c_str(), "Hello World!");
    
    baba::ds::string str2 = str;
    EXPECT_EQ(str2.size(), str.size());
    EXPECT_STREQ(str2.c_str(), str.c_str());
}

// Test stack operations
TEST(StackTest, BasicOperations) {
    baba::ds::stack<std::string> stack;
    
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push("first");
    stack.push("second");
    stack.push("third");
    
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), "third");
    
    stack.pop();
    EXPECT_EQ(stack.top(), "second");
    EXPECT_EQ(stack.size(), 2);
    
    stack.clear();
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}

// Test queue operations
TEST(QueueTest, BasicOperations) {
    baba::ds::queue<std::string> queue;
    
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    
    queue.push("first");
    queue.push("second");
    queue.push("third");
    
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), "first");
    EXPECT_EQ(queue.back(), "third");
    
    queue.pop();
    EXPECT_EQ(queue.front(), "second");
    EXPECT_EQ(queue.size(), 2);
    
    queue.clear();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}

// Test memory alignment
TEST(MemoryTest, AlignedAllocation) {
    // Test different alignments
    for (size_t alignment : {16, 32, 64}) {
        void* ptr = baba::core::aligned_alloc(100, alignment);
        EXPECT_NE(ptr, nullptr);
        EXPECT_TRUE(baba::core::is_aligned(ptr, alignment));
        baba::core::aligned_free(ptr);
    }
    
    // Test null pointer handling
    baba::core::aligned_free(nullptr); // Should not crash
    
    // Test zero size
    void* ptr = baba::core::aligned_alloc(0, 16);
    EXPECT_EQ(ptr, nullptr);
}

