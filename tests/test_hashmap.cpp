#include <gtest/gtest.h>
#include "dsun.h"
#include <string>


class HashTableTest : public ::testing::Test {
protected:
  dsun::HashMap<int, std::string> hash_table;
};

TEST(HashTest, equlityHash) {
  dsun::Hash<int> hash;
  EXPECT_EQ(hash(1), hash(1));
  EXPECT_EQ(hash(2), hash(2));
  EXPECT_EQ(hash(3), hash(3));
  EXPECT_FALSE(hash(1) == hash(2));
}

TEST_F(HashTableTest, InsertTest) {
  hash_table.insert(1, "One");
  hash_table.insert(2, "Two");
  hash_table.insert(3, "Three");

  EXPECT_EQ(hash_table.get(1).value(), "One");
  EXPECT_EQ(hash_table.get(2).value(), "Two");
  EXPECT_EQ(hash_table.get(3).value(), "Three");
}

TEST_F(HashTableTest, GetTest) {
  hash_table.insert(1, "One");
  hash_table.insert(2, "Two");
  hash_table.insert(3, "Three");

  EXPECT_EQ(hash_table.get(1).value(), "One");
  EXPECT_EQ(hash_table.get(2).value(), "Two");
  EXPECT_EQ(hash_table.get(3).value(), "Three");

  EXPECT_FALSE(hash_table.get(4).has_value());
}

TEST_F(HashTableTest, GetMutTest) {
  hash_table.insert(1, "One");
  hash_table.insert(2, "Two");
  hash_table.insert(3, "Three");

  auto one = hash_table.get_mut(1).value();
  *one = std::string("OneOne");
  EXPECT_EQ(hash_table.get(1).value(), "OneOne");

}