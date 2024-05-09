#include "gtest/gtest.h"
#include "dsun.h"

using namespace dsun;
TEST(BSTreeTest, InsertAndContains) {
  BSTree<int> bst;
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

TEST(BSTreeTest, InsertDuplicates) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(5);
  EXPECT_TRUE(bst.contains(5));
}

TEST(BSTreeTest, InsertAndContainsNegativeNumbers) {
  BSTree<int> bst;
  bst.insert(-5);
  bst.insert(-3);
  bst.insert(-7);
  EXPECT_TRUE(bst.contains(-5));
  EXPECT_TRUE(bst.contains(-3));
  EXPECT_TRUE(bst.contains(-7));
  EXPECT_FALSE(bst.contains(-4));
  EXPECT_FALSE(bst.contains(-6));
}

TEST(BSTreeTest, ContainsEmpty) {
  BSTree<int> bst;
  EXPECT_FALSE(bst.contains(5));
}

TEST(BSTreeTest, FindInTree) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  EXPECT_EQ(bst.find(5), std::optional<int>(5));
  EXPECT_EQ(bst.find(3), std::optional<int>(3));
  EXPECT_EQ(bst.find(7), std::optional<int>(7));
}

TEST(BSTreeTest, FindNotInTree) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  EXPECT_EQ(bst.find(4), std::nullopt);
  EXPECT_EQ(bst.find(6), std::nullopt);
  EXPECT_EQ(bst.find(8), std::nullopt);
}

TEST(BSTreeTest, FindInEmptyTree) {
  BSTree<int> bst;
  EXPECT_EQ(bst.find(5), std::nullopt);
}

TEST(BSTreeTest, minAndExtract) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  EXPECT_EQ(bst.min(), std::optional<int>(1));
  EXPECT_EQ(bst.extract_min(), std::optional<int>(1));
  EXPECT_EQ(bst.min(), std::optional<int>(3));
}

TEST(BSTreeTest, minAndExtractEmpty) {
  BSTree<int> bst;
  EXPECT_EQ(bst.min(), std::nullopt);
  EXPECT_EQ(bst.extract_min(), std::nullopt);
}

TEST(BSTreeTest, minAndExtractSingle) {
  BSTree<int> bst;
  bst.insert(5);
  EXPECT_EQ(bst.min(), std::optional<int>(5));
  EXPECT_EQ(bst.extract_min(), std::optional<int>(5));
  EXPECT_EQ(bst.min(), std::nullopt);
}

TEST(BSTreeTest, maxAndExtract) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  EXPECT_EQ(bst.max(), std::optional<int>(7));
  EXPECT_EQ(bst.extract_max(), std::optional<int>(7));
  EXPECT_EQ(bst.max(), std::optional<int>(5));
}

TEST(BSTreeTest, maxAndExtractEmpty) {
  BSTree<int> bst;
  EXPECT_EQ(bst.max(), std::nullopt);
  EXPECT_EQ(bst.extract_max(), std::nullopt);
}

TEST(BSTreeTest, maxAndExtractSingle) {
  BSTree<int> bst;
  bst.insert(5);
  EXPECT_EQ(bst.max(), std::optional<int>(5));
  EXPECT_EQ(bst.extract_max(), std::optional<int>(5));
  EXPECT_EQ(bst.max(), std::nullopt);
}

TEST(BSTreeTest, preOrder) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  std::vector<int> expected = { 5, 3, 1, 4, 7, 6, 8 };
  std::vector<int> result;
  bst.pre_order([&](int value) {
    result.push_back(value);
    });
  EXPECT_EQ(result, expected);
}

TEST(BSTreeTest, inOrder) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  std::vector<int> expected = { 1, 3, 4, 5, 6, 7, 8 };
  std::vector<int> result;
  bst.in_order([&](int value) {
    result.push_back(value);
    });
  EXPECT_EQ(result, expected);
}

TEST(BSTreeTest, postOrder) {
  BSTree<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(1);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  std::vector<int> expected = { 1, 4, 3, 6, 8, 7, 5 };
  std::vector<int> result;
  bst.post_order([&](int value) {
    result.push_back(value);
    });
  EXPECT_EQ(result, expected);
}