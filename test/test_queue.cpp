/**
 */

#include <gtest/gtest.h>
#include <utility>

#include "queue.hpp"


/* Constructors and Destructors */
TEST(QueueTest, DefaultConstructor) {
    mystl::queue<int> que;
    EXPECT_TRUE(que.empty());
}


TEST(QueueTest, CopyConstructor) {
    // 
    mystl::queue<int> que_ref;
    que_ref.push(1);

    // 
    mystl::queue<int> que_copied(que_ref);
    EXPECT_EQ(que_ref.front(), que_copied.front());
    EXPECT_EQ(que_ref.size(), que_copied.size());

    // 
    que_ref.push(2);
    EXPECT_NE(que_ref.back(), que_copied.back());
    EXPECT_NE(que_ref.size(), que_copied.size());
}


TEST(QueueTest, MoveConstructor) {
    // 
    mystl::queue<int> que_ref;
    que_ref.push(1);
    que_ref.push(2);

    // 
    mystl::queue<int> que_moved(std::move(que_ref));
    EXPECT_EQ(que_moved.back(), 2);
    EXPECT_EQ(que_moved.size(), 2);
}


/* Operators */
TEST(QueueTest, CopyAssignmentOperator) {
    // 
    mystl::queue<int> que_ref;
    mystl::queue<int> que_copied;
    que_ref.push(1);
    que_ref.push(2);
    que_ref.push(3);

    // 
    que_copied = que_ref;
    EXPECT_EQ(que_ref.back(), que_copied.back());
    EXPECT_EQ(que_ref.size(), que_copied.size());

    //
    que_ref.push(2);
    EXPECT_NE(que_ref.size(), que_copied.size());
}


TEST(QueueTest, MoveAssignmentOperator) {
    // 
    mystl::queue<int> que_ref;
    mystl::queue<int> que_moved;
    que_ref.push(1);
    que_ref.push(2);

    // 
    que_moved = std::move(que_ref);
    EXPECT_EQ(que_moved.back(), 2);
    EXPECT_EQ(que_moved.size(), 2);
}


/* Element access */
TEST(QueueTest, FrontBack) {
    // 
    mystl::queue<int> que;

    // 
    que.push(1);
    EXPECT_EQ(que.front(), 1);

    que.push(2);
    EXPECT_EQ(que.back(), 2);

    que.push(3);
    EXPECT_EQ(que.front(), 1);
    EXPECT_EQ(que.back(), 3);
}


/* Modifiers */
TEST(QueueTest, Push) {
    // 
    mystl::queue<int> que;

    // 
    que.push(1);
    EXPECT_EQ(que.front(), 1);
    EXPECT_EQ(que.size(), 1);

    que.push(2);
    EXPECT_EQ(que.back(), 2);
    EXPECT_EQ(que.size(), 2);

    que.push(3);
    EXPECT_EQ(que.back(), 3);
    EXPECT_EQ(que.size(), 3);
}


TEST(QueueTest, Emplace) {
    // 
    mystl::queue<std::pair<int, int>> que;

    // 
    que.emplace(1, 1);
    EXPECT_EQ(que.front(), std::make_pair(1, 1));
    EXPECT_EQ(que.size(), 1);

    que.emplace(2, 2);
    EXPECT_EQ(que.back(), std::make_pair(2, 2));
    EXPECT_EQ(que.size(), 2);

    que.emplace(3, 3);
    EXPECT_EQ(que.back(), std::make_pair(3, 3));
    EXPECT_EQ(que.size(), 3);
}


TEST(QueueTest, Pop) {
    // 
    mystl::queue<int> que;

    // 
    que.push(1);
    que.push(2);
    que.push(3);
    EXPECT_EQ(que.size(), 3);

    que.pop();
    EXPECT_EQ(que.front(), 2);

    que.pop();
    EXPECT_EQ(que.front(), 3);

    que.pop();
    EXPECT_TRUE(que.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
