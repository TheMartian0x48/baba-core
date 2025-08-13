#include <baba/baba.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace baba::alias;
using namespace baba::memory;
// using namespace baba::ds::array;

// class ArrayBoolTest : public ::testing::Test
// {
//   protected:
//     void SetUp() override
//     {
//         arena          = create_linear_arena();
//         arena.instance = linear_arena_init(1024 * 1024); // 1MB arena
//     }
//
//     void TearDown() override
//     {
//         if (arena.instance) {
//             arena.destroy(arena.instance);
//         }
//     }
//
//     Arena arena;
// };
//
// // Basic Operations Tests
// TEST_F(ArrayBoolTest, BasicOperations)
// {
//     auto arr = baba::ds::array::make<bool>(&arena, 5);
//
//     EXPECT_EQ(arr.get_size(), 5);
//     EXPECT_EQ(arr.get_capacity(), 5);
//     EXPECT_FALSE(arr.empty());
//     EXPECT_NE(arr.get_data(), nullptr);
// }
//
// TEST_F(ArrayBoolTest, MakeWithCapacity)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 3, 10);
//
//     EXPECT_EQ(arr.get_size(), 3);
//     EXPECT_EQ(arr.get_capacity(), 10);
//     EXPECT_FALSE(arr.empty());
// }
//
// // Append and Access Tests
// TEST_F(ArrayBoolTest, AppendAndAccess)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 10);
//
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//
//     EXPECT_EQ(arr.get_size(), 3);
//     EXPECT_TRUE(arr.at(0));
//     EXPECT_FALSE(arr.at(1));
//     EXPECT_TRUE(arr.at(2));
//
//     EXPECT_TRUE(arr.begin());
//     EXPECT_TRUE(arr.end());
// }
//
// TEST_F(ArrayBoolTest, SafeOperations)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 2);
//
//     arr.append_safe(true);
//     arr.append_safe(false);
//
//     EXPECT_THROW(arr.append_safe(true), std::out_of_range);
//     EXPECT_THROW(arr.at_safe(5), std::out_of_range);
//
//     EXPECT_TRUE(arr.at_safe(0));
//     EXPECT_FALSE(arr.at_safe(1));
// }
//
// TEST_F(ArrayBoolTest, BeginEndSafe)
// {
//     auto empty_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
//
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     arr.append(true);
//     arr.append(false);
//
//     EXPECT_TRUE(arr.begin_safe());
//     EXPECT_FALSE(arr.end_safe());
// }
//
// // Update and Remove Tests
// TEST_F(ArrayBoolTest, UpdateAndRemove)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//
//     arr.update(1, true);
//     EXPECT_TRUE(arr.at(1));
//
//     arr.update_safe(0, false);
//     EXPECT_FALSE(arr.at(0));
//
//     EXPECT_THROW(arr.update_safe(10, true), std::out_of_range);
//
//     arr.remove(0);
//     EXPECT_EQ(arr.get_size(), 2);
//     EXPECT_TRUE(arr.at(0)); // was at index 1
//     EXPECT_TRUE(arr.at(1)); // was at index 2
//
//     arr.remove_safe(0);
//     EXPECT_EQ(arr.get_size(), 1);
//     EXPECT_THROW(arr.remove_safe(5), std::out_of_range);
// }
//
// // Fill Operations Tests
// TEST_F(ArrayBoolTest, FillOperations)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     arr.append(false);
//     arr.append(false);
//     arr.append(false);
//
//     arr.fill(true);
//     for (u32 i = 0; i < arr.get_size(); ++i) {
//         EXPECT_TRUE(arr.at(i));
//     }
//
//     arr.fill_range(0, 2, false);
//     EXPECT_FALSE(arr.at(0));
//     EXPECT_FALSE(arr.at(1));
//     EXPECT_TRUE(arr.at(2));
//
//     arr.fill_range_safe(1, 3, true);
//     EXPECT_FALSE(arr.at(0));
//     EXPECT_TRUE(arr.at(1));
//     EXPECT_TRUE(arr.at(2));
//
//     EXPECT_THROW(arr.fill_range_safe(0, 10, false), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(5, 2, false), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(2, 2, false), std::out_of_range);
// }
//
// // Comparison Operations Tests
// TEST_F(ArrayBoolTest, ComparisonOperations)
// {
//     auto arr1 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     auto arr2 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//
//     arr1.append(true);
//     arr1.append(false);
//
//     arr2.append(true);
//     arr2.append(false);
//
//     EXPECT_TRUE(arr1.equals(&arr2));
//     EXPECT_EQ(arr1.compare(&arr2), 0);
//     EXPECT_FALSE(arr1.less_than(&arr2));
//     EXPECT_FALSE(arr1.greater_than(&arr2));
//
//     arr2.update(1, true);
//     EXPECT_FALSE(arr1.equals(&arr2));
//     EXPECT_LT(arr1.compare(&arr2), 0);
//     EXPECT_TRUE(arr1.less_than(&arr2));
//     EXPECT_FALSE(arr1.greater_than(&arr2));
//
//     arr1.update(0, false);
//     EXPECT_LT(arr1.compare(&arr2), 0);
//     EXPECT_TRUE(arr1.less_than(&arr2));
//     EXPECT_FALSE(arr1.greater_than(&arr2));
// }
//
// // Prefix/Suffix Operations Tests
// TEST_F(ArrayBoolTest, PrefixSuffixOperations)
// {
//     auto arr    = baba::ds::array::make_with_capacity<bool>(&arena, 0, 6);
//     auto prefix = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//     auto suffix = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//
//     prefix.append(true);
//     prefix.append(false);
//
//     suffix.append(false);
//     suffix.append(true);
//
//     EXPECT_TRUE(arr.starts_with(&prefix));
//     EXPECT_TRUE(arr.ends_with(&suffix));
//
//     prefix.update(1, true);
//     EXPECT_FALSE(arr.starts_with(&prefix));
//
//     // Test edge cases
//     auto empty_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     EXPECT_FALSE(arr.starts_with(nullptr));
//     EXPECT_TRUE(arr.starts_with(&empty_arr)); // empty prefix should match
//
//     auto larger_prefix = baba::ds::array::make_with_capacity<bool>(&arena, 0, 10);
//     for (int i = 0; i < 8; i++) {
//         larger_prefix.append(true);
//     }
//     EXPECT_FALSE(arr.starts_with(&larger_prefix)); // larger than array
// }
//
// // Sorted Operations Tests
// TEST_F(ArrayBoolTest, SortedOperations)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 6);
//     arr.append(false);
//     arr.append(false);
//     arr.append(true);
//     arr.append(true);
//     arr.append(true);
//
//     EXPECT_TRUE(arr.sorted());
//     EXPECT_FALSE(arr.sorted_reverse());
//
//     // Test reverse sorted
//     auto rev_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 6);
//     rev_arr.append(true);
//     rev_arr.append(true);
//     rev_arr.append(false);
//     rev_arr.append(false);
//
//     EXPECT_FALSE(rev_arr.sorted());
//     EXPECT_TRUE(rev_arr.sorted_reverse());
//
//     // Test single element and empty
//     auto single_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     single_arr.append(true);
//     EXPECT_TRUE(single_arr.sorted());
//     EXPECT_TRUE(single_arr.sorted_reverse());
//
//     auto empty_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     EXPECT_TRUE(empty_arr.sorted());
//     EXPECT_TRUE(empty_arr.sorted_reverse());
// }
//
// // Range-based Sorted Operations Tests
// TEST_F(ArrayBoolTest, SortedRangeOperations)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 8);
//     arr.append(true);  // 0
//     arr.append(false); // 1 - start of sorted range
//     arr.append(false); // 2
//     arr.append(true);  // 3
//     arr.append(true);  // 4 - end of sorted range
//     arr.append(false); // 5
//     arr.append(true);  // 6
//
//     EXPECT_TRUE(arr.sorted_range(1, 5));  // false, false, true, true
//     EXPECT_FALSE(arr.sorted_range(0, 4)); // true, false, false, true
//     EXPECT_TRUE(arr.sorted_range(2, 5));  // false, true, true
//
//     // Test reverse sorted range
//     auto rev_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 8);
//     rev_arr.append(false); // 0
//     rev_arr.append(true);  // 1 - start of reverse sorted range
//     rev_arr.append(true);  // 2
//     rev_arr.append(false); // 3
//     rev_arr.append(false); // 4 - end of reverse sorted range
//     rev_arr.append(true);  // 5
//
//     EXPECT_TRUE(rev_arr.sorted_range_reverse(1, 5));  // true, true, false, false
//     EXPECT_FALSE(rev_arr.sorted_range_reverse(0, 4)); // false, true, true, false
//     EXPECT_TRUE(rev_arr.sorted_range_reverse(2, 5));  // true, false, false
//
//     // Test edge cases
//     EXPECT_TRUE(arr.sorted_range(3, 4)); // single element range
//     EXPECT_TRUE(arr.sorted_range(3, 3)); // empty range
// }
//
// // Search Operations Tests
// TEST_F(ArrayBoolTest, SearchOperations)
// {
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 6);
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//
//     EXPECT_EQ(arr.find(true), 0);
//     EXPECT_EQ(arr.find(false), 1);
//     EXPECT_EQ(arr.find_reverse(true), 4);
//     EXPECT_EQ(arr.find_reverse(false), 3);
//
//     EXPECT_TRUE(arr.contains(false));
//     EXPECT_TRUE(arr.contains(true));
//
//     EXPECT_EQ(arr.counts(true), 3);
//     EXPECT_EQ(arr.counts(false), 2);
//
//     // Test with array of all same values
//     auto same_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     same_arr.append(true);
//     same_arr.append(true);
//     same_arr.append(true);
//
//     EXPECT_EQ(same_arr.find(true), 0);
//     EXPECT_EQ(same_arr.find(false), -1);
//     EXPECT_EQ(same_arr.find_reverse(true), 2);
//     EXPECT_EQ(same_arr.counts(true), 3);
//     EXPECT_EQ(same_arr.counts(false), 0);
// }
//
// // Utility Functions Tests
// TEST_F(ArrayBoolTest, UtilityFunctions)
// {
//     auto original = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     original.append(true);
//     original.append(false);
//     original.append(true);
//
//     // Test copy
//     auto copied = baba::ds::array::copy(&arena, &original);
//     EXPECT_EQ(copied.get_size(), original.get_size());
//     EXPECT_TRUE(copied.equals(&original));
//
//     copied.update(0, false);
//     EXPECT_FALSE(copied.equals(&original));
//
//     // Test merge
//     auto arr1 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//     auto arr2 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//
//     arr1.append(true);
//     arr1.append(false);
//
//     arr2.append(false);
//     arr2.append(true);
//
//     auto merged = baba::ds::array::merge(&arena, &arr1, &arr2);
//     EXPECT_EQ(merged.get_size(), 4);
//     EXPECT_TRUE(merged.at(0));
//     EXPECT_FALSE(merged.at(1));
//     EXPECT_FALSE(merged.at(2));
//     EXPECT_TRUE(merged.at(3));
//
//     // Test split
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 6);
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//     arr.append(false);
//     arr.append(true);
//
//     auto [left, right] = baba::ds::array::split(&arr, 2);
//     EXPECT_EQ(left.get_size(), 3);
//     EXPECT_EQ(right.get_size(), 2);
//     EXPECT_TRUE(left.at(0));
//     EXPECT_FALSE(left.at(1));
//     EXPECT_TRUE(left.at(2));
//     EXPECT_FALSE(right.at(0));
//     EXPECT_TRUE(right.at(1));
//
//     // Test swap
//     auto swap1 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//     auto swap2 = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//
//     swap1.append(true);
//     swap1.append(false);
//
//     swap2.append(false);
//     swap2.append(true);
//     swap2.append(false);
//
//     u32 original_size1 = swap1.get_size();
//     u32 original_size2 = swap2.get_size();
//
//     baba::ds::array::swap(&swap1, &swap2);
//
//     EXPECT_EQ(swap1.get_size(), original_size2);
//     EXPECT_EQ(swap2.get_size(), original_size1);
//     EXPECT_FALSE(swap1.at(0));
//     EXPECT_TRUE(swap1.at(1));
//     EXPECT_FALSE(swap1.at(2));
//     EXPECT_TRUE(swap2.at(0));
//     EXPECT_FALSE(swap2.at(1));
// }
//
// // Edge Cases Tests
// TEST_F(ArrayBoolTest, EdgeCases)
// {
//     // Empty array operations
//     auto empty_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//
//     EXPECT_TRUE(empty_arr.empty());
//     EXPECT_EQ(empty_arr.get_size(), 0);
//     EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.remove_safe(0), std::out_of_range);
//     EXPECT_EQ(empty_arr.find(true), -1);
//     EXPECT_FALSE(empty_arr.contains(true));
//     EXPECT_EQ(empty_arr.counts(false), 0);
//
//     // Single element array
//     auto single_arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 5);
//     single_arr.append(true);
//
//     EXPECT_FALSE(single_arr.empty());
//     EXPECT_EQ(single_arr.get_size(), 1);
//     EXPECT_TRUE(single_arr.begin());
//     EXPECT_TRUE(single_arr.end());
//     EXPECT_TRUE(single_arr.begin_safe());
//     EXPECT_TRUE(single_arr.end_safe());
//
//     single_arr.remove(0);
//     EXPECT_TRUE(single_arr.empty());
//
//     // Boundary conditions
//     auto arr = baba::ds::array::make_with_capacity<bool>(&arena, 0, 3);
//
//     // Fill to capacity
//     arr.append_safe(true);
//     arr.append_safe(false);
//     arr.append_safe(true);
//
//     EXPECT_THROW(arr.append_safe(false), std::out_of_range);
//
//     // Test range operations at boundaries
//     EXPECT_THROW(arr.fill_range_safe(0, 4, true), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(3, 4, true), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(2, 1, true), std::out_of_range);
// }
