#include <gtest/gtest.h>
#include "dsun.h"
#include <string>


class HashmapTest : public ::testing::Test {
protected:
  dsun::HashMap<int, std::string> hash_map;
};

TEST(HashTest, equlityHash) {
  dsun::Hash<int> hash;
  EXPECT_EQ(hash(1), hash(1));
  EXPECT_EQ(hash(2), hash(2));
  EXPECT_EQ(hash(3), hash(3));
  EXPECT_FALSE(hash(1) == hash(2));
}

TEST_F(HashmapTest, InsertTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  EXPECT_EQ(hash_map.get(1).value(), "One");
  EXPECT_EQ(hash_map.get(2).value(), "Two");
  EXPECT_EQ(hash_map.get(3).value(), "Three");
}

TEST_F(HashmapTest, GetTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  EXPECT_EQ(hash_map.get(1).value(), "One");
  EXPECT_EQ(hash_map.get(2).value(), "Two");
  EXPECT_EQ(hash_map.get(3).value(), "Three");

  EXPECT_FALSE(hash_map.get(4).has_value());
}

TEST_F(HashmapTest, GetMutTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  auto one = hash_map.get_mut(1).value();
  *one = std::string("OneOne");
  EXPECT_EQ(hash_map.get(1).value(), "OneOne");

}