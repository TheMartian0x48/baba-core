#include <baba/baba.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

// using namespace baba::alias;
// using namespace baba::memory;
// using namespace baba::ds::array;
//
// // Custom test struct for non-primitive testing
// struct TestStruct {
//     i32  id;
//     f32  value;
//
//     bool operator==(const TestStruct& other) const
//     {
//         return id == other.id && value == other.value;
//     }
//
//     bool operator<(const TestStruct& other) const
//     {
//         if (id != other.id)
//             return id < other.id;
//         return value < other.value;
//     }
//
//     bool operator>(const TestStruct& other) const { return other < *this; }
// };
//
// // Custom comparison functions for TestStruct
// bool test_struct_equal(const TestStruct* a, const TestStruct* b)
// {
//     return a->id == b->id && a->value == b->value;
// }
//
// int test_struct_compare(const TestStruct* a, const TestStruct* b)
// {
//     if (a->id < b->id)
//         return -1;
//     if (a->id > b->id)
//         return 1;
//     if (a->value < b->value)
//         return -1;
//     if (a->value > b->value)
//         return 1;
//     return 0;
// }
//
// bool test_struct_greater_than(const TestStruct* a, const TestStruct* b) { return *a > *b; }
//
// class ArrayStructTest : public ::testing::Test
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
// TEST_F(ArrayStructTest, BasicOperations)
// {
//     auto arr = baba::ds::array::make<TestStruct>(&arena, 3);
//
//     EXPECT_EQ(arr.get_size(), 3);
//     EXPECT_EQ(arr.get_capacity(), 3);
//     EXPECT_FALSE(arr.empty());
//     EXPECT_NE(arr.get_data(), nullptr);
// }
//
// TEST_F(ArrayStructTest, MakeWithCapacity)
// {
//     auto arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 2, 8);
//
//     EXPECT_EQ(arr.get_size(), 2);
//     EXPECT_EQ(arr.get_capacity(), 8);
//     EXPECT_FALSE(arr.empty());
// }
//
// // Append and Access Tests
// TEST_F(ArrayStructTest, AppendAndAccess)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//
//     TestStruct s1{1, 1.5f};
//     TestStruct s2{2, 2.5f};
//     TestStruct s3{3, 3.5f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//
//     EXPECT_EQ(arr.get_size(), 3);
//     EXPECT_EQ(arr.at(0).id, 1);
//     EXPECT_FLOAT_EQ(arr.at(0).value, 1.5f);
//     EXPECT_EQ(arr.at(1).id, 2);
//     EXPECT_EQ(arr.at(2).id, 3);
//
//     EXPECT_EQ(arr.begin().id, 1);
//     EXPECT_EQ(arr.end().id, 3);
// }
//
// TEST_F(ArrayStructTest, SafeOperations)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 2);
//
//     TestStruct s1{10, 10.5f};
//     TestStruct s2{20, 20.5f};
//     TestStruct s3{30, 30.5f};
//
//     arr.append_safe(&s1);
//     arr.append_safe(&s2);
//
//     EXPECT_THROW(arr.append_safe(&s3), std::out_of_range);
//     EXPECT_THROW(arr.at_safe(5), std::out_of_range);
//
//     EXPECT_EQ(arr.at_safe(0).id, 10);
//     EXPECT_EQ(arr.at_safe(1).id, 20);
// }
//
// TEST_F(ArrayStructTest, BeginEndSafe)
// {
//     auto empty_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
//
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct s1{123, 12.3f};
//     TestStruct s2{456, 45.6f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//
//     EXPECT_EQ(arr.begin_safe().id, 123);
//     EXPECT_EQ(arr.end_safe().id, 456);
// }
//
// // Update and Remove Tests
// TEST_F(ArrayStructTest, UpdateAndRemove)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//
//     TestStruct s1{1, 1.0f};
//     TestStruct s2{2, 2.0f};
//     TestStruct s3{3, 3.0f};
//     TestStruct s_new{99, 99.0f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//
//     arr.update(1, &s_new);
//     EXPECT_EQ(arr.at(1).id, 99);
//
//     TestStruct s_update{-50, -5.0f};
//     arr.update_safe(0, &s_update);
//     EXPECT_EQ(arr.at(0).id, -50);
//
//     EXPECT_THROW(arr.update_safe(10, &s_new), std::out_of_range);
//
//     arr.remove_safe(0);
//     EXPECT_EQ(arr.get_size(), 2);
//     EXPECT_EQ(arr.at(0).id, 99);
//     EXPECT_EQ(arr.at(1).id, 3);
//
//     arr.remove(0);
//     EXPECT_EQ(arr.get_size(), 1);
//     EXPECT_EQ(arr.at(0).id, 3);
//
//     EXPECT_THROW(arr.remove_safe(5), std::out_of_range);
// }
//
// // Fill Operations Tests
// TEST_F(ArrayStructTest, FillOperations)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//
//     TestStruct s_init{0, 0.0f};
//     TestStruct s_fill{42, 42.0f};
//
//     arr.append(&s_init);
//     arr.append(&s_init);
//     arr.append(&s_init);
//
//     arr.fill(&s_fill);
//     for (u32 i = 0; i < arr.get_size(); ++i) {
//         EXPECT_EQ(arr.at(i).id, 42);
//         EXPECT_FLOAT_EQ(arr.at(i).value, 42.0f);
//     }
//
//     TestStruct s_range{-1, -1.0f};
//     arr.fill_range(0, 2, &s_range);
//     EXPECT_EQ(arr.at(0).id, -1);
//     EXPECT_EQ(arr.at(1).id, -1);
//     EXPECT_EQ(arr.at(2).id, 42);
//
//     TestStruct s_safe{99, 9.9f};
//     arr.fill_range_safe(1, 3, &s_safe);
//     EXPECT_EQ(arr.at(0).id, -1);
//     EXPECT_EQ(arr.at(1).id, 99);
//     EXPECT_EQ(arr.at(2).id, 99);
//
//     EXPECT_THROW(arr.fill_range_safe(0, 10, &s_fill), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(5, 2, &s_fill), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(2, 2, &s_fill), std::out_of_range);
// }
//
// // Comparison Operations Tests
// TEST_F(ArrayStructTest, ComparisonWithCustomFunction)
// {
//     auto       arr1 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//     auto       arr2 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//
//     TestStruct s1{1, 1.0f};
//     TestStruct s2{2, 2.0f};
//
//     arr1.append(&s1);
//     arr1.append(&s2);
//
//     arr2.append(&s1);
//     arr2.append(&s2);
//
//     EXPECT_TRUE(arr1.equals(&arr2, test_struct_equal));
//     EXPECT_EQ(arr1.compare(&arr2, test_struct_compare), 0);
//     EXPECT_FALSE(arr1.less_than(&arr2, test_struct_compare));
//     EXPECT_FALSE(arr1.greater_than(&arr2, test_struct_compare));
//
//     TestStruct s3{3, 3.0f};
//     arr2.update(1, &s3);
//
//     EXPECT_FALSE(arr1.equals(&arr2, test_struct_equal));
//     EXPECT_LT(arr1.compare(&arr2, test_struct_compare), 0);
//     EXPECT_TRUE(arr1.less_than(&arr2, test_struct_compare));
//     EXPECT_FALSE(arr1.greater_than(&arr2, test_struct_compare));
//
//     TestStruct s4{5, 5.0f};
//     arr1.update(0, &s4);
//     EXPECT_GT(arr1.compare(&arr2, test_struct_compare), 0);
//     EXPECT_FALSE(arr1.less_than(&arr2, test_struct_compare));
//     EXPECT_TRUE(arr1.greater_than(&arr2, test_struct_compare));
//
//     // Test with different sizes
//     TestStruct s5{10, 10.0f};
//     arr1.append(&s5);
//     EXPECT_GT(arr1.compare(&arr2, test_struct_compare), 0);
//     EXPECT_FALSE(arr1.less_than(&arr2, test_struct_compare));
//     EXPECT_TRUE(arr1.greater_than(&arr2, test_struct_compare));
// }
//
// // Prefix/Suffix Operations Tests
// TEST_F(ArrayStructTest, PrefixSuffixWithCustomFunction)
// {
//     auto       arr    = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     auto       prefix = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 2);
//     auto       suffix = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 2);
//
//     TestStruct s1{1, 1.0f};
//     TestStruct s2{2, 2.0f};
//     TestStruct s3{3, 3.0f};
//     TestStruct s4{4, 4.0f};
//     TestStruct s5{5, 5.0f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//     arr.append(&s4);
//     arr.append(&s5);
//
//     prefix.append(&s1);
//     prefix.append(&s2);
//
//     suffix.append(&s4);
//     suffix.append(&s5);
//
//     EXPECT_TRUE(arr.starts_with(&prefix, test_struct_equal));
//     EXPECT_TRUE(arr.ends_with(&suffix, test_struct_equal));
//
//     TestStruct s_different{99, 99.0f};
//     prefix.update(1, &s_different);
//     EXPECT_FALSE(arr.starts_with(&prefix, test_struct_equal));
//
//     // Test edge cases
//     auto empty_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     EXPECT_FALSE(arr.starts_with(nullptr, test_struct_equal));
//     EXPECT_TRUE(arr.starts_with(&empty_arr, test_struct_equal)); // empty prefix should match
//     EXPECT_TRUE(arr.ends_with(&empty_arr, test_struct_equal));   // empty suffix should match
//
//     auto larger_prefix = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 10);
//     for (int i = 0; i < 8; i++) {
//         TestStruct temp{i + 100, static_cast<f32>(i) + 10.0f};
//         larger_prefix.append(&temp);
//     }
//     EXPECT_FALSE(arr.starts_with(&larger_prefix, test_struct_equal)); // larger than array
//
//     // Test same size arrays
//     auto same_size = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     same_size.append(&s1);
//     same_size.append(&s2);
//     same_size.append(&s3);
//     same_size.append(&s4);
//     same_size.append(&s5);
//     EXPECT_TRUE(arr.starts_with(&same_size, test_struct_equal));
//     EXPECT_TRUE(arr.ends_with(&same_size, test_struct_equal));
// }
//
// // Sorted Operations Tests
// TEST_F(ArrayStructTest, SortedWithCustomFunction)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 4);
//
//     TestStruct s1{1, 1.0f};
//     TestStruct s2{2, 2.0f};
//     TestStruct s3{3, 3.0f};
//     TestStruct s4{4, 4.0f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//     arr.append(&s4);
//
//     EXPECT_TRUE(arr.sorted(test_struct_greater_than));
//     EXPECT_FALSE(arr.sorted_reverse(test_struct_greater_than));
//
//     // Test reverse sorted
//     auto       rev_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 4);
//     TestStruct rs1{10, 10.0f};
//     TestStruct rs2{8, 8.0f};
//     TestStruct rs3{5, 5.0f};
//     TestStruct rs4{2, 2.0f};
//
//     rev_arr.append(&rs1);
//     rev_arr.append(&rs2);
//     rev_arr.append(&rs3);
//     rev_arr.append(&rs4);
//
//     EXPECT_FALSE(rev_arr.sorted(test_struct_greater_than));
//     EXPECT_TRUE(rev_arr.sorted_reverse(test_struct_greater_than));
//
//     // Test unsorted array
//     TestStruct s_out_of_order{0, 0.0f};
//     arr.update(2, &s_out_of_order);
//     EXPECT_FALSE(arr.sorted(test_struct_greater_than));
//     EXPECT_FALSE(arr.sorted_reverse(test_struct_greater_than));
//
//     // Test single element and empty
//     auto       single_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct single{42, 4.2f};
//     single_arr.append(&single);
//     EXPECT_TRUE(single_arr.sorted(test_struct_greater_than));
//     EXPECT_TRUE(single_arr.sorted_reverse(test_struct_greater_than));
//
//     auto empty_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     EXPECT_TRUE(empty_arr.sorted(test_struct_greater_than));
//     EXPECT_TRUE(empty_arr.sorted_reverse(test_struct_greater_than));
//
//     // Test with duplicates
//     auto       dup_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 6);
//     TestStruct d1{1, 1.0f};
//     TestStruct d2{2, 2.0f};
//     TestStruct d2_dup{2, 2.0f};
//     TestStruct d3{3, 3.0f};
//
//     dup_arr.append(&d1);
//     dup_arr.append(&d2);
//     dup_arr.append(&d2_dup);
//     dup_arr.append(&d3);
//     EXPECT_TRUE(dup_arr.sorted(test_struct_greater_than));
// }
//
// // Range-based Sorted Operations Tests
// TEST_F(ArrayStructTest, SortedRangeWithCustomFunction)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 10);
//
//     TestStruct s0{5, 5.0f}; // 0
//     TestStruct s1{1, 1.0f}; // 1 - start of sorted range
//     TestStruct s2{2, 2.0f}; // 2
//     TestStruct s3{4, 4.0f}; // 3
//     TestStruct s4{6, 6.0f}; // 4 - end of sorted range
//     TestStruct s5{3, 3.0f}; // 5
//     TestStruct s6{8, 8.0f}; // 6
//
//     arr.append(&s0);
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//     arr.append(&s4);
//     arr.append(&s5);
//     arr.append(&s6);
//
//     EXPECT_TRUE(arr.sorted_range(1, 5, test_struct_greater_than));  // 1, 2, 4, 6
//     EXPECT_FALSE(arr.sorted_range(0, 4, test_struct_greater_than)); // 5, 1, 2, 4
//     EXPECT_TRUE(arr.sorted_range(2, 5, test_struct_greater_than));  // 2, 4, 6
//     EXPECT_TRUE(arr.sorted_range(3, 4, test_struct_greater_than));  // single element: 4
//     EXPECT_TRUE(arr.sorted_range(3, 3, test_struct_greater_than));  // empty range
//
//     // Test reverse sorted range
//     auto       rev_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 10);
//     TestStruct r0{1, 1.0f};   // 0
//     TestStruct r1{10, 10.0f}; // 1 - start of reverse sorted range
//     TestStruct r2{8, 8.0f};   // 2
//     TestStruct r3{5, 5.0f};   // 3
//     TestStruct r4{2, 2.0f};   // 4 - end of reverse sorted range
//     TestStruct r5{7, 7.0f};   // 5
//
//     rev_arr.append(&r0);
//     rev_arr.append(&r1);
//     rev_arr.append(&r2);
//     rev_arr.append(&r3);
//     rev_arr.append(&r4);
//     rev_arr.append(&r5);
//
//     EXPECT_TRUE(rev_arr.sorted_range_reverse(1, 5, test_struct_greater_than));  // 10, 8, 5, 2
//     EXPECT_FALSE(rev_arr.sorted_range_reverse(0, 4, test_struct_greater_than)); // 1, 10, 8, 5
//     EXPECT_TRUE(rev_arr.sorted_range_reverse(2, 5, test_struct_greater_than));  // 8, 5, 2
//
//     // Test with duplicates in range
//     auto       dup_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 8);
//     TestStruct d0{9, 9.0f}; // 0
//     TestStruct d1{1, 1.0f}; // 1
//     TestStruct d2{2, 2.0f}; // 2
//     TestStruct d3{2, 2.0f}; // 3 (duplicate)
//     TestStruct d4{3, 3.0f}; // 4
//     TestStruct d5{3, 3.0f}; // 5 (duplicate)
//     TestStruct d6{0, 0.0f}; // 6
//
//     dup_arr.append(&d0);
//     dup_arr.append(&d1);
//     dup_arr.append(&d2);
//     dup_arr.append(&d3);
//     dup_arr.append(&d4);
//     dup_arr.append(&d5);
//     dup_arr.append(&d6);
//
//     EXPECT_TRUE(dup_arr.sorted_range(1, 6, test_struct_greater_than)); // 1, 2, 2, 3, 3
//     EXPECT_FALSE(
//         dup_arr.sorted_range(0, 7, test_struct_greater_than)); // includes 9 at start and 0 at end
//
//     // Test with negative values
//     auto       neg_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 6);
//     TestStruct n0{0, 0.0f};
//     TestStruct n1{-5, -5.0f};
//     TestStruct n2{-3, -3.0f};
//     TestStruct n3{-1, -1.0f};
//     TestStruct n4{2, 2.0f};
//
//     neg_arr.append(&n0);
//     neg_arr.append(&n1);
//     neg_arr.append(&n2);
//     neg_arr.append(&n3);
//     neg_arr.append(&n4);
//
//     EXPECT_TRUE(neg_arr.sorted_range(1, 5, test_struct_greater_than));  // -5, -3, -1, 2
//     EXPECT_FALSE(neg_arr.sorted_range(0, 5, test_struct_greater_than)); // includes 0 at start
// }
//
// // Search Operations Tests
// TEST_F(ArrayStructTest, SearchWithCustomFunction)
// {
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//
//     TestStruct s1{1, 1.0f};
//     TestStruct s2{2, 2.0f};
//     TestStruct s3{1, 1.0f}; // duplicate
//     TestStruct s_search{1, 1.0f};
//     TestStruct s_not_found{99, 99.0f};
//
//     arr.append(&s1);
//     arr.append(&s2);
//     arr.append(&s3);
//
//     EXPECT_EQ(arr.find(&s_search, test_struct_equal), 0);
//     EXPECT_EQ(arr.find_reverse(&s_search, test_struct_equal), 2);
//     EXPECT_EQ(arr.find(&s_not_found, test_struct_equal), -1);
//     EXPECT_TRUE(arr.contains(&s_search, test_struct_equal));
//     EXPECT_FALSE(arr.contains(&s_not_found, test_struct_equal));
//     EXPECT_EQ(arr.counts(&s_search, test_struct_equal), 2);
//     EXPECT_EQ(arr.counts(&s_not_found, test_struct_equal), 0);
//
//     // Test with negative values
//     auto       neg_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 6);
//     TestStruct neg1{-5, -5.5f};
//     TestStruct zero{0, 0.0f};
//     TestStruct neg2{-5, -5.5f}; // duplicate
//     TestStruct pos{10, 10.1f};
//
//     neg_arr.append(&neg1);
//     neg_arr.append(&zero);
//     neg_arr.append(&neg2);
//     neg_arr.append(&pos);
//
//     TestStruct search_neg{-5, -5.5f};
//     TestStruct search_zero{0, 0.0f};
//     TestStruct search_missing{-999, -99.9f};
//
//     EXPECT_EQ(neg_arr.find(&search_neg, test_struct_equal), 0);
//     EXPECT_EQ(neg_arr.find_reverse(&search_neg, test_struct_equal), 2);
//     EXPECT_EQ(neg_arr.find(&search_zero, test_struct_equal), 1);
//     EXPECT_TRUE(neg_arr.contains(&search_neg, test_struct_equal));
//     EXPECT_TRUE(neg_arr.contains(&search_zero, test_struct_equal));
//     EXPECT_FALSE(neg_arr.contains(&search_missing, test_struct_equal));
//     EXPECT_EQ(neg_arr.counts(&search_neg, test_struct_equal), 2);
//     EXPECT_EQ(neg_arr.counts(&search_zero, test_struct_equal), 1);
//
//     // Test with all same values
//     auto       same_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct same{42, 4.2f};
//     same_arr.append(&same);
//     same_arr.append(&same);
//     same_arr.append(&same);
//
//     TestStruct search_same{42, 4.2f};
//     EXPECT_EQ(same_arr.find(&search_same, test_struct_equal), 0);
//     EXPECT_EQ(same_arr.find_reverse(&search_same, test_struct_equal), 2);
//     EXPECT_EQ(same_arr.counts(&search_same, test_struct_equal), 3);
// }
//
// // Utility Functions Tests
// TEST_F(ArrayStructTest, UtilityFunctions)
// {
//     auto       original = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct s1{1, 1.1f};
//     TestStruct s2{2, 2.2f};
//     TestStruct s3{3, 3.3f};
//
//     original.append(&s1);
//     original.append(&s2);
//     original.append(&s3);
//
//     // Test copy
//     auto copied = baba::ds::array::copy(&arena, &original);
//     EXPECT_EQ(copied.get_size(), original.get_size());
//     EXPECT_TRUE(copied.equals(&original, test_struct_equal));
//
//     TestStruct s_new{99, 9.9f};
//     copied.update(0, &s_new);
//     EXPECT_FALSE(copied.equals(&original, test_struct_equal));
//
//     // Test merge
//     auto       arr1 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//     auto       arr2 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//
//     TestStruct m1{1, 1.1f};
//     TestStruct m2{2, 2.2f};
//     TestStruct m3{3, 3.3f};
//     TestStruct m4{4, 4.4f};
//
//     arr1.append(&m1);
//     arr1.append(&m2);
//
//     arr2.append(&m3);
//     arr2.append(&m4);
//
//     auto merged = baba::ds::array::merge(&arena, &arr1, &arr2);
//     EXPECT_EQ(merged.get_size(), 4);
//     EXPECT_EQ(merged.at(0).id, 1);
//     EXPECT_EQ(merged.at(1).id, 2);
//     EXPECT_EQ(merged.at(2).id, 3);
//     EXPECT_EQ(merged.at(3).id, 4);
//
//     // Test split
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 6);
//     TestStruct sp1{1, 1.1f};
//     TestStruct sp2{2, 2.2f};
//     TestStruct sp3{3, 3.3f};
//     TestStruct sp4{4, 4.4f};
//     TestStruct sp5{5, 5.5f};
//
//     arr.append(&sp1);
//     arr.append(&sp2);
//     arr.append(&sp3);
//     arr.append(&sp4);
//     arr.append(&sp5);
//
//     auto [left, right] = baba::ds::array::split(&arr, 2);
//     EXPECT_EQ(left.get_size(), 3);
//     EXPECT_EQ(right.get_size(), 2);
//     EXPECT_EQ(left.at(0).id, 1);
//     EXPECT_EQ(left.at(1).id, 2);
//     EXPECT_EQ(left.at(2).id, 3);
//     EXPECT_EQ(right.at(0).id, 4);
//     EXPECT_EQ(right.at(1).id, 5);
//
//     // Test split edge cases
//     auto [left_edge, right_edge] = baba::ds::array::split(&arr, 0);
//     EXPECT_EQ(left_edge.get_size(), 1);
//     EXPECT_EQ(right_edge.get_size(), 4);
//
//     auto [left_end, right_end] = baba::ds::array::split(&arr, 4);
//     EXPECT_EQ(left_end.get_size(), 5);
//     EXPECT_EQ(right_end.get_size(), 0);
//
//     // Test swap
//     auto       swap1 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//     auto       swap2 = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//
//     TestStruct sw1{1, 1.1f};
//     TestStruct sw2{2, 2.2f};
//     TestStruct sw3{3, 3.3f};
//     TestStruct sw4{4, 4.4f};
//     TestStruct sw5{5, 5.5f};
//
//     swap1.append(&sw1);
//     swap1.append(&sw2);
//
//     swap2.append(&sw3);
//     swap2.append(&sw4);
//     swap2.append(&sw5);
//
//     u32 original_size1 = swap1.get_size();
//     u32 original_size2 = swap2.get_size();
//
//     baba::ds::array::swap(&swap1, &swap2);
//
//     EXPECT_EQ(swap1.get_size(), original_size2);
//     EXPECT_EQ(swap2.get_size(), original_size1);
//     EXPECT_EQ(swap1.at(0).id, 3);
//     EXPECT_EQ(swap1.at(1).id, 4);
//     EXPECT_EQ(swap1.at(2).id, 5);
//     EXPECT_EQ(swap2.at(0).id, 1);
//     EXPECT_EQ(swap2.at(1).id, 2);
// }
//
// // Edge Cases Tests
// TEST_F(ArrayStructTest, EdgeCases)
// {
//     // Empty array operations
//     auto empty_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//
//     EXPECT_TRUE(empty_arr.empty());
//     EXPECT_EQ(empty_arr.get_size(), 0);
//     EXPECT_THROW(empty_arr.begin_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.end_safe(), std::out_of_range);
//     EXPECT_THROW(empty_arr.remove_safe(0), std::out_of_range);
//
//     TestStruct search{42, 4.2f};
//     EXPECT_EQ(empty_arr.find(&search, test_struct_equal), -1);
//     EXPECT_FALSE(empty_arr.contains(&search, test_struct_equal));
//     EXPECT_EQ(empty_arr.counts(&search, test_struct_equal), 0);
//
//     // Single element array
//     auto       single_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct single{42, 4.2f};
//     single_arr.append(&single);
//
//     EXPECT_FALSE(single_arr.empty());
//     EXPECT_EQ(single_arr.get_size(), 1);
//     EXPECT_EQ(single_arr.begin().id, 42);
//     EXPECT_EQ(single_arr.end().id, 42);
//     EXPECT_EQ(single_arr.begin_safe().id, 42);
//     EXPECT_EQ(single_arr.end_safe().id, 42);
//
//     single_arr.remove(0);
//     EXPECT_TRUE(single_arr.empty());
//
//     // Boundary conditions
//     auto       arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 3);
//     TestStruct s1{1, 1.1f};
//     TestStruct s2{2, 2.2f};
//     TestStruct s3{3, 3.3f};
//     TestStruct s4{4, 4.4f};
//
//     // Fill to capacity
//     arr.append_safe(&s1);
//     arr.append_safe(&s2);
//     arr.append_safe(&s3);
//
//     EXPECT_THROW(arr.append_safe(&s4), std::out_of_range);
//
//     // Test range operations at boundaries
//     TestStruct fill_val{99, 9.9f};
//     EXPECT_THROW(arr.fill_range_safe(0, 4, &fill_val), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(3, 4, &fill_val), std::out_of_range);
//     EXPECT_THROW(arr.fill_range_safe(2, 1, &fill_val), std::out_of_range);
//
//     // Test with extreme struct values
//     auto       extreme_arr = baba::ds::array::make_with_capacity<TestStruct>(&arena, 0, 5);
//     TestStruct max_struct{INT32_MAX, FLT_MAX};
//     TestStruct min_struct{INT32_MIN, FLT_MIN};
//     TestStruct zero_struct{0, 0.0f};
//
//     extreme_arr.append(&max_struct);
//     extreme_arr.append(&min_struct);
//     extreme_arr.append(&zero_struct);
//
//     TestStruct search_max{INT32_MAX, FLT_MAX};
//     TestStruct search_min{INT32_MIN, FLT_MIN};
//     EXPECT_EQ(extreme_arr.find(&search_max, test_struct_equal), 0);
//     EXPECT_EQ(extreme_arr.find(&search_min, test_struct_equal), 1);
//     EXPECT_TRUE(extreme_arr.contains(&zero_struct, test_struct_equal));
//     EXPECT_FALSE(extreme_arr.sorted(test_struct_greater_than));
// }
