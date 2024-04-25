/**
 * \file test_vector.cpp
 */

#include "vector.hpp"
#include <gtest/gtest.h>


/**
 * Test Case: DefaultConstructor
 */
TEST(vectorTest, DefaultConstructor) {
    mystl::vector<int> vec;
    EXPECT_EQ(0, vec.size());
}


/**
 * Test Case: PushBackLValue
 */
TEST(vectorTest, PushBackLValue) {
    mystl::vector<int> vec;
    int value = 42;
    vec.push_back(value);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(42, vec[0]); 
}


/**
 * Test Case: PushBackRValue
 */
TEST(vectorTest, PushBackRValue) {
    mystl::vector<int> vec;
    vec.push_back(42);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(42, vec[0]);
}


/**
 * Test Case: EmplaceBack
 */
TEST(vectorTest, EmplaceBack) {
    mystl::vector<std::pair<int, int>> vec;
    vec.emplace_back(1, 2);
    EXPECT_EQ(1, vec.size());
    EXPECT_EQ(std::make_pair(1, 2), vec[0]);
}


/**
 * Test Case: pop_back
 */
TEST(vectorTest, pop_back) {
    mystl::vector<int> vec;
    vec.push_back(42);
    EXPECT_EQ(1, vec.size());
    vec.pop_back();
    EXPECT_EQ(true, vec.empty());
}


/**
 * Test Case: CorrectlyManagesResourcesWithDynamicAllocation
 *
 * Purpose:
 * This test is designed to verify the vector class's capability to safely destruct objects 
 * with dynamically allocated memory, and have been moved into the vector using push_back.
 *
 * Note: Failure of this test may result in a "pointer being freed was not
 * allocated" error message, indicating improper management of dynamic memory.
 */
TEST(vectorTest, CorrectlyManagesResourcesWithDynamicAllocation) {
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
    mystl::vector<MyClass> vec;
    vec.push_back(MyClass());
}


/**
 * Test Case: ConstructAndAssign
 */
TEST(vectorIteratorTest, ConstructAndAssign) {
    // 
    mystl::vector<int> vec = {1, 2, 3, 4, 5};
    mystl::vector<int>::iterator it = vec.begin();

    // 
    EXPECT_EQ(1, *it);

    // construct by copy
    mystl::vector<int>::iterator it2 = it;
    EXPECT_EQ(1, *it2);
}


/**
 * Test Case: Dereference
 */
TEST(vectorIteratorTest, Dereference) {
    // 
    mystl::vector<std::string> vec = {"123", "test"};
    mystl::vector<std::string>::iterator it = vec.begin();

    // 
    EXPECT_EQ(3, it->size());
    EXPECT_EQ("test", *(++it));
}


/**
 * Test case: Iteratoin
 */
TEST(vectorIteratorTest, Iteration) {
    // 
    mystl::vector<int> vec = {1, 2, 3, 4, 5};
    int i = 1;

    for (mystl::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        EXPECT_EQ(i++, *it);
    }
}


/**
 * Test case: IncrementAndDecrement
 */
TEST(vectorIteratorTest, IncrementAndDecrement) {
    // 
    mystl::vector<int> vec = {1, 2, 3};
    mystl::vector<int>::iterator it = vec.begin();

    // 
    EXPECT_EQ(2, *(++it));
    EXPECT_EQ(3, *(++it));
    EXPECT_EQ(2, *(--it));
}


/**
 * Test case: EqualityAndInequality
 */
TEST(vectorIteratorTest, EqualityAndInequality) {
    mystl::vector<int> vec = {1, 2};
    auto it1 = vec.begin();
    auto it2 = vec.begin();
    auto it3 = vec.end();

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
    EXPECT_FALSE(it1 == it3);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

