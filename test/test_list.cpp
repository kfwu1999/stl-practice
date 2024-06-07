/**
 */
#include <gtest/gtest.h>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "list.hpp"

/* Constructors and Destructors */
TEST(ListTest, DefaultConstructorAndDestructor) {
    mystl::list<int> list;
    EXPECT_EQ(list.size(), 0);
}


TEST(ListTest, ConstructByCount) {
    // 
    int count = 10, value = 10;
    mystl::list<int> list1(count, value);

    // 
    for (auto it = list1.cbegin(); it != list1.cend(); ++it) {
        EXPECT_EQ(*it, value) << "Mismatch in forward traversal";
    }

    for (auto it = list1.crbegin(); it != list1.crend(); ++it) {
        EXPECT_EQ(*it, value) << "Mismatch in backward traversal";
    }

    // 
    mystl::list<int> list2(count);

    // 
    for (auto it = list2.cbegin(); it != list2.cend(); ++it) {
        EXPECT_EQ(*it, mystl::list<int>::value_type()) << "Mismatch in forward traversal";
    }

    for (auto it = list2.crbegin(); it != list2.crend(); ++it) {
        EXPECT_EQ(*it, mystl::list<int>::value_type()) << "Mismatch in backward traversal";
    }
}


TEST(ListTest, ConstructByRange) {
    // 
    const std::vector<int> expected = {1, 2, 3, 4, 5};
    mystl::list<int> list1(expected.begin(), expected.end());

    // 
    int idx = 0;
    for (auto it = list1.cbegin(); it != list1.cend(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in forward traversal";
    }

    idx = static_cast<int>(expected.size()) - 1;
    for (auto it = list1.crbegin(); it != list1.crend(); ++it, --idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in backward traversal";
    }
}


TEST(ListTest, CopyConstructor) {
    // 
    const std::vector<int> expected = {1, 2, 3, 4, 5};
    mystl::list<int> ref_list(expected.begin(), expected.end());

    // 
    mystl::list<int> copied_list(ref_list);
    EXPECT_EQ(ref_list.size(), copied_list.size());

    // forward
    for (auto rit = ref_list.begin(), cit = copied_list.begin();
         cit != copied_list.end(); ++rit, ++cit) 
    {
        EXPECT_EQ(*rit, *cit) << "Mismatch in forward traversal";
        (*rit)++;
        EXPECT_NE(*rit, *cit) << "Nodes in two lists should be independent after modification";
    }

    // backward
    for (auto rit = ref_list.rbegin(), cit = copied_list.rbegin();
         cit != copied_list.rend(); ++rit, ++cit) 
    {
        EXPECT_EQ(*rit - 1, *cit) << "Mismatch in backward traversal";
        (*rit)++;
        EXPECT_NE(*rit, *cit) << "Nodes in two lists should be independent after modification";
    }
}


TEST(ListTest, MoveConstructor) {
    // 
    const std::vector<int> expected = {1, 2, 3, 4, 5};
    mystl::list<int> ref_list(expected.begin(), expected.end());

    // 
    mystl::list<int> moved_list(ref_list);
    EXPECT_EQ(expected.size(), moved_list.size());

    // forward
    int idx = 0;
    for (auto it = moved_list.cbegin(); it != moved_list.cend(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in forward traversal at index: " << idx;
    }

    // backward
    idx = expected.size() - 1;
    for (auto it = moved_list.crbegin(); it != moved_list.crend(); ++it, --idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in backward traversal at index: " << idx;
    }
}


TEST(ListTest, InitializerConstructor) {
    // 
    mystl::list<int> myList = {10, 20, 30, 40, 50};

    // 
    EXPECT_EQ(myList.size(), 5);

    // 
    std::vector<int> expected = {10, 20, 30, 40, 50};
    int idx = 0;
    for (auto it = myList.begin(); it != myList.end(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in forward traversal at index: " << idx;
    }

    idx = expected.size() - 1;
    for (auto it = myList.crbegin(); it != myList.crend(); ++it, --idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in backward traversal at index: " << idx;
    }
}


/* Operators */
TEST(ListTest, CopyAssignment) {
    // 
    const std::vector<int> expected = {1, 2, 3, 4, 5};
    mystl::list<int> ref_list(expected.begin(), expected.end());

    // 
    mystl::list<int> copied_list;
    copied_list = ref_list;  // Copy assignment
    EXPECT_EQ(ref_list.size(), copied_list.size());

    // 
    auto rit = ref_list.begin(), cit = copied_list.begin();
    for (; cit != copied_list.end(); ++rit, ++cit) {
        EXPECT_EQ(*rit, *cit) << "Mismatch in forward traversal";
        (*rit)++;
        EXPECT_NE(*rit, *cit) << "Nodes in two lists should be independent after modification";
    }

    // 
    auto rrit = ref_list.rbegin(), rcit = copied_list.rbegin();
    for (; rcit != copied_list.rend(); ++rrit, ++rcit) {
        EXPECT_EQ(*rrit, *rcit + 1) << "Mismatch in backward traversal";
        (*rrit)++;
        EXPECT_NE(*rrit, *rcit) << "Nodes in two lists should be independent after modification";
    }
}


TEST(ListTest, MoveAssignment) {
    // 
    const std::vector<std::string> expected = {"1", "2", "3", "4", "5"};
    mystl::list<std::string> ref_list(expected.begin(), expected.end());

    // 
    mystl::list<std::string> moved_list;
    moved_list = std::move(ref_list);  // Move assignment
    EXPECT_EQ(expected.size(), moved_list.size());

    // 
    int idx = 0;
    for (auto it = moved_list.begin(); it != moved_list.end(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in forward traversal at index: " << idx;
    }

    // 
    idx = expected.size() - 1;
    for (auto it = moved_list.rbegin(); it != moved_list.rend(); ++it, --idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in backward traversal at index: " << idx;
    }
}


TEST(ListTest, InitializerListAssignment) {
    // 
    mystl::list<int> myList;
    myList = {10, 20, 30, 40, 50};  // Initializer list assignment
    EXPECT_EQ(myList.size(), 5);

    // forward traversal
    std::vector<int> expected = {10, 20, 30, 40, 50};
    int idx = 0;
    for (auto it = myList.begin(); it != myList.end(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in forward traversal at index: " << idx;
    }

    // backward traversal
    idx = expected.size() - 1;
    for (auto it = myList.rbegin(); it != myList.rend(); ++it, --idx) {
        EXPECT_EQ(*it, expected[idx]) << "Mismatch in backward traversal at index: " << idx;
    }
}



/* Element access */
TEST(ListTest, Front) {
    // 
    mystl::list<int> list;

    // 
    EXPECT_THROW(list.front(), std::out_of_range);

    // 
    list.push_front(1);
    EXPECT_EQ(list.front(), 1);
}


TEST(ListTest, Back) {
    // 
    mystl::list<int> list;

    // 
    EXPECT_THROW(list.back(), std::out_of_range);

    // 
    list.push_back(1);
    EXPECT_EQ(list.back(), 1);
}


/* Capacity */
TEST(ListTest, Empty) {
    // 
    mystl::list<int> list;
    EXPECT_TRUE(list.empty());
}


/* Modifiers */
TEST(ListTest, Insert) {
    // 
    mystl::list<int> list;

    // Insert at the beginning when the list is empty
    auto it = list.insert(list.cbegin(), 10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 10);

    // Insert at the end
    it = list.insert(list.cend(), 20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 20);

    // Insert at the middle
    auto cit = list.cbegin();
    std::advance(cit, 1);
    it = list.insert(cit, 15);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*it, 15);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(*(++list.cbegin()), 15);
    EXPECT_EQ(list.back(), 20);

    // Verify entire list content
    std::vector<int> expected = {10, 15, 20};
    int index = 0;
    for (auto fit = list.begin(); fit != list.end(); ++fit, ++index) {
        EXPECT_EQ(*fit, expected[index]) << "Mismatch in forward traversal at index: " << index;
    }

    index = 2;
    for (auto rit = list.rbegin(); rit != list.rend(); ++rit, --index) {
        EXPECT_EQ(*rit, expected[index]) << "Mismatch in backward traversal at index: " << index;
    }
}


TEST(ListTest, InsertByMove) {
    // 
    mystl::list<std::string> list;

    // Insert at the beginning when the list is empty
    std::string str1 = "10";
    auto it = list.insert(list.cbegin(), std::move(str1));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(*it, "10");
    EXPECT_EQ(list.front(), "10");
    EXPECT_EQ(list.back(), "10");

    // Insert at the end
    std::string str2 = "20";
    it = list.insert(list.cend(), std::move(str2));
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, "20");
    EXPECT_EQ(list.front(), "10");
    EXPECT_EQ(list.back(), "20");

    // Insert at the middle
    std::string str3 = "15";
    auto cit = list.cbegin();
    std::advance(cit, 1);
    it = list.insert(cit, std::move(str3));
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*it, "15");
    EXPECT_EQ(list.front(), "10");
    EXPECT_EQ(*(++list.cbegin()), "15");
    EXPECT_EQ(list.back(), "20");

    // Verify entire list content
    std::vector<std::string> expected = {"10", "15", "20"};
    int index = 0;
    for (auto fit = list.begin(); fit != list.end(); ++fit, ++index) {
        EXPECT_EQ(*fit, expected[index]) << "Mismatch in forward traversal at index: " << index;
    }

    index = 2;
    for (auto rit = list.rbegin(); rit != list.rend(); ++rit, --index) {
        EXPECT_EQ(*rit, expected[index]) << "Mismatch in backward traversal at index: " << index;
    }
}


TEST(ListTest, InsertByRange) {
    // 
    mystl::list<int> list;
    list.insert(list.cend(), {30, 40, 50});
    std::vector<int> toInsert = {10, 20};

    // Insert at the beginning
    auto it = list.insert(list.cbegin(), toInsert.begin(), toInsert.end());
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(*it, 10);
    std::vector<int> expected = {10, 20, 30, 40, 50};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);

    // Insert at the end
    it = list.insert(list.cend(), toInsert.begin(), toInsert.end());
    EXPECT_EQ(list.size(), 7);
    EXPECT_EQ(*it, 10);
    expected = {10, 20, 30, 40, 50, 10, 20};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);

    // Insert in the middle
    auto cit = list.cbegin();
    std::advance(cit, 3);
    it = list.insert(cit, toInsert.begin(), toInsert.end());
    EXPECT_EQ(list.size(), 9);
    EXPECT_EQ(*it, 10);
    expected = {10, 20, 30, 10, 20, 40, 50, 10, 20};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);
}


TEST(ListTest, InsertByInitializerList) {
    // 
    mystl::list<int> list;
    list.insert(list.cend(), {30, 40, 50});

    // beginning
    auto it = list.insert(list.cbegin(), {10, 20});
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(*it, 10);
    std::vector<int> expected = {10, 20, 30, 40, 50};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);

    // end
    it = list.insert(list.cend(), {60, 70});
    EXPECT_EQ(list.size(), 7);
    EXPECT_EQ(*it, 60);
    expected = {10, 20, 30, 40, 50, 60, 70};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);

    // middle
    auto cit = list.cbegin();
    std::advance(cit, 3);
    it = list.insert(cit, {100, 200});
    EXPECT_EQ(list.size(), 9);
    EXPECT_EQ(*it, 100);
    expected = {10, 20, 30, 100, 200, 40, 50, 60, 70};
    EXPECT_EQ(std::vector<int>(list.begin(), list.end()), expected);
}


TEST(ListTest, Emplace) {
    // 
    mystl::list<std::pair<int, int>> list;

    // beginning
    auto it = list.emplace(list.cbegin(), 10, 100);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(*it, std::make_pair(10, 100));
    EXPECT_EQ(list.front(), std::make_pair(10, 100));
    EXPECT_EQ(list.back(), std::make_pair(10, 100));

    // end
    it = list.emplace(list.cend(), 30, 300);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, std::make_pair(30, 300));
    EXPECT_EQ(list.front(), std::make_pair(10, 100));
    EXPECT_EQ(list.back(), std::make_pair(30, 300));

    // middle
    auto middle = list.cbegin();
    std::advance(middle, 1);
    it = list.emplace(middle, 20, 200);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*it, std::make_pair(20, 200));
    EXPECT_EQ(*list.begin(), std::make_pair(10, 100));
    EXPECT_EQ(*(++list.begin()), std::make_pair(20, 200));
    EXPECT_EQ(list.back(), std::make_pair(30, 300));

    // 
    std::vector<std::pair<int, int>> expected = {
        {10, 100}, {20, 200}, {30, 300}
    };

    // forward travese
    int index = 0;
    for (auto iter = list.begin(); iter != list.end(); ++iter, ++index) {
        EXPECT_EQ(*iter, expected[index]) << "Mismatch at index " << index;
    }

    // backward traverse
    index = 2;
    for (auto riter = list.rbegin(); riter != list.rend(); ++riter, --index) {
        EXPECT_EQ(*riter, expected[index]) << "Mismatch at index " << index;
    }
}


TEST(ListTest, Erase) {
    // 
    std::vector<int> expected = {10, 20, 30, 40, 50};
    mystl::list<int> list(expected.cbegin(), expected.cend());

    // beginning
    auto it_b = list.erase(list.cbegin());
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.front(), 20);
    EXPECT_EQ(*it_b, 20);

    // middle
    auto cit = list.cbegin();
    std::advance(cit, 1);
    auto it_m = list.erase(cit);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 20);
    EXPECT_EQ(*it_m, 40);

    // end
    EXPECT_THROW(list.erase(list.cend()), std::out_of_range);
}


TEST(ListTest, EraseByRange) {
    // 
    std::vector<int> expected = {10, 20, 30, 40, 50};
    mystl::list<int> list(expected.cbegin(), expected.cend());

    // 
    auto first = list.cbegin();
    auto last = list.cbegin();
    std::advance(first, 1);
    std::advance(last, 3);
    auto it = list.erase(first, last);

    // 
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*it, 40);
}


TEST(ListTest, PushBack) {
    // 
    mystl::list<int> list;

    // Test adding elements to the end of the list
    list.push_back(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 1);

    list.push_back(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 2);

    list.push_back(3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);

    // test forward traversal
    const std::vector<int> expected = {1, 2, 3};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 2;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, PushBackByMove) {
    mystl::list<std::string> list;

    std::string t1 = "1";
    list.push_back(std::move(t1));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), "1");
    EXPECT_EQ(list.back(), "1");

    std::string t2 = "2";
    list.push_back(std::move(t2));
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), "1");
    EXPECT_EQ(list.back(), "2");

    // test forward traversal
    const std::vector<std::string> expected = {"1", "2"};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 1;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, EmplaceBack) {
    mystl::list<std::pair<int, int>> list;

    list.emplace_back(1, 2);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), std::make_pair(1, 2));
    EXPECT_EQ(list.back(), std::make_pair(1, 2));

    list.emplace_back(2, 3);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), std::make_pair(1, 2));
    EXPECT_EQ(list.back(), std::make_pair(2, 3));

    // test forward traversal
    const std::vector<std::pair<int, int>> expected = {{1, 2}, {2, 3}};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 1;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, PopBack) {
    mystl::list<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    EXPECT_EQ(list.size(), 3);

    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 1);

    list.pop_back();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.pop_back(), std::out_of_range);
}


TEST(ListTest, PushFront) {
    // 
    mystl::list<int> list;

    // 
    list.push_front(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 1);

    // 
    list.push_front(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 1);

    // 
    list.push_front(3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 1);

    // test forward traversal
    const std::vector<int> expected = {3, 2, 1};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 2;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, PushFrontByMove) {
    // 
    mystl::list<std::string> list;

    // 
    std::string t1 = "1";
    list.push_front(std::move(t1));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), "1");
    EXPECT_EQ(list.back(), "1");

    // 
    std::string t2 = "2";
    list.push_front(std::move(t2));
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), "2");
    EXPECT_EQ(list.back(), "1");

    // test forward traversal
    const std::vector<std::string> expected = {"2", "1"};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 1;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, EmplaceFront) {
    //
    mystl::list<std::pair<int, int>> list;

    // 
    list.emplace_front(1, 2);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), std::make_pair(1, 2));
    EXPECT_EQ(list.back(), std::make_pair(1, 2));

    // 
    list.emplace_front(2, 3);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), std::make_pair(2, 3));
    EXPECT_EQ(list.back(), std::make_pair(1, 2));

    // test forward traversal
    const std::vector<std::pair<int, int>> expected = {{2, 3}, {1, 2}};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it, ++i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in forward traversal at index: " << i;
    }

    // test backward traversal
    i = 1;
    for (auto it = list.rbegin(); it != list.rend(); ++it, --i) {
        EXPECT_EQ(*it, expected[i]) << "Mismatch in backward traversal at index: " << i;
    }
}


TEST(ListTest, PopFront) {
    //
    mystl::list<int> list;

    // 
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    EXPECT_EQ(list.size(), 3);

    // 
    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.pop_front();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);

    list.pop_front();
    EXPECT_EQ(list.size(), 0);

    EXPECT_THROW(list.pop_front(), std::out_of_range);
}


TEST(ListTest, Resize) {
    // 
    mystl::list<int> list;

    // 
    list.resize(5, 10);
    EXPECT_EQ(list.size(), 5);
    for (const auto& item : list) {
        EXPECT_EQ(item, 10);
    }

    // 
    list.resize(3);
    EXPECT_EQ(list.size(), 3);
    int count = 0;
    for (const auto& item : list) {
        EXPECT_EQ(item, 10);
        ++count;
    }
    EXPECT_EQ(count, 3);

    // 
    list.resize(0);
    EXPECT_TRUE(list.empty());

    // 
    list.resize(2, 20);
    EXPECT_EQ(list.size(), 2);
    for (const auto& item : list) {
        EXPECT_EQ(item, 20);
    }
}


TEST(ListTest, Swap) {
    // Create two forward lists
    mystl::list<int> list1 = {1, 2, 3};
    mystl::list<int> list2 = {4, 8, 7, 6, 3};

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
TEST(ListTest, Merge) {
    // 
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    mystl::list<int> list1 = {1, 3, 5};
    mystl::list<int> list2 = {2, 4, 6};

    list1.merge(list2);

    // 
    EXPECT_EQ(list1.size(), expected.size());

    // forward
    int idx = 0;
    for (auto it = list1.cbegin(); it != list1.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }

    // backword
    idx = expected.size() - 1;
    for (auto rit = list1.crbegin(); rit != list1.crend() && idx >= 0; ++rit) {
        EXPECT_EQ(*rit, expected[idx--]);
    }

    // list2 should be empty
    EXPECT_EQ(list2.size(), 0);
    EXPECT_EQ(list2.empty(), true);
    EXPECT_TRUE(list2.begin() == list2.end());
}


TEST(ListTest, Splice) {
    // Setup two lists
    mystl::list<int> list1 = {1, 2, 3};
    mystl::list<int> list2 = {4, 5, 6};

    // Iterator to the position after which elements will be inserted
    auto pos = list1.cbegin();
    std::advance(pos, 1);  // Move iterator to '2'

    // Perform splice
    list1.splice(pos, list2);

    // Expected result in list1
    std::vector<int> expected = {1, 2, 4, 5, 6, 3};
    std::vector<int> result(list1.begin(), list1.end());
    EXPECT_EQ(result, expected);

    // List2 should be empty
    EXPECT_TRUE(list2.empty());
    EXPECT_EQ(list2.size(), 0);
}


TEST(ListTest, Remove) {
    // 
    mystl::list<int> list = {1, 2, 3, 2, 4, 2, 5};

    // 
    list.remove(2);

    // 
    std::vector<int> expected = {1, 3, 4, 5};
    std::vector<int> actual(list.begin(), list.end());
    EXPECT_EQ(actual, expected);

    // 
    EXPECT_EQ(list.size(), 4);

    // no effect
    list.remove(6);
    EXPECT_EQ(list.size(), 4);
}


TEST(ListTest, Reverse) {
    // 
    std::vector<int> expected = {5, 4, 3, 2, 1};
    mystl::list<int> list = {1, 2, 3, 4, 5};

    // 
    list.reverse();

    // 
    int idx = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }

    // backword
    idx = expected.size() - 1;
    for (auto rit = list.crbegin(); rit != list.crend() && idx >= 0; ++rit) {
        EXPECT_EQ(*rit, expected[idx--]);
    }

    // reverse again
    list.reverse();

    // forward
    idx = expected.size() - 1;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx--]);
    }

    // backword
    idx = 0;
    for (auto rit = list.crbegin(); rit != list.crend(); ++rit) {
        EXPECT_EQ(*rit, expected[idx++]);
    }
}


TEST(ListTest, Unique) {
    // 
    mystl::list<int> list = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5};
    std::vector<int> expected = {1, 2, 3, 4, 5};

    // 
    list.unique();
    EXPECT_EQ(list.size(), 5);

    // 
    int idx = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }

    // backword
    idx = expected.size() - 1;
    for (auto rit = list.crbegin(); rit != list.crend() && idx >= 0; ++rit) {
        EXPECT_EQ(*rit, expected[idx--]);
    }

    // the list is already unique
    list.unique();
    EXPECT_EQ(list.size(), 5);

    // 
    idx = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, expected[idx++]);
    }

    // backword
    idx = expected.size() - 1;
    for (auto rit = list.crbegin(); rit != list.crend() && idx >= 0; ++rit) {
        EXPECT_EQ(*rit, expected[idx--]);
    }
}


TEST(ListTest, Sort) {
    // 
    std::vector<int> unsorted = {7, 3, 5, 1, 2, 6, 4, 1, 8};
    std::vector<int> sorted   = {1, 1, 2, 3, 4, 5, 6, 7, 8};
    mystl::list<int> list(unsorted.begin(), unsorted.end());

    // 
    list.sort();

    // 
    EXPECT_EQ(list.size(), sorted.size());

    // forward
    int idx = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it)
        EXPECT_EQ(*it, sorted[idx++]);
    
    // backward
    idx = sorted.size() - 1;
    for (auto rit = list.crbegin(); rit != list.crend(); ++rit)
        EXPECT_EQ(*rit, sorted[idx--]);

    // Sort a sorted list again
    list.sort();

    // 
    EXPECT_EQ(list.size(), sorted.size());

    // forward
    idx = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it)
        EXPECT_EQ(*it, sorted[idx++]);
    
    // backward
    idx = sorted.size() - 1;
    for (auto rit = list.crbegin(); rit != list.crend(); ++rit)
        EXPECT_EQ(*rit, sorted[idx--]);
}


/**/
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
