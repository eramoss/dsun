#include "gtest/gtest.h"
#include "bst.h"
using namespace dsun;
TEST(BSTTest, InsertAndContains) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_TRUE(bst.contains(1));
  EXPECT_TRUE(bst.contains(4));
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(8));
  EXPECT_FALSE(bst.contains(2));
  EXPECT_FALSE(bst.contains(9));
}

TEST(BSTTest, InsertDuplicates) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(5);
  EXPECT_TRUE(bst.contains(5));
}

TEST(BSTTest, InsertAndContainsNegativeNumbers) {
  BST<int> bst;
  bst.insert(-5);
  bst.insert(-3);
  bst.insert(-7);
  EXPECT_TRUE(bst.contains(-5));
  EXPECT_TRUE(bst.contains(-3));
  EXPECT_TRUE(bst.contains(-7));
  EXPECT_FALSE(bst.contains(-4));
  EXPECT_FALSE(bst.contains(-6));
}

TEST(BSTTest, ContainsEmpty) {
  BST<int> bst;
  EXPECT_FALSE(bst.contains(5));
}

TEST(BSTTest, FindInTree) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  EXPECT_EQ(bst.find(5), std::optional<int>(5));
  EXPECT_EQ(bst.find(3), std::optional<int>(3));
  EXPECT_EQ(bst.find(7), std::optional<int>(7));
}

TEST(BSTTest, FindNotInTree) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  EXPECT_EQ(bst.find(4), std::nullopt);
  EXPECT_EQ(bst.find(6), std::nullopt);
  EXPECT_EQ(bst.find(8), std::nullopt);
}

TEST(BSTTest, FindInEmptyTree) {
  BST<int> bst;
  EXPECT_EQ(bst.find(5), std::nullopt);
}