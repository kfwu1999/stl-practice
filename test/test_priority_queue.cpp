/**
 * \brief test_priority_queue.cpp
 */

#include <functional>
#include <gtest/gtest.h>

#include "vector.hpp"
#include "priority_queue.hpp"

// 
class PriorityQueueTests : public ::testing::Test {
protected:
    // 
    mystl::priority_queue<int> max_heap;
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap;
    mystl::vector<int> random_vec;

    // 
    void SetUp() {
        random_vec = {74, -42, 48, -44, 14, 5, 96, -98, -80, 18, 64, -38, -31, -36, 73, 25, -18, -45, -42, 30};
    }
};


// 
TEST_F(PriorityQueueTests, MaxHeapInsertAndCheckTop) {
    // 
    max_heap.push(1);
    EXPECT_EQ(1, max_heap.top());

    // 
    max_heap.push(10);
    EXPECT_EQ(10, max_heap.top());

    // 
    max_heap.push(5);
    EXPECT_EQ(10, max_heap.top());
}


// 
TEST_F(PriorityQueueTests, MinHeapInsertAndCheckTop) {
    // 
    min_heap.push(5);
    EXPECT_EQ(5, min_heap.top());

    // 
    min_heap.push(10);
    EXPECT_EQ(5, min_heap.top());

    // 
    min_heap.push(1);
    EXPECT_EQ(1, min_heap.top());
}


//
TEST_F(PriorityQueueTests, MaxHeapSize) {
    // 
    EXPECT_TRUE(max_heap.empty());

    // 
    max_heap.push(1);
    max_heap.push(1);
    max_heap.push(1);
    EXPECT_EQ(max_heap.size(), 3);

    // 
    max_heap.pop();
    EXPECT_EQ(max_heap.size(), 2);
    max_heap.pop();
    EXPECT_EQ(max_heap.size(), 1);
    max_heap.pop();
    EXPECT_TRUE(max_heap.empty());
}


//
TEST_F(PriorityQueueTests, MinHeapSize) {
    // 
    EXPECT_TRUE(min_heap.empty());

    // 
    min_heap.push(1);
    min_heap.push(1);
    min_heap.push(1);
    EXPECT_EQ(min_heap.size(), 3);

    // 
    min_heap.pop();
    EXPECT_EQ(min_heap.size(), 2);
    min_heap.pop();
    EXPECT_EQ(min_heap.size(), 1);
    min_heap.pop();
    EXPECT_TRUE(min_heap.empty());
}


// 
TEST_F(PriorityQueueTests, MaxHeapPopAndCheckTop) {
    //
    max_heap.push(1);
    max_heap.push(2);
    max_heap.push(3);
    max_heap.push(3);

    // 
    max_heap.pop();
    EXPECT_EQ(max_heap.top(), 3);
    max_heap.pop();
    EXPECT_EQ(max_heap.top(), 2);
    max_heap.pop();
    EXPECT_EQ(max_heap.top(), 1);
    max_heap.pop();
    EXPECT_TRUE(max_heap.empty());
}


// 
TEST_F(PriorityQueueTests, MinHeapPopAndCheckTop) {
    //
    min_heap.push(1);
    min_heap.push(1);
    min_heap.push(2);
    min_heap.push(3);

    // 
    min_heap.pop();
    EXPECT_EQ(min_heap.top(), 1);
    min_heap.pop();
    EXPECT_EQ(min_heap.top(), 2);
    min_heap.pop();
    EXPECT_EQ(min_heap.top(), 3);
    min_heap.pop();
    EXPECT_TRUE(min_heap.empty());
}


// 
TEST_F(PriorityQueueTests, MaxHeapConstructByVector) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), random_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!max_heap_by_vec.empty()) {
        EXPECT_EQ(max_heap_by_vec.top(), random_vec[idx]);
        max_heap_by_vec.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MinHeapConstructByVector) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!min_heap_by_vec.empty()) {
        EXPECT_EQ(min_heap_by_vec.top(), random_vec[idx]);
        min_heap_by_vec.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MaxHeapConstructByMoveVector) {
    // 
    mystl::vector<int> vec_to_be_moved = random_vec;
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), std::move(vec_to_be_moved));

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!max_heap_by_vec.empty()) {
        EXPECT_EQ(max_heap_by_vec.top(), random_vec[idx]);
        max_heap_by_vec.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MinHeapConstructByMoveVector) {
    // 
    mystl::vector<int> vec_to_be_moved = random_vec;
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), std::move(vec_to_be_moved));

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!min_heap_by_vec.empty()) {
        EXPECT_EQ(min_heap_by_vec.top(), random_vec[idx]);
        min_heap_by_vec.pop();
        idx++;
    }
}


//
TEST_F(PriorityQueueTests, MaxHeapCopyConstruct) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), random_vec);
    mystl::priority_queue<int> moved_max_heap(max_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!moved_max_heap.empty()) {
        EXPECT_EQ(moved_max_heap.top(), random_vec[idx]);
        moved_max_heap.pop();
        idx++;
    }
}


//
TEST_F(PriorityQueueTests, MinHeapCopyConstruct) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);
    auto moved_min_heap(min_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!moved_min_heap.empty()) {
        EXPECT_EQ(moved_min_heap.top(), random_vec[idx]);
        moved_min_heap.pop();
        idx++;
    }
}


//
TEST_F(PriorityQueueTests, MaxHeapMoveConstruct) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), random_vec);
    mystl::priority_queue<int> moved_max_heap(max_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!moved_max_heap.empty()) {
        EXPECT_EQ(moved_max_heap.top(), random_vec[idx]);
        moved_max_heap.pop();
        idx++;
    }
}


//
TEST_F(PriorityQueueTests, MinHeapMoveConstruct) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);
    auto moved_min_heap(min_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!moved_min_heap.empty()) {
        EXPECT_EQ(moved_min_heap.top(), random_vec[idx]);
        moved_min_heap.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MaxHeapConstructByVectorRange) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(random_vec.begin(), random_vec.end());

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!max_heap_by_vec.empty()) {
        EXPECT_EQ(max_heap_by_vec.top(), random_vec[idx]);
        max_heap_by_vec.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MinHeapConstructByVectorRange) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!min_heap_by_vec.empty()) {
        EXPECT_EQ(min_heap_by_vec.top(), random_vec[idx]);
        min_heap_by_vec.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MaxHeapCopyAssignment) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), random_vec);
    mystl::priority_queue<int> copied_max_heap;
    copied_max_heap = max_heap_by_vec;

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!copied_max_heap.empty()) {
        EXPECT_EQ(copied_max_heap.top(), random_vec[idx]);
        copied_max_heap.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MinHeapCopyAssignment) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> copied_min_heap;
    copied_min_heap = min_heap_by_vec;

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!copied_min_heap.empty()) {
        EXPECT_EQ(copied_min_heap.top(), random_vec[idx]);
        copied_min_heap.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MaxHeapMoveAssignment) {
    // 
    mystl::priority_queue<int> max_heap_by_vec(std::less<int>(), random_vec);
    mystl::priority_queue<int> moved_max_heap;
    moved_max_heap = std::move(max_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end(), std::greater<int>());

    // 
    int idx = 0;
    while (!moved_max_heap.empty()) {
        EXPECT_EQ(moved_max_heap.top(), random_vec[idx]);
        moved_max_heap.pop();
        idx++;
    }
}


// 
TEST_F(PriorityQueueTests, MinHeapMoveAssignment) {
    // 
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> min_heap_by_vec(std::greater<int>(), random_vec);
    mystl::priority_queue<int, mystl::vector<int>, std::greater<int>> moved_min_heap;
    moved_min_heap = std::move(min_heap_by_vec);

    // 
    std::sort(random_vec.begin(), random_vec.end());

    // 
    int idx = 0;
    while (!moved_min_heap.empty()) {
        EXPECT_EQ(moved_min_heap.top(), random_vec[idx]);
        moved_min_heap.pop();
        idx++;
    }
}


// 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
