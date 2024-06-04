#include "gtest/gtest.h"
#define private public
#include "../src/swiss_table.h"
using namespace SwissTables;


TEST(FlatHashMap, Iter) {
  auto map = FlatHashMap<int, int>(10);
  map.insert(1, 1);
  map.insert(2, 2);
  map.insert(3, 3);
  map.insert(4, 4);
  auto count = 1;
  for (auto& entry : map) {
    EXPECT_EQ(count, entry.value);
    count++;
  }
}
