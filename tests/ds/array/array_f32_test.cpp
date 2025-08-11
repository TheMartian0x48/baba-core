#include <baba/baba.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace baba::alias;
using namespace baba::memory;
using namespace baba::ds::array;

class ArrayF32Test : public ::testing::Test
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
TEST_F(ArrayF32Test, BasicOperations)
{
    auto arr = baba::ds::array::make<f32>(&arena, 3);

    EXPECT_EQ(arr.get_size(), 3);
    EXPECT_EQ(arr.get_capacity(), 3);
    EXPECT_FALSE(arr.empty());
    EXPECT_NE(arr.get_data(), nullptr);
}

TEST_F(ArrayF32Test, MakeWithCapacity)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 4, 12);

    EXPECT_EQ(arr.get_size(), 4);
    EXPECT_EQ(arr.get_capacity(), 12);
    EXPECT_FALSE(arr.empty());
}

// Append and Access Tests
TEST_F(ArrayF32Test, AppendAndAccess)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 10);

    arr.append(3.14f);
    arr.append(-2.5f);
    arr.append(0.0f);

    EXPECT_EQ(arr.get_size(), 3);
    EXPECT_FLOAT_EQ(arr.at(0), 3.14f);
    EXPECT_FLOAT_EQ(arr.at(1), -2.5f);
    EXPECT_FLOAT_EQ(arr.at(2), 0.0f);

    EXPECT_FLOAT_EQ(arr.begin(), 3.14f);
    EXPECT_FLOAT_EQ(arr.end(), 0.0f);
}

TEST_F(ArrayF32Test, SafeOperations)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 2);

    arr.append_safe(1.5f);
    arr.append_safe(2.5f);

    EXPECT_THROW(arr.append_safe(3.5f), std::out_of_range);
    EXPECT_THROW(arr.at_safe(5), std::out_of_range);

    EXPECT_FLOAT_EQ(arr.at_safe(0), 1.5f);
    EXPECT_FLOAT_EQ(arr.at_safe(1), 2.5f);
}

TEST_F(ArrayF32Test, BeginEndSafe)
{
    auto empty_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);

    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    arr.append(1.23f);
    arr.append(4.56f);

    EXPECT_FLOAT_EQ(arr.begin_safe(), 1.23f);
    EXPECT_FLOAT_EQ(arr.end_safe(), 4.56f);
}

// Update and Remove Tests
TEST_F(ArrayF32Test, UpdateAndRemove)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);

    arr.append(1.1f);
    arr.append(2.2f);
    arr.append(3.3f);

    arr.update(1, 9.9f);
    EXPECT_FLOAT_EQ(arr.at(1), 9.9f);

    arr.update_safe(0, -5.5f);
    EXPECT_FLOAT_EQ(arr.at(0), -5.5f);

    EXPECT_THROW(arr.update_safe(10, 99.9f), std::out_of_range);

    arr.remove_safe(0);
    EXPECT_EQ(arr.get_size(), 2);
    EXPECT_FLOAT_EQ(arr.at(0), 9.9f);
    EXPECT_FLOAT_EQ(arr.at(1), 3.3f);

    arr.remove(0);
    EXPECT_EQ(arr.get_size(), 1);
    EXPECT_FLOAT_EQ(arr.at(0), 3.3f);

    EXPECT_THROW(arr.remove_safe(5), std::out_of_range);
}

// Fill Operations Tests
TEST_F(ArrayF32Test, FillOperations)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 4);
    arr.append(0.0f);
    arr.append(0.0f);
    arr.append(0.0f);
    arr.append(0.0f);

    arr.fill(4.2f);
    for (u32 i = 0; i < arr.get_size(); ++i) {
        EXPECT_FLOAT_EQ(arr.at(i), 4.2f);
    }

    arr.fill_range_safe(1, 3, -1.1f);
    EXPECT_FLOAT_EQ(arr.at(0), 4.2f);
    EXPECT_FLOAT_EQ(arr.at(1), -1.1f);
    EXPECT_FLOAT_EQ(arr.at(2), -1.1f);
    EXPECT_FLOAT_EQ(arr.at(3), 4.2f);

    arr.fill_range(0, 2, 99.9f);
    EXPECT_FLOAT_EQ(arr.at(0), 99.9f);
    EXPECT_FLOAT_EQ(arr.at(1), 99.9f);
    EXPECT_FLOAT_EQ(arr.at(2), -1.1f);

    EXPECT_THROW(arr.fill_range_safe(0, 10, 1.23f), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(5, 2, 1.23f), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(2, 2, 1.23f), std::out_of_range);

    // Test with special float values
    arr.fill(INFINITY);
    EXPECT_TRUE(std::isinf(arr.at(0)));

    arr.fill(-INFINITY);
    EXPECT_TRUE(std::isinf(arr.at(0)) && arr.at(0) < 0);

    arr.fill(NAN);
    EXPECT_TRUE(std::isnan(arr.at(0)));
}

// Comparison Operations Tests
TEST_F(ArrayF32Test, ComparisonOperations)
{
    auto arr1 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);
    auto arr2 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);

    arr1.append(1.0f);
    arr1.append(2.0f);

    arr2.append(1.0f);
    arr2.append(2.0f);

    EXPECT_TRUE(arr1.equals(&arr2));
    EXPECT_EQ(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_FALSE(arr1.greater_than(&arr2));

    arr2.update(1, 3.0f);
    EXPECT_FALSE(arr1.equals(&arr2));
    EXPECT_LT(arr1.compare(&arr2), 0);
    EXPECT_TRUE(arr1.less_than(&arr2));
    EXPECT_FALSE(arr1.greater_than(&arr2));

    arr1.update(0, 5.0f);
    EXPECT_GT(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_TRUE(arr1.greater_than(&arr2));

    // Test with different sizes
    arr1.append(10.0f);
    EXPECT_GT(arr1.compare(&arr2), 0);
    EXPECT_FALSE(arr1.less_than(&arr2));
    EXPECT_TRUE(arr1.greater_than(&arr2));

    // Test with very close values
    auto close1 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);
    auto close2 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);

    close1.append(1.0000001f);
    close2.append(1.0000002f);

    EXPECT_FALSE(close1.equals(&close2));
    EXPECT_LT(close1.compare(&close2), 0);
}

// Prefix/Suffix Operations Tests
TEST_F(ArrayF32Test, PrefixSuffixOperations)
{
    auto arr    = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    auto prefix = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);
    auto suffix = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);

    arr.append(1.1f);
    arr.append(2.2f);
    arr.append(3.3f);
    arr.append(4.4f);
    arr.append(5.5f);

    prefix.append(1.1f);
    prefix.append(2.2f);

    suffix.append(4.4f);
    suffix.append(5.5f);

    EXPECT_TRUE(arr.starts_with(&prefix));
    EXPECT_TRUE(arr.ends_with(&suffix));

    prefix.update(1, 9.9f);
    EXPECT_FALSE(arr.starts_with(&prefix));

    // Test edge cases
    auto empty_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    EXPECT_FALSE(arr.starts_with(nullptr));
    EXPECT_TRUE(arr.starts_with(&empty_arr)); // empty prefix should match
    EXPECT_TRUE(arr.ends_with(&empty_arr));   // empty suffix should match

    auto larger_prefix = baba::ds::array::make_with_capacity<f32>(&arena, 0, 10);
    for (int i = 0; i < 8; i++) {
        larger_prefix.append(static_cast<f32>(i) + 0.5f);
    }
    EXPECT_FALSE(arr.starts_with(&larger_prefix)); // larger than array

    // Test same size arrays
    auto same_size = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    same_size.append(1.1f);
    same_size.append(2.2f);
    same_size.append(3.3f);
    same_size.append(4.4f);
    same_size.append(5.5f);
    EXPECT_TRUE(arr.starts_with(&same_size));
    EXPECT_TRUE(arr.ends_with(&same_size));
}

// Sorted Operations Tests
TEST_F(ArrayF32Test, SortedOperations)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    arr.append(1.1f);
    arr.append(2.2f);
    arr.append(3.3f);
    arr.append(4.4f);
    arr.append(5.5f);

    EXPECT_TRUE(arr.sorted());
    EXPECT_FALSE(arr.sorted_reverse());

    // Test reverse sorted
    auto rev_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    rev_arr.append(10.5f);
    rev_arr.append(8.3f);
    rev_arr.append(5.1f);
    rev_arr.append(3.7f);
    rev_arr.append(1.2f);

    EXPECT_FALSE(rev_arr.sorted());
    EXPECT_TRUE(rev_arr.sorted_reverse());

    // Test unsorted array
    arr.update(2, 0.5f);
    EXPECT_FALSE(arr.sorted());
    EXPECT_FALSE(arr.sorted_reverse());

    // Test single element and empty
    auto single_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    single_arr.append(4.2f);
    EXPECT_TRUE(single_arr.sorted());
    EXPECT_TRUE(single_arr.sorted_reverse());

    auto empty_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    EXPECT_TRUE(empty_arr.sorted());
    EXPECT_TRUE(empty_arr.sorted_reverse());

    // Test with duplicates
    auto dup_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    dup_arr.append(1.5f);
    dup_arr.append(2.5f);
    dup_arr.append(2.5f);
    dup_arr.append(3.5f);
    dup_arr.append(3.5f);
    EXPECT_TRUE(dup_arr.sorted());

    auto rev_dup_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    rev_dup_arr.append(5.5f);
    rev_dup_arr.append(3.3f);
    rev_dup_arr.append(3.3f);
    rev_dup_arr.append(1.1f);
    rev_dup_arr.append(1.1f);
    EXPECT_TRUE(rev_dup_arr.sorted_reverse());

    // Test with negative numbers
    auto neg_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    neg_arr.append(-5.5f);
    neg_arr.append(-2.2f);
    neg_arr.append(0.0f);
    neg_arr.append(3.3f);
    EXPECT_TRUE(neg_arr.sorted());
}

// Range-based Sorted Operations Tests
TEST_F(ArrayF32Test, SortedRangeOperations)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 10);
    arr.append(5.5f); // 0
    arr.append(1.1f); // 1 - start of sorted range
    arr.append(2.2f); // 2
    arr.append(4.4f); // 3
    arr.append(6.6f); // 4 - end of sorted range
    arr.append(3.3f); // 5
    arr.append(8.8f); // 6

    EXPECT_TRUE(arr.sorted_range(1, 5));  // 1.1, 2.2, 4.4, 6.6
    EXPECT_FALSE(arr.sorted_range(0, 4)); // 5.5, 1.1, 2.2, 4.4
    EXPECT_TRUE(arr.sorted_range(2, 5));  // 2.2, 4.4, 6.6
    EXPECT_TRUE(arr.sorted_range(3, 4));  // single element: 4.4
    EXPECT_TRUE(arr.sorted_range(3, 3));  // empty range

    // Test reverse sorted range
    auto rev_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 10);
    rev_arr.append(1.1f);  // 0
    rev_arr.append(10.5f); // 1 - start of reverse sorted range
    rev_arr.append(8.3f);  // 2
    rev_arr.append(5.7f);  // 3
    rev_arr.append(2.9f);  // 4 - end of reverse sorted range
    rev_arr.append(7.1f);  // 5

    EXPECT_TRUE(rev_arr.sorted_range_reverse(1, 5));  // 10.5, 8.3, 5.7, 2.9
    EXPECT_FALSE(rev_arr.sorted_range_reverse(0, 4)); // 1.1, 10.5, 8.3, 5.7
    EXPECT_TRUE(rev_arr.sorted_range_reverse(2, 5));  // 8.3, 5.7, 2.9

    // Test with duplicates in range
    auto dup_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 8);
    dup_arr.append(9.9f); // 0
    dup_arr.append(1.1f); // 1
    dup_arr.append(2.2f); // 2
    dup_arr.append(2.2f); // 3
    dup_arr.append(3.3f); // 4
    dup_arr.append(3.3f); // 5
    dup_arr.append(0.5f); // 6

    EXPECT_TRUE(dup_arr.sorted_range(1, 6));  // 1.1, 2.2, 2.2, 3.3, 3.3
    EXPECT_FALSE(dup_arr.sorted_range(0, 7)); // includes 9.9 at start and 0.5 at end

    // Test negative numbers
    auto neg_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    neg_arr.append(0.0f);
    neg_arr.append(-5.5f);
    neg_arr.append(-3.3f);
    neg_arr.append(-1.1f);
    neg_arr.append(2.2f);

    EXPECT_TRUE(neg_arr.sorted_range(1, 5));  // -5.5, -3.3, -1.1, 2.2
    EXPECT_FALSE(neg_arr.sorted_range(0, 5)); // includes 0.0 at start

    // Test with very close values
    auto close_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    close_arr.append(1.0000001f);
    close_arr.append(1.0000002f);
    close_arr.append(1.0000003f);
    close_arr.append(1.0000004f);

    EXPECT_TRUE(close_arr.sorted_range(0, 4));
}

// Search Operations Tests
TEST_F(ArrayF32Test, SearchOperations)
{
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    arr.append(1.1f);
    arr.append(2.2f);
    arr.append(1.1f);
    arr.append(3.3f);

    EXPECT_EQ(arr.find(1.1f), 0);
    EXPECT_EQ(arr.find_reverse(1.1f), 2);
    EXPECT_TRUE(arr.contains(2.2f));
    EXPECT_FALSE(arr.contains(9.9f));
    EXPECT_EQ(arr.counts(1.1f), 2);

    // Test with negative numbers
    auto neg_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    neg_arr.append(-5.5f);
    neg_arr.append(0.0f);
    neg_arr.append(-5.5f);
    neg_arr.append(10.1f);

    EXPECT_EQ(neg_arr.find(-5.5f), 0);
    EXPECT_EQ(neg_arr.find_reverse(-5.5f), 2);
    EXPECT_EQ(neg_arr.find(0.0f), 1);
    EXPECT_TRUE(neg_arr.contains(-5.5f));
    EXPECT_TRUE(neg_arr.contains(0.0f));
    EXPECT_FALSE(neg_arr.contains(-99.9f));
    EXPECT_EQ(neg_arr.counts(-5.5f), 2);
    EXPECT_EQ(neg_arr.counts(0.0f), 1);

    // Test with all same values
    auto same_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    same_arr.append(4.2f);
    same_arr.append(4.2f);
    same_arr.append(4.2f);

    EXPECT_EQ(same_arr.find(4.2f), 0);
    EXPECT_EQ(same_arr.find_reverse(4.2f), 2);
    EXPECT_EQ(same_arr.counts(4.2f), 3);

    // Test with special float values
    auto special_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    special_arr.append(INFINITY);
    special_arr.append(-INFINITY);
    special_arr.append(0.0f);

    EXPECT_EQ(special_arr.find(INFINITY), 0);
    EXPECT_EQ(special_arr.find(-INFINITY), 1);
    EXPECT_TRUE(special_arr.contains(0.0f));

    // Note: NaN comparisons are tricky - NaN != NaN
    special_arr.append(NAN);
    EXPECT_EQ(special_arr.find(NAN), -1); // NaN != NaN, so it won't be found
}

// Utility Functions Tests
TEST_F(ArrayF32Test, UtilityFunctions)
{
    auto original = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    original.append(1.1f);
    original.append(2.2f);
    original.append(3.3f);

    // Test copy
    auto copied = baba::ds::array::copy(&arena, &original);
    EXPECT_EQ(copied.get_size(), original.get_size());
    EXPECT_TRUE(copied.equals(&original));

    copied.update(0, 9.9f);
    EXPECT_FALSE(copied.equals(&original));

    // Test merge
    auto arr1 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);
    auto arr2 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);

    arr1.append(1.1f);
    arr1.append(2.2f);

    arr2.append(3.3f);
    arr2.append(4.4f);

    auto merged = baba::ds::array::merge(&arena, &arr1, &arr2);
    EXPECT_EQ(merged.get_size(), 4);
    EXPECT_FLOAT_EQ(merged.at(0), 1.1f);
    EXPECT_FLOAT_EQ(merged.at(1), 2.2f);
    EXPECT_FLOAT_EQ(merged.at(2), 3.3f);
    EXPECT_FLOAT_EQ(merged.at(3), 4.4f);

    // Test split
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 6);
    arr.append(1.1f);
    arr.append(2.2f);
    arr.append(3.3f);
    arr.append(4.4f);
    arr.append(5.5f);

    auto [left, right] = baba::ds::array::split(&arr, 2);
    EXPECT_EQ(left.get_size(), 3);
    EXPECT_EQ(right.get_size(), 2);
    EXPECT_FLOAT_EQ(left.at(0), 1.1f);
    EXPECT_FLOAT_EQ(left.at(1), 2.2f);
    EXPECT_FLOAT_EQ(left.at(2), 3.3f);
    EXPECT_FLOAT_EQ(right.at(0), 4.4f);
    EXPECT_FLOAT_EQ(right.at(1), 5.5f);

    // Test split edge cases
    auto [left_edge, right_edge] = baba::ds::array::split(&arr, 0);
    EXPECT_EQ(left_edge.get_size(), 1);
    EXPECT_EQ(right_edge.get_size(), 4);

    auto [left_end, right_end] = baba::ds::array::split(&arr, 4);
    EXPECT_EQ(left_end.get_size(), 5);
    EXPECT_EQ(right_end.get_size(), 0);

    // Test swap
    auto swap1 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);
    auto swap2 = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);

    swap1.append(1.1f);
    swap1.append(2.2f);

    swap2.append(3.3f);
    swap2.append(4.4f);
    swap2.append(5.5f);

    u32 original_size1 = swap1.get_size();
    u32 original_size2 = swap2.get_size();

    baba::ds::array::swap(&swap1, &swap2);

    EXPECT_EQ(swap1.get_size(), original_size2);
    EXPECT_EQ(swap2.get_size(), original_size1);
    EXPECT_FLOAT_EQ(swap1.at(0), 3.3f);
    EXPECT_FLOAT_EQ(swap1.at(1), 4.4f);
    EXPECT_FLOAT_EQ(swap1.at(2), 5.5f);
    EXPECT_FLOAT_EQ(swap2.at(0), 1.1f);
    EXPECT_FLOAT_EQ(swap2.at(1), 2.2f);
}

// Edge Cases Tests
TEST_F(ArrayF32Test, EdgeCases)
{
    // Empty array operations
    auto empty_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);

    EXPECT_TRUE(empty_arr.empty());
    EXPECT_EQ(empty_arr.get_size(), 0);
    EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
    EXPECT_THROW(empty_arr.remove_safe(0), std::out_of_range);
    EXPECT_EQ(empty_arr.find(4.2f), -1);
    EXPECT_FALSE(empty_arr.contains(4.2f));
    EXPECT_EQ(empty_arr.counts(4.2f), 0);

    // Single element array
    auto single_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    single_arr.append(4.2f);

    EXPECT_FALSE(single_arr.empty());
    EXPECT_EQ(single_arr.get_size(), 1);
    EXPECT_FLOAT_EQ(single_arr.begin(), 4.2f);
    EXPECT_FLOAT_EQ(single_arr.end(), 4.2f);
    EXPECT_FLOAT_EQ(single_arr.begin_safe(), 4.2f);
    EXPECT_FLOAT_EQ(single_arr.end_safe(), 4.2f);

    single_arr.remove(0);
    EXPECT_TRUE(single_arr.empty());

    // Boundary conditions
    auto arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 3);

    // Fill to capacity
    arr.append_safe(1.1f);
    arr.append_safe(2.2f);
    arr.append_safe(3.3f);

    EXPECT_THROW(arr.append_safe(4.4f), std::out_of_range);

    // Test range operations at boundaries
    EXPECT_THROW(arr.fill_range_safe(0, 4, 9.9f), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(3, 4, 9.9f), std::out_of_range);
    EXPECT_THROW(arr.fill_range_safe(2, 1, 9.9f), std::out_of_range);

    // Test with extreme float values
    auto extreme_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    extreme_arr.append(FLT_MAX);
    extreme_arr.append(FLT_MIN);
    extreme_arr.append(0.0f);
    extreme_arr.append(-0.0f);

    EXPECT_EQ(extreme_arr.find(FLT_MAX), 0);
    EXPECT_EQ(extreme_arr.find(FLT_MIN), 1);
    EXPECT_TRUE(extreme_arr.contains(0.0f));
    EXPECT_FALSE(extreme_arr.sorted());

    // Test precision edge cases
    auto precision_arr = baba::ds::array::make_with_capacity<f32>(&arena, 0, 5);
    precision_arr.append(1.0f);
    precision_arr.append(1.0f + FLT_EPSILON);
    precision_arr.append(1.0f + 2 * FLT_EPSILON);

    EXPECT_TRUE(precision_arr.sorted());
    EXPECT_TRUE(precision_arr.equals(&precision_arr)); // Array should equal itself
}
