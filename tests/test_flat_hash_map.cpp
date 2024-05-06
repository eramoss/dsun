#include "gtest/gtest.h"
#define private public
#include "swiss_table.h"
using namespace SwissTables;


TEST(FlatHashMap, Rehash) {
  FlatHashMap<int, int> map(4);
  map.rehash();
  EXPECT_EQ(map.capacity_, 8);
}

TEST(FlatHashMap, insert_and_get) {
  FlatHashMap<int, int> map;
  map.insert(1, 2);
  map.insert(2, 3);
  map.insert(3, 4);
  map.insert(4, 5);

  EXPECT_EQ(map.get(1).value(), 2);
  EXPECT_EQ(map.get(2).value(), 3);
  EXPECT_EQ(map.get(3).value(), 4);
  EXPECT_EQ(map.get(4).value(), 5);
}

TEST(FlatHashMap, insert_and_get_with_rehash) {
  FlatHashMap<int, int> map(2);
  map.insert(1, 2);
  map.insert(2, 3);
  map.insert(3, 4);
  map.insert(4, 5);

  EXPECT_EQ(map.get(1).value(), 2);
  EXPECT_EQ(map.get(2).value(), 3);
  EXPECT_EQ(map.get(3).value(), 4);
  EXPECT_EQ(map.get(4).value(), 5);
}

TEST(FlatHashMap, insert_and_multiple_rehash) {
  FlatHashMap<int, int> map;
  for (int i = 0; i < 1000; i++) {
    map.insert(i, i);
  }
  EXPECT_EQ(map.get(16).value(), 16);
}