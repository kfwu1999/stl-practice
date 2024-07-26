/**
 * \file test/test_heap.cpp
 */
#include <functional>
#include <algorithm>   // for comparing the STL function
#include <gtest/gtest.h>

#include "vector.hpp"
#include "algorithm/is_heap.hpp"
#include "algorithm/push_heap.hpp"
#include "algorithm/pop_heap.hpp"
#include "algorithm/make_heap.hpp"
#include "algorithm/sort_heap.hpp"


// 
class HeapFunctionTests : public ::testing::Test {
protected:
    mystl::vector<int> random_array;
    mystl::vector<int> max_heap;
    mystl::vector<int> min_heap;
    mystl::vector<int> dup_elems;

    void SetUp() override {
        //
        random_array = {74, -42, 48, -44, 14, 5, 96, -98, -80, 18, 64, -38, -31, -36, 73, 25, -18, -45, -42, 30};

        // 
        max_heap = random_array;
        std::make_heap(max_heap.begin(), max_heap.end());

        //
        min_heap = random_array;
        std::make_heap(min_heap.begin(), min_heap.end(), std::greater<>());

        // 
        dup_elems.resize(20, 1);
    }
};


// Test `is_heap` for empty container
TEST_F(HeapFunctionTests, EmptyHeapIsHeap) {
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.begin()));
}


// Test `is_heap` for single element
TEST_F(HeapFunctionTests, SingleElementIsHeap) {
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.begin() + 1));
}


// Test `is_heap` for duplicate elements
TEST_F(HeapFunctionTests, DuplicateElementIsHeap) {
    EXPECT_TRUE(mystl::is_heap(dup_elems.begin(), dup_elems.end()));
    EXPECT_EQ(mystl::is_heap(dup_elems.begin(), dup_elems.end()), std::is_heap(dup_elems.begin(), dup_elems.end()));
}


// Test `is_heap` for max heap
TEST_F(HeapFunctionTests, MaxHeapIsHeap) {
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));   // the correctness of is_heap for max heap
    EXPECT_EQ(mystl::is_heap(max_heap.begin(), max_heap.end()),
                std::is_heap(max_heap.begin(), max_heap.end()));     // the result should be same as std::is_heap
}


// Test `is_heap` for min heap
TEST_F(HeapFunctionTests, MinHeapIsHeap) {
    EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));
    EXPECT_EQ(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()),
                std::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));
}


// Test `is_heap` for container that's not a heap
TEST_F(HeapFunctionTests, NotAHeapIsHeap) {
    EXPECT_FALSE(mystl::is_heap(random_array.begin(), random_array.end()));
    EXPECT_EQ(mystl::is_heap(random_array.begin(), random_array.end()),
                std::is_heap(random_array.begin(), random_array.end()));
    EXPECT_FALSE(mystl::is_heap(random_array.begin(), random_array.end(), std::greater<>()));
    EXPECT_EQ(mystl::is_heap(random_array.begin(), random_array.end(), std::greater<>()),
                std::is_heap(random_array.begin(), random_array.end(), std::greater<>()));
}


// Test `push_heap` for max heap
TEST_F(HeapFunctionTests, MaxHeapPushHeap) {
    // 
    int new_max_element = 95;
    max_heap.push_back(new_max_element);
    mystl::push_heap(max_heap.begin(), max_heap.end());
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));

    // 
    max_heap.push_back(new_max_element);
    mystl::push_heap(max_heap.begin(), max_heap.end());
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));

    // 
    int new_min_element = 0;
    max_heap.push_back(new_min_element);
    mystl::push_heap(max_heap.begin(), max_heap.end());
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));

    // 
    max_heap.push_back(new_min_element);
    mystl::push_heap(max_heap.begin(), max_heap.end());
    EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));
}


// Test `push_heap` for min heap
TEST_F(HeapFunctionTests, MinHeapPushHeap) {
    // 
    int new_max_element = 95;
    min_heap.push_back(new_max_element);
    mystl::push_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));

    // 
    min_heap.push_back(new_max_element);
    mystl::push_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));

    // 
    int new_min_element = 0;
    min_heap.push_back(new_min_element);
    mystl::push_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));

    // 
    min_heap.push_back(new_min_element);
    mystl::push_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, MaxHeapPopHeap) {
    while (!max_heap.empty()) {
        mystl::pop_heap(max_heap.begin(), max_heap.end());
        max_heap.pop_back();
        EXPECT_TRUE(mystl::is_heap(max_heap.begin(), max_heap.end()));
    }
}


// 
TEST_F(HeapFunctionTests, MinHeapPopHeap) {
    while (!min_heap.empty()) {
        mystl::pop_heap(min_heap.begin(), min_heap.end(), std::greater<>());
        min_heap.pop_back();
        EXPECT_TRUE(mystl::is_heap(min_heap.begin(), min_heap.end(), std::greater<>()));
    }
}


// 
TEST_F(HeapFunctionTests, MaxHeapCreatedByMakeHeap) {
    // create max heap by a random array
    mystl::make_heap(random_array.begin(), random_array.end());
    EXPECT_TRUE(std::is_heap(random_array.begin(), random_array.end())) << "Failed to create a max heap with random array";

    // create max heap by a max heap
    mystl::make_heap(max_heap.begin(), max_heap.end());
    EXPECT_TRUE(std::is_heap(max_heap.begin(), max_heap.end())) << "Failed to create a max heap with max heap";

    // create max heap by a min heap
    mystl::make_heap(min_heap.begin(), min_heap.end());
    EXPECT_TRUE(std::is_heap(min_heap.begin(), min_heap.end())) << "Failed to create a max heap with min heap";
}


// 
TEST_F(HeapFunctionTests, MinHeapCreatedByMakeHeap) {
    // create min heap by a random array
    mystl::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    EXPECT_TRUE(std::is_heap(random_array.begin(), random_array.end(), std::greater<>())) << "Failed to create a min heap with random array";

    // create min heap by a max heap
    mystl::make_heap(max_heap.begin(), max_heap.end(), std::greater<>());
    EXPECT_TRUE(std::is_heap(max_heap.begin(), max_heap.end(), std::greater<>())) << "Failed to create a min heap with max heap";

    // create min heap by a min heap
    mystl::make_heap(min_heap.begin(), min_heap.end(), std::greater<>());
    EXPECT_TRUE(std::is_heap(min_heap.begin(), min_heap.end(), std::greater<>())) << "Failed to create a min heap with min heap";
}


// 
TEST_F(HeapFunctionTests, EmptyContainerMakeHeap) {
    // 
    mystl::vector<int> empty_vec;

    // 
    mystl::make_heap(empty_vec.begin(), empty_vec.end());
    EXPECT_TRUE(mystl::is_heap(empty_vec.begin(), empty_vec.end()));

    // 
    mystl::make_heap(empty_vec.begin(), empty_vec.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(empty_vec.begin(), empty_vec.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, SingleElementMakeHeap) {
    // 
    mystl::vector<int> single_elem_vec = {1};

    // 
    mystl::make_heap(single_elem_vec.begin(), single_elem_vec.end());
    EXPECT_TRUE(mystl::is_heap(single_elem_vec.begin(), single_elem_vec.end()));

    // 
    mystl::make_heap(single_elem_vec.begin(), single_elem_vec.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(single_elem_vec.begin(), single_elem_vec.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, DuplicateElementsMakeHeap) {
    // 
    mystl::make_heap(dup_elems.begin(), dup_elems.end());
    EXPECT_TRUE(mystl::is_heap(dup_elems.begin(), dup_elems.end()));

    //
    mystl::make_heap(dup_elems.begin(), dup_elems.end(), std::greater<>());
    EXPECT_TRUE(mystl::is_heap(dup_elems.begin(), dup_elems.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, SortHeapAscending) {
    std::make_heap(random_array.begin(), random_array.end());
    mystl::sort_heap(random_array.begin(), random_array.end());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end()));
}


// 
TEST_F(HeapFunctionTests, SortHeapDescending) {
    std::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    mystl::sort_heap(random_array.begin(), random_array.end(), std::greater<>());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, SortedArraySortHeap) {
    // Sort Ascending
    std::make_heap(random_array.begin(), random_array.end());
    mystl::sort_heap(random_array.begin(), random_array.end());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end()));
    std::make_heap(random_array.begin(), random_array.end());
    mystl::sort_heap(random_array.begin(), random_array.end());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end()));

    // Sort Descending
    std::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    mystl::sort_heap(random_array.begin(), random_array.end(), std::greater<>());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end(), std::greater<>()));
    std::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    mystl::sort_heap(random_array.begin(), random_array.end(), std::greater<>());
    EXPECT_TRUE(std::is_sorted(random_array.begin(), random_array.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, DuplicateElementSortHeap) {
    // Sort Ascending
    std::make_heap(random_array.begin(), random_array.end());
    mystl::sort_heap(dup_elems.begin(), dup_elems.end());
    EXPECT_TRUE(std::is_sorted(dup_elems.begin(), dup_elems.end()));

    // Sort Descending
    std::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    mystl::sort_heap(dup_elems.begin(), dup_elems.end(), std::greater<>());
    EXPECT_TRUE(std::is_sorted(dup_elems.begin(), dup_elems.end(), std::greater<>()));
}


// 
TEST_F(HeapFunctionTests, SingleElementSortHeap) {
    // 
    mystl::vector<int> single_elem = {1};

    // Sort Ascending
    std::make_heap(random_array.begin(), random_array.end());
    mystl::sort_heap(single_elem.begin(), single_elem.end());
    EXPECT_TRUE(std::is_sorted(single_elem.begin(), single_elem.end()));

    // Sort Descending
    std::make_heap(random_array.begin(), random_array.end(), std::greater<>());
    mystl::sort_heap(dup_elems.begin(), dup_elems.end(), std::greater<>());
    EXPECT_TRUE(std::is_sorted(single_elem.begin(), single_elem.end(), std::greater<>()));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
