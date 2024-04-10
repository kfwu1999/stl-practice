/**
 * \file test_vector.cpp
 */

#include "vector.hpp"
#include <gtest/gtest.h>


/**
 * Test Case: DefaultConstructor
 */
TEST(VectorTest, DefaultConstructor) {
    mystl::Vector<int> vec;
    EXPECT_EQ(0, vec.size());
}


/**
 * Test Case: PushBackLValue
 */
TEST(VectorTest, PushBackLValue) {
    mystl::Vector<int> vec;
    int value = 42;
    vec.pushBack(value);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(42, vec[0]); 
}


/**
 * Test Case: PushBackRValue
 */
TEST(VectorTest, PushBackRValue) {
    mystl::Vector<int> vec;
    vec.pushBack(42);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(42, vec[0]);
}


/**
 * Test Case: EmplaceBack
 */
TEST(VectorTest, EmplaceBack) {
    mystl::Vector<std::pair<int, int>> vec;
    vec.emplaceBack(1, 2);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(std::make_pair(1, 2), vec[0]);
}


/**
 * Test Case: PopBack
 */
TEST(VectorTest, PopBack) {
    mystl::Vector<int> vec;
    vec.pushBack(42);
    EXPECT_EQ(1, vec.size());
    vec.popBack();
    EXPECT_EQ(true, vec.empty());
}


/**
 * Test Case: CorrectlyManagesResourcesWithDynamicAllocation
 *
 * Purpose:
 * This test is designed to verify the Vector class's capability to safely destruct objects 
 * with dynamically allocated memory, and have been moved into the vector using pushBack.
 *
 * Note: Failure of this test may result in a "pointer being freed was not
 * allocated" error message, indicating improper management of dynamic memory.
 */
TEST(VectorTest, CorrectlyManagesResourcesWithDynamicAllocation) {
    // 
    class MyClass {
    public:
        MyClass() 
            : p_int(new int[5]) {}

        MyClass(const MyClass& other) = delete;

        MyClass(MyClass&& other) {
            p_int = other.p_int;
            other.p_int = nullptr;
        }

        MyClass& operator=(const MyClass& other) = delete;

        MyClass& operator=(MyClass&& other) {
            p_int = other.p_int;
            other.p_int = nullptr;
            return *this;
        }

        ~MyClass() {
            delete[] p_int;
        }

    private:
        int* p_int = nullptr;
    };

    // 
    mystl::Vector<MyClass> vec;
    vec.pushBack(MyClass());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

