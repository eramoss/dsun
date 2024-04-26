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