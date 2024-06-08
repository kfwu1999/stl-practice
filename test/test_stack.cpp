/**
 * \file test_stack.hpp
 */

#include <gtest/gtest.h>
#include <utility>

#include "stack.hpp"
#include "list.hpp"


/* Constructors and Destructors */
TEST(StackTest, DefaultConstructor) {
    mystl::stack<int> stk;
    EXPECT_TRUE(stk.empty());
}


TEST(StackTest, CopyConstructor) {
    // 
    mystl::stack<int> stk_ref;
    stk_ref.push(1);

    // 
    mystl::stack<int> stk_copied(stk_ref);
    EXPECT_EQ(stk_ref.top(), stk_copied.top());
    EXPECT_EQ(stk_ref.size(), stk_copied.size());

    // 
    stk_ref.push(2);
    EXPECT_NE(stk_ref.top(), stk_copied.top());
    EXPECT_NE(stk_ref.size(), stk_copied.size());
}


TEST(StackTest, MoveConstructor) {
    // 
    mystl::stack<int> stk_ref;
    stk_ref.push(1);
    stk_ref.push(2);

    // 
    mystl::stack<int> stk_moved(std::move(stk_ref));
    EXPECT_EQ(stk_moved.top(), 2);
    EXPECT_EQ(stk_moved.size(), 2);
}


/* Operators */
TEST(StackTest, CopyAssignmentOperator) {
    // 
    mystl::stack<int> stk_ref;
    mystl::stack<int> stk_copied;
    stk_ref.push(1);
    stk_ref.push(2);
    stk_ref.push(3);

    // 
    stk_copied = stk_ref;

    // 
    EXPECT_EQ(stk_ref.top(), stk_copied.top());
    EXPECT_EQ(stk_ref.size(), stk_copied.size());

    // 
    stk_ref.push(2);
    EXPECT_NE(stk_ref.top(), stk_copied.top());
    EXPECT_NE(stk_ref.size(), stk_copied.size());
}


TEST(StackTest, MoveAssignmentOperator) {
    // 
    mystl::stack<int> stk_ref;
    mystl::stack<int> stk_moved;
    stk_ref.push(1);
    stk_ref.push(2);

    // 
    stk_moved = std::move(stk_ref);

    // 
    EXPECT_EQ(stk_moved.top(), 2);
    EXPECT_EQ(stk_moved.size(), 2);
}


/* Element access */
TEST(StackTest, Top) {
    // 
    mystl::stack<int> stk;

    // 
    stk.push(1);
    EXPECT_EQ(stk.top(), 1);
    stk.push(2);
    EXPECT_EQ(stk.top(), 2);
    stk.push(3);
    EXPECT_EQ(stk.top(), 3);
}


/* Modifiers */
TEST(StackTest, Push) {
    // 
    mystl::stack<int> stk;

    // 
    stk.push(1);
    EXPECT_EQ(stk.top(), 1);
    EXPECT_EQ(stk.size(), 1);

    stk.push(2);
    EXPECT_EQ(stk.top(), 2);
    EXPECT_EQ(stk.size(), 2);

    stk.push(3);
    EXPECT_EQ(stk.top(), 3);
    EXPECT_EQ(stk.size(), 3);
}


TEST(StackTest, Emplace) {
    // 
    mystl::stack<std::pair<int, int>> stk;

    // 
    stk.emplace(1, 1);
    EXPECT_EQ(stk.top(), std::make_pair(1, 1));
    EXPECT_EQ(stk.size(), 1);

    stk.emplace(2, 2);
    EXPECT_EQ(stk.top(), std::make_pair(2, 2));
    EXPECT_EQ(stk.size(), 2);

    stk.emplace(3, 3);
    EXPECT_EQ(stk.top(), std::make_pair(3, 3));
    EXPECT_EQ(stk.size(), 3);
}


TEST(StackTest, Pop) {
    // 
    mystl::stack<int> stk;

    // 
    stk.push(1);
    stk.push(2);
    stk.push(3);
    EXPECT_EQ(stk.size(), 3);

    // 
    stk.pop();
    EXPECT_EQ(stk.top(), 2);

    stk.pop();
    EXPECT_EQ(stk.top(), 1);

    stk.pop();
    EXPECT_TRUE(stk.empty());
}


/* Use another container */
TEST(StackTest, UseListAsContainerType) {
    // 
    mystl::stack<int, mystl::list<int>> stk;

    // 
    stk.push(1);
    stk.push(2);
    stk.push(3);
    EXPECT_EQ(stk.top(), 3);
    EXPECT_EQ(stk.size(), 3);

    // 
    stk.pop();
    EXPECT_EQ(stk.top(), 2);
    EXPECT_EQ(stk.size(), 2);

    stk.pop();
    EXPECT_EQ(stk.top(), 1);
    EXPECT_EQ(stk.size(), 1);

    stk.pop();
    EXPECT_TRUE(stk.empty());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
