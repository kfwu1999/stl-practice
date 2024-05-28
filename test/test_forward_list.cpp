/**
 */

#include <stdexcept>
#include <gtest/gtest.h>
#include <utility>
#include "forward_list.hpp"


/* Constructors and Destructors */
TEST(ForwardListTest, DefaultConstructorAndDestructor) {
    mystl::forward_list<int> list;
    EXPECT_EQ(list.size(), 0);
}


TEST(ForwardListTest, CopyConstructor) {
    // 
    mystl::forward_list<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    // 
    mystl::forward_list<int> copied_list(list);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(copied_list.front(), 1);
    EXPECT_EQ(copied_list.size(), 3);

    // the nodes in two list should be independent
    for (auto it = list.begin(), copied_it = copied_list.begin(); it != list.end(); ++it, ++copied_it) {
        *it += 1;
        EXPECT_NE(*it, *copied_it);
    }
}


TEST(ForwardListTest, MoveConstructor) {
    // 
    mystl::forward_list<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    mystl::forward_list<int> moved_list(std::move(list));
    EXPECT_EQ(moved_list.front(), 1);
    EXPECT_EQ(moved_list.size(), 3);
}


TEST(ForwardListTest, InitializerConstructor) {
    mystl::forward_list<int> list = {1, 2, 3};

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
}


TEST(ForwardListTest, Front) {
    // 
    mystl::forward_list<int> list;

    // 
    EXPECT_THROW(list.front(), std::out_of_range);

    // 
    list.push_front(5);
    EXPECT_EQ(list.front(), 5);
}


/* Iterators */
TEST(ForwardListIteratorTest, Increment) {
    mystl::forward_list<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    mystl::forward_list<int>::iterator it = list.begin();
    EXPECT_EQ(*(it++), 1);
    EXPECT_EQ(*(it++), 2);
    EXPECT_EQ(*it, 3);
}


TEST(ForwardListIteratorTest, Iteration) {
    mystl::forward_list<int> list;
    list.push_front(5);
    list.push_front(4);
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    int i = 1;
    for (mystl::forward_list<int>::iterator it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, i++);
    }
}


TEST(ForwardListIteratorTest, IterationByConst) {
    mystl::forward_list<int> list;
    list.push_front(5);
    list.push_front(4);
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    int i = 1;
    for (mystl::forward_list<int>::const_iterator it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, i++);
    }
}


/* Modifiers */
TEST(ForwardListTest, InsertAfter) {
    // 
    mystl::forward_list<int> list;
    EXPECT_THROW(list.insert_after(list.cbegin(), 1), std::logic_error);
    list.push_front(1);

    // 
    list.insert_after(list.cbegin(), 3);
    auto it = list.insert_after(list.cbegin(), 2);
    EXPECT_EQ(*it, 2);

    // 
    int i = 1;
    for (auto it = list.cbegin(); it != list.cend(); ++it)
        EXPECT_EQ(*it, i++);
    EXPECT_EQ(list.size(), 3);
}


TEST(ForwardListTest, InsertAfterWithCount) {
    mystl::forward_list<int> list;
    EXPECT_THROW(list.insert_after(list.cbegin(), 1, 0), std::logic_error);
    list.insert_after(list.cbefore_begin(), 3, 1);
    EXPECT_EQ(list.size(), 3);
    while (!list.empty()) {
        EXPECT_EQ(list.front(), 1);
        list.pop_front();
    }
}


TEST(ForwardListTest, EmplaceAfter) {
    // 
    mystl::forward_list<std::pair<int, int>> list;
    EXPECT_THROW(list.emplace_after(list.cbegin(), 1, 1), std::logic_error);

    // 
    list.emplace_after(list.cbefore_begin(), 1, -1);   // sentinel -> (1,-1)
    list.emplace_after(list.cbegin(), 3, -3);          // sentinel -> (1,-1) -> (3,-3)
    list.emplace_after(list.cbegin(), 2, -2);          // sentinel -> (1,-1) -> (2,-2) -> (3,-3)
    EXPECT_EQ(list.size(), 3);

    // 
    int i = 1;
    for (auto it = list.cbegin(); it != list.cend(); ++it, ++i)
        EXPECT_EQ(*it, std::make_pair(i, -i));
}


TEST(ForwardListTest, EraseAfter) {
    // 
    mystl::forward_list<int> list;

    // 
    EXPECT_THROW(list.erase_after(list.cbegin()), std::logic_error);   // list.cbegin() is empty
    list.emplace_after(list.cbefore_begin(), 1);   // sentinel -> 1

    // 
    EXPECT_THROW(list.erase_after(list.cbegin()), std::logic_error);   // next of list.cbegin() is empty
    list.emplace_after(list.cbegin(), 3);          // sentinel -> 1 -> 3
    list.emplace_after(list.cbegin(), 2);          // sentinel -> 1 -> 2 -> 3

    // 
    auto it = list.erase_after(list.cbefore_begin());
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(list.size(), 2);
}


TEST(ForwardListTest, EraseAfterRange) {
    // 
    mystl::forward_list<int> list;

    // 
    EXPECT_THROW(list.erase_after(list.cbegin(), list.cend()), std::logic_error);   // Empty range
    list.emplace_after(list.cbefore_begin(), 1);   // sentinel -> 1

    // 
    EXPECT_THROW(list.erase_after(list.cbegin(), list.cend()), std::logic_error);   // Range contains only one element
    list.emplace_after(list.cbegin(), 3);          // sentinel -> 1 -> 3
    list.emplace_after(list.cbegin(), 2);          // sentinel -> 1 -> 2 -> 3

    // 
    auto first = list.cbegin();
    auto last = first;
    std::advance(last, 2);
    auto it = list.erase_after(first, last);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(list.size(), 2);
}


TEST(ForwardListTest, PushFront) {
    mystl::forward_list<int> list;
    list.push_front(1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.size(), 1);
    list.push_front(2);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2);
    list.push_front(3);
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.size(), 3);
}


TEST(ForwardListTest, EmplaceFront) {
    mystl::forward_list<std::pair<int, int>> list;
    list.emplace_front(1, -1);
    EXPECT_EQ(list.front(), std::make_pair(1, -1));
    EXPECT_EQ(list.size(), 1);
    list.emplace_front(2, -2);
    EXPECT_EQ(list.front(), std::make_pair(2, -2));
    EXPECT_EQ(list.size(), 2);
}


TEST(ForwardListTest, PopFront) {
    mystl::forward_list<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.pop_front();
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2);
    list.pop_front();
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.size(), 1);
    list.pop_front();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.pop_front(), std::out_of_range);
}


TEST(ForwardListTest, Resize) {
    // 
    mystl::forward_list<int> list;

    // 
    list.resize(5, 1);
    EXPECT_EQ(list.size(), 5);
    int i = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it, ++i) {
        EXPECT_EQ(*it, 1);
    }

    // shrink size
    list.resize(2, 1);
    EXPECT_EQ(list.size(), 2);
    i = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it, ++i) {
        EXPECT_EQ(*it, 1);
    }

    // does nothing if resize to same size
    list.resize(2);
    EXPECT_EQ(list.size(), 2);
}


TEST(ForwardListTest, Swap) {
    // Create two forward lists
    mystl::forward_list<int> list1 = {1, 2, 3};
    mystl::forward_list<int> list2 = {4, 8, 7, 6, 3};

    // Swap the lists
    list1.swap(list2);

    // Check the contents of list1 after swap
    std::vector<int> expected_list1 = {4, 8, 7, 6, 3};
    int i = 0;
    for (auto it = list1.cbegin(); it != list1.cend(); ++it, ++i) {
        EXPECT_EQ(*it, expected_list1[i]);
    }

    // Check the contents of list2 after swap
    std::vector<int> expected_list2 = {1, 2, 3};
    i = 0;
    for (auto it = list2.cbegin(); it != list2.cend(); ++it, ++i) {
        EXPECT_EQ(*it, expected_list2[i]);
    }
}


/* Operations */


/**/
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
