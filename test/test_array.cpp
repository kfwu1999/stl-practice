/**
 * \file test_array.cpp
 */

#include <stdexcept>

#include <gtest/gtest.h>

#include "array.hpp"

/**
 * Test Case: DefaultConstructor
 */
TEST(ArrayTest, DefaultConstructor) {
    mystl::array<int, 5> arr = {0};
    EXPECT_EQ(arr.size(), 5);
}

/**
 * Test Case: ElementAccess
 */
TEST(ArrayTest, ElementAccess) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);
}

/**
 * Test Case: AtFunction
 */
TEST(ArrayTest, AtFunction) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    EXPECT_EQ(arr.at(0), 1);
    EXPECT_THROW(arr.at(5), std::out_of_range);
}

/**
 * Test Case: FrontAndBack
 */
TEST(ArrayTest, FrontAndBack) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 5);
}

/**
 * Test Case: DataAccess
 */
TEST(ArrayTest, DataAccess) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    int* ptr = arr.data();
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[4], 5);
}

/**
 * Test Case: Fill
 */
TEST(ArrayTest, Fill) {
    mystl::array<int, 5> arr;
    arr.fill(42);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 42);
    EXPECT_EQ(arr[2], 42);
    EXPECT_EQ(arr[3], 42);
    EXPECT_EQ(arr[4], 42);
}

/**
 * Test Case: Swap
 */
TEST(ArrayTest, Swap) {
    mystl::array<int, 5> arr1 = {1, 2, 3, 4, 5};
    mystl::array<int, 5> arr2 = {5, 4, 3, 2, 1};
    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 5);
    EXPECT_EQ(arr2[0], 1);
}

/**
 * Test Case: Iterators
 */
TEST(ArrayTest, Iterators) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    int i = 1;
    for (auto it = arr.begin(); it != arr.end(); ++it, ++i) {
        EXPECT_EQ(*it, i);
    }
}

/**
 * Test Case: ConstIterators
 */
TEST(ArrayTest, ConstIterators) {
    const mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    int i = 1;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it, ++i) {
        EXPECT_EQ(*it, i);
    }
}


/**
 * Test Case: ReverseIterators
 */
TEST(ArrayTest, ReverseIterators) {
    mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    int i = 5;
    for (auto it = arr.rbegin(); it != arr.rend(); ++it, --i) {
        EXPECT_EQ(*it, i);
    }
}

/**
 * Test Case: ConstReverseIterators
 */
TEST(ArrayTest, ConstReverseIterators) {
    const mystl::array<int, 5> arr = {1, 2, 3, 4, 5};
    int i = 5;
    for (auto it = arr.crbegin(); it != arr.crend(); ++it, --i) {
        EXPECT_EQ(*it, i);
    }
}

/**
 * Test Case: Test for random_access_iterator concept in C++20
 */
TEST(ArrayTest, RandomAccessIteratorCategoryTest) {
    constexpr bool is_random_access = std::random_access_iterator<mystl::array<int, 10>::iterator>;
    EXPECT_TRUE(is_random_access) << "mystl::array::iterator must satisfy the random_access_iterator concept";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
