#include "../src/avltree.h"
#include "gtest/gtest.h"

using namespace dsun;

TEST(AVLTest, test_insert_and_search) {
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(3);
  avl.insert(7);
  avl.insert(1);
  avl.insert(4);
  avl.insert(6);
  avl.insert(8);
  EXPECT_TRUE(avl.search(5) == 5);
  EXPECT_TRUE(avl.search(3) == 3);
  EXPECT_TRUE(avl.search(7) == 7);
  EXPECT_TRUE(avl.search(1) == 1);
  EXPECT_TRUE(avl.search(4) == 4);
  EXPECT_TRUE(avl.search(6) == 6);
  EXPECT_TRUE(avl.search(8) == 8);
}

TEST(AVLTest, test_with_multiple_balances) {
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(3);
  avl.insert(7);
  avl.insert(1);
  avl.insert(4);
  avl.insert(6);
  avl.insert(8);
  avl.insert(2);
  avl.insert(9);
  avl.insert(10);
  EXPECT_TRUE(avl.search(5) == 5);
  EXPECT_TRUE(avl.search(3) == 3);
  EXPECT_TRUE(avl.search(7) == 7);
  EXPECT_TRUE(avl.search(1) == 1);
  EXPECT_TRUE(avl.search(4) == 4);
  EXPECT_TRUE(avl.search(6) == 6);
  EXPECT_TRUE(avl.search(8) == 8);
  EXPECT_TRUE(avl.search(2) == 2);
  EXPECT_TRUE(avl.search(9) == 9);
  EXPECT_TRUE(avl.search(10) == 10);
}