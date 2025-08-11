#include <baba/baba.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace baba::alias;
using namespace baba::memory;
using namespace baba::ds::array;

class ArrayI32Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        arena          = create_linear_arena();
        arena.instance = linear_arena_init(1024 * 1024); // 1MB arena
    }

    void TearDown() override
    {
        if (arena.instance) {
            arena.destroy(arena.instance);
        }
    }

    Arena arena;
};

// Basic Operations Tests
TEST_F(ArrayI32Test, BasicOperations)
{
    auto arr = baba::ds::array::make<i32>(&arena, 3);

    EXPECT_EQ(arr.get_size(), 3);
    EXPECT_EQ(arr.get_capacity(), 3);
    EXPECT_FALSE(arr.empty());
    EXPECT_NE(arr.get_data(), nullptr);
}

TEST_F(ArrayI32Test, MakeWithCapacity)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 5, 15);

    EXPECT_EQ(arr.get_size(), 5);
    EXPECT_EQ(arr.get_capacity(), 15);
    EXPECT_FALSE(arr.empty());
}

// Append and Access Tests
TEST_F(ArrayI32Test, AppendAndAccess)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 10);

    arr.append(42);
    arr.append(-17);
    arr.append(100);

    EXPECT_EQ(arr.get_size(), 3);
    EXPECT_EQ(arr.at(0), 42);
    EXPECT_EQ(arr.at(1), -17);
    EXPECT_EQ(arr.at(2), 100);

    EXPECT_EQ(arr.begin(), 42);
    EXPECT_EQ(arr.end(), 100);
}

TEST_F(ArrayI32Test, SafeOperations)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 2);

    arr.append_safe(10);
    arr.append_safe(20);

    EXPECT_THROW(arr.append_safe(30), std::out_of_range);
    EXPECT_THROW(arr.at_safe(5), std::out_of_range);

    EXPECT_EQ(arr.at_safe(0), 10);
    EXPECT_EQ(arr.at_safe(1), 20);
}

TEST_F(ArrayI32Test, BeginEndSafe)
{
    auto empty_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);

    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    arr.append(123);
    arr.append(456);

    EXPECT_EQ(arr.begin_safe(), 123);
    EXPECT_EQ(arr.end_safe(), 456);
}

// Update and Remove Tests
TEST_F(ArrayI32Test, UpdateAndRemove)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);

    arr.append(1);
    arr.append(2);
    arr.append(3);

    arr.update(1, 99);
    EXPECT_EQ(arr.at(1), 99);

    arr.update_safe(0, -50);
    EXPECT_EQ(arr.at(0), -50);

    EXPECT_THROW(arr.update_safe(10, 999), std::out_of_range);

    arr.remove_safe(0);
    EXPECT_EQ(arr.get_size(), 2);
    EXPECT_EQ(arr.at(0), 99);
    EXPECT_EQ(arr.at(1), 3);

    arr.remove(0);
    EXPECT_EQ(arr.get_size(), 1);
    EXPECT_EQ(arr.at(0), 3);

    EXPECT_THROW(arr.remove_safe(5), std::out_of_range);
}

// Fill Operations Tests
TEST_F(ArrayI32Test, FillOperations)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 4);
    arr.append(0);
    arr.append(0);
    arr.append(0);
    arr.append(0);

    arr.fill(42);
    for (u32 i = 0; i < arr.get_size(); ++i) {
        EXPECT_EQ(arr.at(i), 42);
    }

    arr.fill_range_safe(1, 3, -1);
    EXPECT_EQ(arr.at(0), 42);
    EXPECT_EQ(arr.at(1), -1);
    EXPECT_EQ(arr.at(2), -1);
    EXPECT_EQ(arr.at(3), 42);

    arr.fill_range(0, 2, 999);
    EXPECT_EQ(arr.at(0), 999);
    EXPECT_EQ(arr.at(1), 999);
    EXPECT_EQ(arr.at(2), -1);

    EXPECT_THROW(arr.fill_range_safe(0, 10, 123), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(5, 2, 123), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(2, 2, 123), std::out_of_range);
}

// Comparison Operations Tests
TEST_F(ArrayI32Test, ComparisonOperations)
{
    auto arr1 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    auto arr2 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);

    arr1.append(1);
    arr1.append(2);
    arr1.append(3);

    arr2.append(1);
    arr2.append(2);
    arr2.append(3);
    // testing for equality
    EXPECT_TRUE(arr1.equals(&arr2));
    EXPECT_EQ(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_FALSE(arr1.greater_than(&arr2));
    // testing for less than
    arr2.update(2, 4);
    EXPECT_FALSE(arr1.equals(&arr2));
    EXPECT_LT(arr1.compare(&arr2), 0);
    EXPECT_TRUE(arr1.less_than(&arr2));
    EXPECT_FALSE(arr1.greater_than(&arr2));
    //testing for greater than
    arr1.update(0, 5);
    EXPECT_FALSE(arr1.equals(&arr2));
    EXPECT_GT(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_TRUE(arr1.greater_than(&arr2));

    // Test with different sizes
    arr1.append(10);
    EXPECT_GT(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_TRUE(arr1.greater_than(&arr2));
}

// Prefix/Suffix Operations Tests
TEST_F(ArrayI32Test, PrefixSuffixOperations)
{
    auto arr    = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    auto prefix = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);
    auto suffix = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);

    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append(5);

    prefix.append(1);
    prefix.append(2);

    suffix.append(4);
    suffix.append(5);

    EXPECT_TRUE(arr.starts_with(&prefix));
    EXPECT_TRUE(arr.ends_with(&suffix));

    prefix.update(1, 99);
    EXPECT_FALSE(arr.starts_with(&prefix));

    // Test edge cases
    auto empty_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    EXPECT_FALSE(arr.starts_with(nullptr));
    EXPECT_TRUE(arr.starts_with(&empty_arr)); // empty prefix should match
    EXPECT_TRUE(arr.ends_with(&empty_arr));   // empty suffix should match

    auto larger_prefix = baba::ds::array::make_with_capacity<i32>(&arena, 0, 10);
    for (int i = 0; i < 8; i++) {
        larger_prefix.append(i);
    }
    EXPECT_FALSE(arr.starts_with(&larger_prefix)); // larger than array

    // Test same size arrays
    auto same_size = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    same_size.append(1);
    same_size.append(2);
    same_size.append(3);
    same_size.append(4);
    same_size.append(5);
    EXPECT_TRUE(arr.starts_with(&same_size));
    EXPECT_TRUE(arr.ends_with(&same_size));
}

// Sorted Operations Tests
TEST_F(ArrayI32Test, SortedOperations)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append(5);

    EXPECT_TRUE(arr.sorted());
    EXPECT_FALSE(arr.sorted_reverse());

    // Test reverse sorted
    auto rev_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    rev_arr.append(10);
    rev_arr.append(8);
    rev_arr.append(5);
    rev_arr.append(3);
    rev_arr.append(1);

    EXPECT_FALSE(rev_arr.sorted());
    EXPECT_TRUE(rev_arr.sorted_reverse());

    // Test unsorted array
    arr.update(2, 0);
    EXPECT_FALSE(arr.sorted());
    EXPECT_FALSE(arr.sorted_reverse());

    // Test single element and empty
    auto single_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    single_arr.append(42);
    EXPECT_TRUE(single_arr.sorted());
    EXPECT_TRUE(single_arr.sorted_reverse());

    auto empty_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    EXPECT_TRUE(empty_arr.sorted());
    EXPECT_TRUE(empty_arr.sorted_reverse());

    // Test with duplicates
    auto dup_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    dup_arr.append(1);
    dup_arr.append(2);
    dup_arr.append(2);
    dup_arr.append(3);
    dup_arr.append(3);
    EXPECT_TRUE(dup_arr.sorted());

    auto rev_dup_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    rev_dup_arr.append(5);
    rev_dup_arr.append(3);
    rev_dup_arr.append(3);
    rev_dup_arr.append(1);
    rev_dup_arr.append(1);
    EXPECT_TRUE(rev_dup_arr.sorted_reverse());
}

// Range-based Sorted Operations Tests
TEST_F(ArrayI32Test, SortedRangeOperations)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 10);
    arr.append(5); // 0
    arr.append(1); // 1 - start of sorted range
    arr.append(2); // 2
    arr.append(4); // 3
    arr.append(6); // 4 - end of sorted range
    arr.append(3); // 5
    arr.append(8); // 6

    EXPECT_TRUE(arr.sorted_range(1, 5));  // 1, 2, 4, 6
    EXPECT_FALSE(arr.sorted_range(0, 4)); // 5, 1, 2, 4
    EXPECT_TRUE(arr.sorted_range(2, 5));  // 2, 4, 6
    EXPECT_TRUE(arr.sorted_range(3, 4));  // single element: 4
    EXPECT_TRUE(arr.sorted_range(3, 3));  // empty range

    // Test reverse sorted range
    auto rev_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 10);
    rev_arr.append(1);  // 0
    rev_arr.append(10); // 1 - start of reverse sorted range
    rev_arr.append(8);  // 2
    rev_arr.append(5);  // 3
    rev_arr.append(2);  // 4 - end of reverse sorted range
    rev_arr.append(7);  // 5

    EXPECT_TRUE(rev_arr.sorted_range_reverse(1, 5));  // 10, 8, 5, 2
    EXPECT_FALSE(rev_arr.sorted_range_reverse(0, 4)); // 1, 10, 8, 5
    EXPECT_TRUE(rev_arr.sorted_range_reverse(2, 5));  // 8, 5, 2

    // Test with duplicates in range
    auto dup_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 8);
    dup_arr.append(9); // 0
    dup_arr.append(1); // 1
    dup_arr.append(2); // 2
    dup_arr.append(2); // 3
    dup_arr.append(3); // 4
    dup_arr.append(3); // 5
    dup_arr.append(0); // 6

    EXPECT_TRUE(dup_arr.sorted_range(1, 6));  // 1, 2, 2, 3, 3
    EXPECT_FALSE(dup_arr.sorted_range(0, 7)); // includes 9 at start and 0 at end

    // Test negative numbers
    auto neg_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    neg_arr.append(0);
    neg_arr.append(-5);
    neg_arr.append(-3);
    neg_arr.append(-1);
    neg_arr.append(2);

    EXPECT_TRUE(neg_arr.sorted_range(1, 5));  // -5, -3, -1, 2
    EXPECT_FALSE(neg_arr.sorted_range(0, 5)); // includes 0 at start
}

// Search Operations Tests
TEST_F(ArrayI32Test, SearchOperations)
{
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    arr.append(10);
    arr.append(20);
    arr.append(10);
    arr.append(30);
    arr.append(10);

    EXPECT_EQ(arr.find(10), 0);
    EXPECT_EQ(arr.find_reverse(10), 4);
    EXPECT_EQ(arr.find(99), -1);
    EXPECT_TRUE(arr.contains(20));
    EXPECT_FALSE(arr.contains(99));
    EXPECT_EQ(arr.counts(10), 3);
    EXPECT_EQ(arr.counts(20), 1);
    EXPECT_EQ(arr.counts(99), 0);

    // Test with negative numbers
    auto neg_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    neg_arr.append(-5);
    neg_arr.append(0);
    neg_arr.append(-5);
    neg_arr.append(10);

    EXPECT_EQ(neg_arr.find(-5), 0);
    EXPECT_EQ(neg_arr.find_reverse(-5), 2);
    EXPECT_EQ(neg_arr.find(0), 1);
    EXPECT_TRUE(neg_arr.contains(-5));
    EXPECT_TRUE(neg_arr.contains(0));
    EXPECT_FALSE(neg_arr.contains(-999));
    EXPECT_EQ(neg_arr.counts(-5), 2);
    EXPECT_EQ(neg_arr.counts(0), 1);

    // Test with all same values
    auto same_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    same_arr.append(42);
    same_arr.append(42);
    same_arr.append(42);

    EXPECT_EQ(same_arr.find(42), 0);
    EXPECT_EQ(same_arr.find_reverse(42), 2);
    EXPECT_EQ(same_arr.counts(42), 3);
}

// Utility Functions Tests
TEST_F(ArrayI32Test, UtilityFunctions)
{
    auto original = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    original.append(1);
    original.append(2);
    original.append(3);

    // Test copy
    auto copied = baba::ds::array::copy(&arena, &original);
    EXPECT_EQ(copied.get_size(), original.get_size());
    EXPECT_TRUE(copied.equals(&original));

    copied.update(0, 99);
    EXPECT_FALSE(copied.equals(&original));

    // Test merge
    auto arr1 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);
    auto arr2 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);

    arr1.append(1);
    arr1.append(2);

    arr2.append(3);
    arr2.append(4);

    auto merged = baba::ds::array::merge(&arena, &arr1, &arr2);
    EXPECT_EQ(merged.get_size(), 4);
    EXPECT_EQ(merged.at(0), 1);
    EXPECT_EQ(merged.at(1), 2);
    EXPECT_EQ(merged.at(2), 3);
    EXPECT_EQ(merged.at(3), 4);

    // Test split
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 6);
    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append(5);

    auto [left, right] = baba::ds::array::split(&arr, 2);
    EXPECT_EQ(left.get_size(), 3);
    EXPECT_EQ(right.get_size(), 2);
    EXPECT_EQ(left.at(0), 1);
    EXPECT_EQ(left.at(1), 2);
    EXPECT_EQ(left.at(2), 3);
    EXPECT_EQ(right.at(0), 4);
    EXPECT_EQ(right.at(1), 5);

    // Test split edge cases
    auto [left_edge, right_edge] = baba::ds::array::split(&arr, 0);
    EXPECT_EQ(left_edge.get_size(), 1);
    EXPECT_EQ(left_edge.at(0), 1);
    EXPECT_EQ(right_edge.get_size(), 4);
    EXPECT_EQ(right_edge.at(0), 2);
    EXPECT_EQ(right_edge.at(1), 3);
    EXPECT_EQ(right_edge.at(2), 4);
    EXPECT_EQ(right_edge.at(3), 5);

    auto [left_end, right_end] = baba::ds::array::split(&arr, 4);
    EXPECT_EQ(left_end.get_size(), 5);
    EXPECT_EQ(left_end.at(0), 1);
    EXPECT_EQ(left_end.at(1), 2);
    EXPECT_EQ(left_end.at(2), 3);
    EXPECT_EQ(left_end.at(3), 4);
    EXPECT_EQ(left_end.at(4), 5);
    EXPECT_EQ(right_end.get_size(), 0);

    // Test swap
    auto swap1 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);
    auto swap2 = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);

    swap1.append(1);
    swap1.append(2);

    swap2.append(3);
    swap2.append(4);
    swap2.append(5);

    u32 original_size1 = swap1.get_size();
    u32 original_size2 = swap2.get_size();

    baba::ds::array::swap(&swap1, &swap2);

    EXPECT_EQ(swap1.get_size(), original_size2);
    EXPECT_EQ(swap1.at(0), 3);
    EXPECT_EQ(swap1.at(1), 4);
    EXPECT_EQ(swap1.at(2), 5);
    EXPECT_EQ(swap2.get_size(), original_size1);
    EXPECT_EQ(swap2.at(0), 1);
    EXPECT_EQ(swap2.at(1), 2);
}

// Edge Cases Tests
TEST_F(ArrayI32Test, EdgeCases)
{
    // Empty array operations
    auto empty_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);

    EXPECT_TRUE(empty_arr.empty());
    EXPECT_EQ(empty_arr.get_size(), 0);
    EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.remove_safe(0), std::out_of_range);
    EXPECT_EQ(empty_arr.find(42), -1);
    EXPECT_FALSE(empty_arr.contains(42));
    EXPECT_EQ(empty_arr.counts(42), 0);

    // Single element array
    auto single_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    single_arr.append(42);

    EXPECT_FALSE(single_arr.empty());
    EXPECT_EQ(single_arr.get_size(), 1);
    EXPECT_EQ(single_arr.begin(), 42);
    EXPECT_EQ(single_arr.end(), 42);
    EXPECT_EQ(single_arr.begin_safe(), 42);
    EXPECT_EQ(single_arr.end_safe(), 42);

    single_arr.remove(0);
    EXPECT_TRUE(single_arr.empty());

    // Boundary conditions
    auto arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 3);

    // Fill to capacity
    arr.append_safe(1);
    arr.append_safe(2);
    arr.append_safe(3);

    EXPECT_THROW(arr.append_safe(4), std::out_of_range);

    // Test range operations at boundaries
    EXPECT_THROW(arr.fill_range_safe(0, 4, 99), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(3, 4, 99), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(2, 1, 99), std::out_of_range);

    // Test with extreme values
    auto extreme_arr = baba::ds::array::make_with_capacity<i32>(&arena, 0, 5);
    extreme_arr.append(INT32_MAX);
    extreme_arr.append(INT32_MIN);
    extreme_arr.append(0);

    EXPECT_EQ(extreme_arr.find(INT32_MAX), 0);
    EXPECT_EQ(extreme_arr.find(INT32_MIN), 1);
    EXPECT_TRUE(extreme_arr.contains(0));
    EXPECT_FALSE(extreme_arr.sorted());
}
