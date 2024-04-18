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

TEST_F(HashmapTest, GetInEmpty) {
  EXPECT_FALSE(hash_map.get(1).has_value());
}

TEST_F(HashmapTest, GetMutTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  auto one = hash_map.get_mut(1).value();
  *one = std::string("OneOne");
  EXPECT_EQ(hash_map.get(1).value(), "OneOne");
}

TEST_F(HashmapTest, GetMutInEmpty) {
  EXPECT_FALSE(hash_map.get_mut(1).has_value());
}

TEST_F(HashmapTest, ContainsKeyTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  EXPECT_TRUE(hash_map.contains_key(1));
  EXPECT_TRUE(hash_map.contains_key(2));
  EXPECT_TRUE(hash_map.contains_key(3));
  EXPECT_FALSE(hash_map.contains_key(4));
}

TEST_F(HashmapTest, ContainsKeyInEmpty) {
  EXPECT_FALSE(hash_map.contains_key(1));
}

TEST_F(HashmapTest, RemoveTest) {
  hash_map.insert(1, "One");
  hash_map.insert(2, "Two");
  hash_map.insert(3, "Three");

  hash_map.remove(1);
  EXPECT_FALSE(hash_map.contains_key(1));
  EXPECT_TRUE(hash_map.contains_key(2));
  EXPECT_TRUE(hash_map.contains_key(3));

  hash_map.remove(2);
  EXPECT_FALSE(hash_map.contains_key(2));
  EXPECT_TRUE(hash_map.contains_key(3));

  hash_map.remove(3);
  EXPECT_FALSE(hash_map.contains_key(3));
}

TEST_F(HashmapTest, RemoveInEmpty) {
  auto removed = hash_map.remove(1);
  EXPECT_FALSE(removed.has_value());
  EXPECT_FALSE(hash_map.contains_key(1));
}

TEST_F(HashmapTest, EntryOrInsertTest) {
  hash_map.entry(1).or_insert("One");
  EXPECT_EQ(hash_map.get(1).value(), "One");

  *hash_map.entry(1).or_insert("two") = "one one";
  EXPECT_EQ(hash_map.get(1).value(), "one one");
}

TEST_F(HashmapTest, EntryAndModifyTest) {
  hash_map.insert(1, "One");
  auto entry = hash_map.entry(1).and_modify([](std::string& value) {
    value = "OneOne";
    }).or_insert("two");
    auto entry2 = hash_map.entry(2).and_modify([](std::string& value) {
      value = "OneOne";
      }).or_insert("two");
      EXPECT_EQ(entry2, "two");
      EXPECT_EQ(entry, "OneOne");
}