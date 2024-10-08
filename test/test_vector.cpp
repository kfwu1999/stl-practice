/**
 * \file test_vector.cpp
 */

#include <algorithm>      // std::sort
#include <iterator>
#include <stdexcept>
#include <forward_list>
#include <list>

#include <gtest/gtest.h>

#include "vector.hpp"


/**
 * Test Case: DefaultConstructor
 */
TEST(vectorTest, DefaultConstructor) {
    mystl::vector<int> vec;
    EXPECT_EQ(0, vec.size());
}


/**
 * Test Case: ConstructByCountOfCopies
 */
TEST(vectorTest, ConstructByCountOfCopies) {
    // construct by given value
    mystl::vector<int> vec(15, 1);
    ASSERT_EQ(vec.size(), 15);
    EXPECT_EQ(vec.capacity(), 15);
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], 1);
    }

    // construct by default-constructed value_type
    mystl::vector<int> vec2(15);
    ASSERT_EQ(vec2.size(), 15);
    EXPECT_EQ(vec2.capacity(), 15);
    for (size_t i = 0; i < vec2.size(); ++i) {
        EXPECT_EQ(vec2[i], 0);
    }
}


/**
 * Test Case: ConstructByRange
 */
TEST(vectorTest, ConstructByRange) {
    // Construct by raw array
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    mystl::vector<int> vec(arr, arr + 10);
    ASSERT_EQ(vec.size(), 10);
    for (size_t i = 0; i < 10; ++i) {
        ASSERT_EQ(vec[i], i) << "Vectors differ at index " << i << " when construct by range of raw array";
    }

    // Construct by mystl::vector (Random Access Iterator)
    mystl::vector<int> newVec(vec.begin(), vec.end());
    ASSERT_EQ(newVec.size(), 10);
    for (size_t i = 0; i < 10; ++i) {
        ASSERT_EQ(newVec[i], i) << "Vectors differ at index " << i << "when construct by range of mystl::vector";
    }

    // Construct by range of forward_list (Forward Iterator)
    std::forward_list<int> flist = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    mystl::vector<int> vec_by_flist(flist.begin(), flist.end());
    ASSERT_EQ(vec_by_flist.size(), 10);
    std::forward_list<int>::iterator flit = flist.begin();
    for (size_t i = 0; i < 10 && flit != flist.end(); ++i, ++flit) {
        ASSERT_EQ(vec_by_flist[i], *flit) << "Vectors differ at index " << i << " when construct by range of std::forward_list";
    }

    // Construct by range of list (Bidirectional Iterator)
    std::list<int> list = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    mystl::vector<int> vec_by_list(flist.begin(), flist.end());
    ASSERT_EQ(vec_by_flist.size(), 10);
    std::list<int>::iterator lit = list.begin();
    for (size_t i = 0; i < 10 && lit != list.end(); ++i, ++lit) {
        ASSERT_EQ(vec_by_flist[i], *lit) << "Vectors differ at index " << i << " when construct by range of std::list";
    }
}


/**
 * Test Case: Copy constructor
 */
TEST(vectorTest, CopyConstructor) {
    mystl::vector<int> original{1, 2, 3, 4, 5};
    mystl::vector<int> copy = original;

    ASSERT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]) << "Vectors differ at index " << i;
    }
}


/**
 * Test Case: Move constructor
 */
TEST(vectorTest, MoveConstructor) {
    mystl::vector<int> original{1, 2, 3};
    mystl::vector<int> moved = std::move(original);

    ASSERT_EQ(moved.size(), 3);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);

    // 
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.capacity(), 0);
}


/**
 * Test Case: Initializer constructor
 */
TEST(vectorTest, InitializerConstructor) {
    mystl::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQ(vec.size(), 11);
    EXPECT_EQ(vec.capacity(), 11);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec[2], 2);
}


/**
 * Test Case: CopyAssignment
 */
TEST(vectorTest, CopyAssignment) {
    mystl::vector<int> original{1, 2, 3};
    mystl::vector<int> copy;
    copy = original;   // copy assignment

    ASSERT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]) << "Copy assignment failed at index " << i;
    }

    // Modify copy and check original remains unchanged
    copy[0] = 10;
    EXPECT_EQ(original[0], 1) << "Original vector modified after copy assignment.";
}


/**
 * Test Case: MoveAssignment
 */
TEST(vectorTest, MoveAssignment) {
    mystl::vector<int> original{1, 2, 3};
    mystl::vector<int> moved;
    moved = std::move(original);   // move assignment

    ASSERT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);

    // Ensure the original vector is in a valid empty state
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.capacity(), 0);
}


/**
 * Test Case: AtThrowsOutOfRange
 */
TEST(vectorTest, AtThrowsOutOfRange) {
    mystl::vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.at(0), 1);
    EXPECT_THROW(vec.at(3), std::out_of_range);
}


/**
 */
TEST(vectorTest, Front) {
    mystl::vector<int> vec;
    EXPECT_THROW(vec.front(), std::out_of_range);
    vec.push_back(1);
    EXPECT_EQ(vec.front(), *vec.cbegin());
}


/**
 */
TEST(vectorTest, Back) {
    mystl::vector<int> vec;
    EXPECT_THROW(vec.back(), std::out_of_range);
    vec.push_back(1);
    EXPECT_EQ(vec.back(), *std::prev(vec.cend()));
}


/**
 */
TEST(vectorTest, Reserve) {
    mystl::vector<int> vec;
    vec.reserve(100);
    EXPECT_EQ(vec.capacity(), 100);
    EXPECT_EQ(vec.size(), 0) << "The size of vector changed after reserve";
}


/**
 * Test Case: ShrinkToFit
 */
TEST(vectorTest, ShrinkToFit) {
    mystl::vector<int> vec;
    for (int i = 0; i < 100; ++i)
        vec.push_back(i);
    int valBefore = vec[99];
    vec.shrink_to_fit();
    int valAfter = vec[99];
    ASSERT_EQ(vec.capacity(), 100);
    EXPECT_EQ(vec.size(), 100) << "The size changed after shrink_to_fit";
    EXPECT_EQ(valBefore, valAfter) << "The value of last element changed after shrink_to_fit";
}


/**
 */
TEST(vectorTest, Erase) {
    // 
    mystl::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    vec.erase(vec.cbegin());
    EXPECT_EQ(vec.size(), 9);
    EXPECT_EQ(vec[0], 1);
    EXPECT_THROW(vec.erase(vec.cbegin() - 1), std::out_of_range);

    vec.erase(vec.cbegin() + 2, vec.cbegin() + 5);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 6);
    EXPECT_THROW(vec.erase(vec.cbegin() + 2, vec.cbegin() + 10), std::out_of_range);
}


TEST(vectorTest, Emplace) {
    mystl::vector<int> vec = {1, 2, 4, 5};

    // emplace at first
    auto it = vec.emplace(vec.cbegin(), 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);

    // emplace at middle
    it = vec.emplace(vec.cbegin() + 3, 3);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[3], 3);

    // emplace at end
    it = vec.emplace(vec.cend(), 6);
    EXPECT_EQ(*it, 6);
    EXPECT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[6], 6);
}


TEST(vectorTest, Insert) {
    mystl::vector<int> vec = {1, 2, 3, 5};

    // Test using const_reference input
    auto it = vec.insert(vec.cbegin(), 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 0);

    // insert at middle
    it = vec.insert(vec.cbegin() + 3, 3);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec[3], 3);

    // insert at end
    it = vec.insert(vec.cend(), 6);
    EXPECT_EQ(*it, 6);
    EXPECT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[6], 6);
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
    EXPECT_THROW(vec.pop_back(), std::length_error);
}


/**
 */
TEST(vectorTest, Resize) {
    mystl::vector<int> vec;
    for (std::size_t i = 0; i < 20; ++i)
        vec.push_back(i);

    vec.resize(25);
    ASSERT_EQ(vec.size(), 25);
    EXPECT_EQ(vec[24], 0) << "The value of appended default-inserted elements is wrong.";

    vec.resize(30, 10);
    ASSERT_EQ(vec.size(), 30);
    EXPECT_EQ(vec[29], 10) << "The value of appended elements is wrong";

    vec.resize(10);
    ASSERT_EQ(vec.size(), 10);
}


/**
 */
TEST(vectorTest, SwapVectors) {
    // 
    mystl::vector<int> vec1 = {0, 1, 2, 3};
    mystl::vector<int> vec2 = {4, 5, 6};

    // Record original sizes and capacities
    auto size1 = vec1.size();
    auto size2 = vec2.size();
    auto capacity1 = vec1.capacity();
    auto capacity2 = vec2.capacity();

    // Swap vectors
    swap(vec1, vec2);

    // Check sizes and capacities swapped
    EXPECT_EQ(vec1.size(), size2);
    EXPECT_EQ(vec2.size(), size1);
    EXPECT_EQ(vec1.capacity(), capacity2);
    EXPECT_EQ(vec2.capacity(), capacity1);

    // Check contents swapped
    EXPECT_EQ(vec1[0], 4);
    EXPECT_EQ(vec1[1], 5);
    EXPECT_EQ(vec1[2], 6);

    EXPECT_EQ(vec2[0], 0);
    EXPECT_EQ(vec2[1], 1);
    EXPECT_EQ(vec2[2], 2);
    EXPECT_EQ(vec2[3], 3);

    // Ensure no excess elements
    EXPECT_THROW(vec1.at(3), std::out_of_range);
    EXPECT_THROW(vec2.at(4), std::out_of_range);
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
 * Test Case: SortUsingStdSort
 * 
 * This test checks if mystl::vector can be used with the std::sort algorithm
 * to correctly sort its elements.
 */
TEST(VectorTest, SortUsingStdSort) {
    // 
    mystl::vector<int> vec = {4, 4, 8, 7, 6, 3, 3};

    // Using std::sort to sort the vector
    std::sort(vec.begin(), vec.end());

    // Check if the vector is sorted
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        EXPECT_LE(vec[i], vec[i + 1]) << "Vector is not sorted at index " << i;
    }
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


/**
 * Test Case: ReverseIterators
 */
TEST(vectorIteratorTest, ReverseIterators) {
    mystl::vector<int> vec = {1, 2, 3, 4, 5};
    int i = 5;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it, --i) {
        EXPECT_EQ(*it, i);
    }
}


/**
 * Test Case: ConstReverseIterators
 */
TEST(vectorIteratorTest, ConstReverseIterators) {
    const mystl::vector<int> vec = {1, 2, 3, 4, 5};
    int i = 5;
    for (auto it = vec.crbegin(); it != vec.crend(); ++it, --i) {
        EXPECT_EQ(*it, i);
    }
}


/**
 * Test Case: Test for random_access_iterator concept in C++20
 */
TEST(vectorIteratorTest, RandomAccessIteratorCategoryTest) {
    constexpr bool is_random_access = std::random_access_iterator<mystl::vector<int>::iterator>;
    EXPECT_TRUE(is_random_access) << "mystl::vector::iterator must satisfy the random_access_iterator concept";
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

