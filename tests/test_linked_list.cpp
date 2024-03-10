#include <gtest/gtest.h>
#include "../src/linked_list.h" // Assuming your LinkedList class is defined in LinkedList.h

using namespace dsun;

TEST(LinkedListInitTest, FromArrTest) {
  int arr[] = { 1, 2, 3, 4, 5 };
  LinkedList<int> list_from_arr = LinkedList<int>::from_arr(arr, 5);

  ASSERT_EQ(list_from_arr.len(), 5);

  for (int i = 0; i < 5; ++i) {
    auto node = list_from_arr.pop_front();
    ASSERT_TRUE(node.has_value());
    ASSERT_EQ(node.value(), arr[i]);
  }
}

TEST(LinkedListInitTest, FromListTest) {
  LinkedList<int> list_from_list = LinkedList<int>::from_list({ 1, 2, 3, 4, 5 });
  ASSERT_EQ(list_from_list.len(), 5);


  for (int i = 1; i <= 5; ++i) {
    auto node = list_from_list.pop_front();
    ASSERT_TRUE(node.has_value());
    ASSERT_EQ(node.value(), i);
  }
}

TEST(LinkedListInitTest, FromSliceTest) {
  LinkedList<int> list_from_list = LinkedList<int>::from_slice({ 1, 2, 3, 4, 5 });
  ASSERT_EQ(list_from_list.len(), 5);


  for (int i = 1; i <= 5; ++i) {
    auto node = list_from_list.pop_front();
    ASSERT_TRUE(node.has_value());
    ASSERT_EQ(node.value(), i);
  }
}

TEST(LinkedListInitTest, FromPartsTest) {
  LinkedList<int> list1 = LinkedList<int>::from_list({ 1, 2, 3 });
  LinkedList<int> list2 = LinkedList<int>::from_list({ 4, 5, 6 });

  LinkedList<int> list_from_parts = LinkedList<int>::from_parts(list1, list2);
  ASSERT_EQ(list_from_parts.len(), 6);

  for (int i = 1; i <= 6; ++i) {
    auto node = list_from_parts.pop_front();
    ASSERT_TRUE(node.has_value());
    ASSERT_EQ(node.value(), i);
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

TEST_F(LinkedListTest, Get) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_EQ(list.get(0).value(), 10);
  EXPECT_EQ(list.get(1).value(), 20);
  EXPECT_EQ(list.get(2).value(), 30);
  EXPECT_FALSE(list.get(3).has_value());
}

TEST_F(LinkedListTest, OperatorIndex) {
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_EQ(list[0].value(), 10);
  EXPECT_EQ(list[1].value(), 20);
  EXPECT_EQ(list[2].value(), 30);
  EXPECT_FALSE(list[3].has_value());
}

TEST_F(LinkedListTest, InsertSorted) {
  list.insert_sorted(10);
  EXPECT_EQ(list.len(), 1);
  EXPECT_EQ(list.front().value(), 10);
  EXPECT_EQ(list.back().value(), 10);

  list.insert_sorted(5);
  EXPECT_EQ(list.len(), 2);
  EXPECT_EQ(list.front().value(), 5);
  EXPECT_EQ(list.back().value(), 10);

  list.insert_sorted(15);
  EXPECT_EQ(list.len(), 3);
  EXPECT_EQ(list.front().value(), 5);
  EXPECT_EQ(list.back().value(), 15);

  list.insert_sorted(8);
  EXPECT_EQ(list.len(), 4);
  EXPECT_EQ(list.front().value(), 5);
  EXPECT_EQ(list.back().value(), 15);

  list.insert_sorted(20);
  EXPECT_EQ(list.len(), 5);
  EXPECT_EQ(list.front().value(), 5);
  EXPECT_EQ(list.back().value(), 20);

  list.insert_sorted(1);
  EXPECT_EQ(list.len(), 6);
  EXPECT_EQ(list.front().value(), 1);
  EXPECT_EQ(list.back().value(), 20);
}


TEST(LinkedListCustomTypeTest, InsertSorted) {
  struct CustomType {
    int value;
    bool operator<(const CustomType& other) const {
      return value < other.value;
    }
    bool operator>(const CustomType& other) const {
      return value > other.value;
    }
  };
  auto list = LinkedList<CustomType>();
  list.insert_sorted(CustomType{ 10 });
  EXPECT_EQ(list.len(), 1);
  EXPECT_EQ(list.front().value().value, 10);
  EXPECT_EQ(list.back().value().value, 10);

  list.insert_sorted(CustomType{ 5 });
  EXPECT_EQ(list.len(), 2);
  EXPECT_EQ(list.front().value().value, 5);
  EXPECT_EQ(list.back().value().value, 10);

  list.insert_sorted(CustomType{ 15 });
  EXPECT_EQ(list.len(), 3);
  EXPECT_EQ(list.front().value().value, 5);
  EXPECT_EQ(list.back().value().value, 15);
}