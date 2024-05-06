#include "gtest/gtest.h"
#define private public
#include "swiss_table.h"
using namespace SwissTables;


TEST(FlatHashMap, Rehash) {
  FlatHashMap<int, int> map(4);
  map.rehash();
  EXPECT_EQ(map.capacity_, 8);
}

TEST(FlatHashMap, insert_and_find) {
  FlatHashMap<int, int> map;
  map.insert(1, 2);
  map.insert(2, 3);
  map.insert(3, 4);
  map.insert(4, 5);

  EXPECT_EQ(*map.find(1), 2);
  EXPECT_EQ(*map.find(2), 3);
  EXPECT_EQ(*map.find(3), 4);
  EXPECT_EQ(*map.find(4), 5);
}

TEST(FlatHashMap, insert_and_find_with_rehash) {
  FlatHashMap<int, int> map(4);
  map.insert(1, 2);
  map.insert(2, 3);
  map.insert(3, 4);
  map.insert(4, 5);

  EXPECT_EQ(*map.find(1), 2);
  EXPECT_EQ(*map.find(2), 3);
  EXPECT_EQ(*map.find(3), 4);
  EXPECT_EQ(*map.find(4), 5);
}

TEST(FlatHashMap, insert_and_multiple_rehash) {
  FlatHashMap<int, int> map;
  for (int i = 0; i < 1000; i++) {
    map.insert(i, i);
  }
  for (int i = 0; i < 1000; i++) {
    EXPECT_EQ(*map.find(i), i);
  }
}