#include <gtest/gtest.h>
#include <baba/string_utils.hpp>

using namespace baba::core::string_utils;

class StringUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test trim functions
TEST_F(StringUtilsTest, TrimFunctions) {
    EXPECT_EQ(trim("  hello  "), "hello");
    EXPECT_EQ(trim("hello"), "hello");
    EXPECT_EQ(trim(""), "");
    EXPECT_EQ(trim("   "), "");
    EXPECT_EQ(trim("\t\n hello \r\n\t"), "hello");
    
    EXPECT_EQ(ltrim("  hello  "), "hello  ");
    EXPECT_EQ(ltrim("hello  "), "hello  ");
    EXPECT_EQ(ltrim(""), "");
    
    EXPECT_EQ(rtrim("  hello  "), "  hello");
    EXPECT_EQ(rtrim("  hello"), "  hello");
    EXPECT_EQ(rtrim(""), "");
}

// Test split functions
TEST_F(StringUtilsTest, SplitByChar) {
    auto result = split("a,b,c", ',');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
    
    // Test empty string
    result = split("", ',');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "");
    
    // Test no delimiter
    result = split("hello", ',');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "hello");
    
    // Test consecutive delimiters
    result = split("a,,b", ',');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "b");
}

TEST_F(StringUtilsTest, SplitByString) {
    auto result = split("hello::world::test", "::");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
    EXPECT_EQ(result[2], "test");
    
    // Test empty delimiter
    result = split("hello", "");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "hello");
    
    // Test delimiter not found
    result = split("hello", "xyz");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "hello");
}

// Test join function
TEST_F(StringUtilsTest, JoinFunction) {
    std::vector<std::string> strings = {"a", "b", "c"};
    EXPECT_EQ(join(strings, ","), "a,b,c");
    EXPECT_EQ(join(strings, "::"), "a::b::c");
    
    // Test empty vector
    std::vector<std::string> empty;
    EXPECT_EQ(join(empty, ","), "");
    
    // Test single element
    std::vector<std::string> single = {"hello"};
    EXPECT_EQ(join(single, ","), "hello");
}

// Test case conversion
TEST_F(StringUtilsTest, CaseConversion) {
    EXPECT_EQ(to_upper("hello"), "HELLO");
    EXPECT_EQ(to_upper("Hello World"), "HELLO WORLD");
    EXPECT_EQ(to_upper(""), "");
    EXPECT_EQ(to_upper("123!@#"), "123!@#");
    
    EXPECT_EQ(to_lower("HELLO"), "hello");
    EXPECT_EQ(to_lower("Hello World"), "hello world");
    EXPECT_EQ(to_lower(""), "");
    EXPECT_EQ(to_lower("123!@#"), "123!@#");
}

// Test pattern matching
TEST_F(StringUtilsTest, PatternMatching) {
    // starts_with tests
    EXPECT_TRUE(starts_with("hello world", "hello"));
    EXPECT_TRUE(starts_with("hello", "hello"));
    EXPECT_FALSE(starts_with("hello", "world"));
    EXPECT_FALSE(starts_with("hi", "hello"));
    EXPECT_TRUE(starts_with("", ""));
    EXPECT_FALSE(starts_with("", "hello"));
    
    // ends_with tests
    EXPECT_TRUE(ends_with("hello world", "world"));
    EXPECT_TRUE(ends_with("world", "world"));
    EXPECT_FALSE(ends_with("hello", "world"));
    EXPECT_FALSE(ends_with("hi", "hello"));
    EXPECT_TRUE(ends_with("", ""));
    EXPECT_FALSE(ends_with("", "hello"));
    
    // contains tests
    EXPECT_TRUE(contains("hello world", "lo wo"));
    EXPECT_TRUE(contains("hello", "hello"));
    EXPECT_FALSE(contains("hello", "xyz"));
    EXPECT_TRUE(contains("", ""));
    EXPECT_FALSE(contains("", "hello"));
}

// Test replace_all function
TEST_F(StringUtilsTest, ReplaceAll) {
    EXPECT_EQ(replace_all("hello world hello", "hello", "hi"), "hi world hi");
    EXPECT_EQ(replace_all("aaa", "a", "bb"), "bbbbbb");
    EXPECT_EQ(replace_all("hello", "xyz", "abc"), "hello");
    EXPECT_EQ(replace_all("", "a", "b"), "");
    EXPECT_EQ(replace_all("hello", "", "x"), "hello");
    
    // Test overlapping replacements
    EXPECT_EQ(replace_all("aaaa", "aa", "b"), "bb");
}

// Test format function
TEST_F(StringUtilsTest, FormatFunction) {
    EXPECT_EQ(format("Hello %s", "world"), "Hello world");
    EXPECT_EQ(format("Number: %d", 42), "Number: 42");
    EXPECT_EQ(format("Float: %.2f", 3.14159), "Float: 3.14");
    EXPECT_EQ(format("Multiple: %s %d %.1f", "test", 123, 4.5), "Multiple: test 123 4.5");
    EXPECT_EQ(format("No args"), "No args");
}

// Edge cases and error handling
TEST_F(StringUtilsTest, EdgeCases) {
    // Test with special characters
    EXPECT_EQ(trim("\x00\x01\x02  hello  \x7F"), "hello");
    
    // Test with very long strings
    std::string long_string(10000, 'a');
    EXPECT_EQ(to_upper(long_string), std::string(10000, 'A'));
    
    // Test split with long delimiter
    std::string long_delimiter(100, 'x');
    auto result = split("hello" + long_delimiter + "world", long_delimiter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
}

// Performance-related tests
TEST_F(StringUtilsTest, PerformanceEdgeCases) {
    // Test with many splits
    std::string many_parts;
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) many_parts += ",";
        many_parts += std::to_string(i);
    }
    
    auto result = split(many_parts, ',');
    EXPECT_EQ(result.size(), 1000);
    EXPECT_EQ(result[0], "0");
    EXPECT_EQ(result[999], "999");
}
