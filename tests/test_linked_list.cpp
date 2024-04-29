#include <gtest/gtest.h>
#include "../src/linked_list.h" // Assuming your LinkedList class is defined in LinkedList.h

using namespace dsun;


TEST(LinkedListInitTest, FromListTest) {
  LinkedList<int> list_from_list = LinkedList<int>::from_list({ 1, 2, 3, 4, 5 });
  ASSERT_EQ(list_from_list.len(), 5);


  for (int i = 1; i <= 5; ++i) {
    auto node = list_from_list.pop_front();
    ASSERT_TRUE(node.has_value());
    ASSERT_EQ(node.value(), i);
  }
}


TEST(LinkedListInitTest, Copy) {
  LinkedList<int> list1 = LinkedList<int>::from_list({ 1, 2, 3 });
  LinkedList<int> list2 = LinkedList<int>::from_list({ 4, 5, 6 });

  list1 = list2;
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(list1[i], list2[i]);
  }
}

class LinkedListTest : public ::testing::Test {
protected:
  LinkedList<int> list;
};

TEST_F(LinkedListTest, PushBack) {
  list.push_back(10);
  EXPECT_EQ(list.len(), 1);
}

TEST_F(LinkedListTest, PushFront) {
  list.push_front(20);
  EXPECT_EQ(list.len(), 1);
}

TEST_F(LinkedListTest, PushMoreBack) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  EXPECT_EQ(list.back(), 30);
  EXPECT_EQ(list.front(), 10);
  EXPECT_EQ(list.len(), 3);
}

TEST_F(LinkedListTest, PushMoreFront) {
  list.push_front(20);
  list.push_front(30);
  list.push_front(40);
  EXPECT_EQ(list.back(), 20);
  EXPECT_EQ(list.front(), 40);
  EXPECT_EQ(list.len(), 3);
}

TEST_F(LinkedListTest, PopBack) {
  list.push_back(30);
  ASSERT_EQ(list.len(), 1);
  auto popped = list.pop_back();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 30);
  EXPECT_EQ(list.len(), 0);
}

TEST_F(LinkedListTest, PopFront) {
  list.push_back(40);
  ASSERT_EQ(list.len(), 1);
  auto popped = list.pop_front();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 40);
  EXPECT_EQ(list.len(), 0);
}

TEST_F(LinkedListTest, Len) {
  EXPECT_EQ(list.len(), 0);

  list.push_back(10);
  EXPECT_EQ(list.len(), 1);

  list.push_front(20);
  EXPECT_EQ(list.len(), 2);

  list.pop_back();
  EXPECT_EQ(list.len(), 1);

  list.pop_front();
  EXPECT_EQ(list.len(), 0);
}

TEST_F(LinkedListTest, Front) {
  EXPECT_FALSE(list.front().has_value());

  list.push_back(10);
  EXPECT_TRUE(list.front().has_value());
  EXPECT_EQ(list.front().value(), 10);

  list.push_front(20);
  EXPECT_EQ(list.front().value(), 20);
}

TEST_F(LinkedListTest, Back) {
  EXPECT_FALSE(list.back().has_value());

  list.push_back(10);
  EXPECT_TRUE(list.back().has_value());
  EXPECT_EQ(list.back().value(), 10);

  list.push_front(20);
  EXPECT_EQ(list.back().value(), 10);
}

TEST_F(LinkedListTest, At) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_EQ(list.at(0).value(), 10);
  EXPECT_EQ(list.at(1).value(), 20);
  EXPECT_EQ(list.at(2).value(), 30);
  EXPECT_FALSE(list.at(3).has_value());
}
TEST_F(LinkedListTest, AtMut) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  int* data = list.at_mut(0).value();
  *data = 15;
  EXPECT_EQ(list.at(0).value(), 15);
}

TEST_F(LinkedListTest, Get) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_EQ(list.get(10).value(), 10);
  EXPECT_EQ(list.get(20).value(), 20);
  EXPECT_EQ(list.get(30).value(), 30);
  EXPECT_FALSE(list.get(40).has_value());
}
TEST_F(LinkedListTest, GetMut) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  int* data = list.get_mut(10).value();
  *data = 15;
  EXPECT_EQ(list.get(15).value(), 15);
  EXPECT_FALSE(list.get(10).has_value());
}

TEST_F(LinkedListTest, OperatorIndex) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_EQ(list[0], 10);
  EXPECT_EQ(list[1], 20);
  EXPECT_EQ(list[2], 30);
  EXPECT_THROW(list[3], std::out_of_range);
}

TEST_F(LinkedListTest, Insert) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  list.insert(1, 15);
  EXPECT_EQ(list.len(), 4);

  EXPECT_EQ(list[0], 10);
  EXPECT_EQ(list[1], 15);
  EXPECT_EQ(list[2], 20);
  EXPECT_EQ(list[3], 30);
}

TEST_F(LinkedListTest, InsertEmpty) {
  list.insert(0, 10);
  EXPECT_EQ(list.len(), 1);
  EXPECT_EQ(list[0], 10);
}

TEST_F(LinkedListTest, InsertOutBounds) {
  list.insert(2, 10);
  EXPECT_EQ(list.len(), 0);
}

TEST_F(LinkedListTest, RemoveIndex) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  list.remove(2);
  EXPECT_EQ(list.len(), 4);
  EXPECT_EQ(list[2], 40);
}

TEST_F(LinkedListTest, RemoveEmpty) {
  LinkedList<int> list;
  list.remove(0);
  EXPECT_EQ(list.len(), 0);
}

TEST_F(LinkedListTest, RemoveOutOfBounds) {
  LinkedList<int> list;
  list.remove(2);
  EXPECT_EQ(list.len(), 0);
}


TEST(IteratorLinkedListTest, next) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  for (auto it = list.begin(); it.has_next(); ++it) {
    EXPECT_EQ(*it, it.index() * 10 + 10);
  }
}
TEST(IteratorLinkedListTest, getMut) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  for (auto it = list.begin(); it.has_next(); ++it) {
    it.get_mut() = 7;
  }
  for (auto it = list.begin(); it.has_next(); ++it) {
    EXPECT_EQ(*it, 7);
  }
}

TEST(HighOrderFunctions, map) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  auto new_list = list.map([](int x) { return x * 2; });
  for (auto it = new_list.begin(); it.has_next(); ++it) {
    EXPECT_EQ(*it, (it.index() * 10 + 10) * 2);
  }
}

TEST(HighOrderFunctions, map_mut) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  list.map_mut([](int& x) { x = x * 2; });
  for (auto it = list.begin(); it.has_next(); ++it) {
    EXPECT_EQ(*it, (it.index() * 10 + 10) * 2);
  }
}

TEST(HighOrderFunctions, filter) {
  auto list = LinkedList<int>::from_list({ 10, 20, 30, 40, 50 });
  auto new_list = list.filter([](int x) { return x % 20 == 0; });
  EXPECT_EQ(new_list.len(), 2);
  for (auto it = new_list.begin(); it.has_next(); ++it) {
    EXPECT_EQ(*it, (it.index() * 20) + 20);
  }
}
