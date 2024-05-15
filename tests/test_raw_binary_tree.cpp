#include <gtest/gtest.h>
#include "../src/raw/binary_tree.h"

using namespace dsun::raw;



TEST(BinaryTreeTest, MergeTest) {
  std::unique_ptr<node_t<int>> left = std::make_unique<node_t<int>>(5);
  std::unique_ptr<node_t<int>> right = std::make_unique<node_t<int>>(10);
  std::unique_ptr<node_t<int>> root = std::make_unique<node_t<int>>(7);

  std::unique_ptr<node_t<int>> mergedTree = BinaryTree<int>::merge(std::move(left), std::move(right), std::move(root));

  EXPECT_EQ(mergedTree->value, 7);
  EXPECT_EQ(mergedTree->left->value, 5);
  EXPECT_EQ(mergedTree->right->value, 10);
}

TEST(BinaryTreeTest, LinkNodeLeftTest) {
  std::unique_ptr<node_t<int>> root = std::make_unique<node_t<int>>(7);
  std::unique_ptr<node_t<int>> left = std::make_unique<node_t<int>>(5);

  std::unique_ptr<node_t<int>> linkedTree = BinaryTree<int>::link_node_left(std::move(root), std::move(left));

  EXPECT_EQ(linkedTree->value, 7);
  EXPECT_EQ(linkedTree->left->value, 5);
}

TEST(BinaryTreeTest, LinkNodeRightTest) {
  std::unique_ptr<node_t<int>> root = std::make_unique<node_t<int>>(7);
  std::unique_ptr<node_t<int>> right = std::make_unique<node_t<int>>(10);

  std::unique_ptr<node_t<int>> linkedTree = BinaryTree<int>::link_node_right(std::move(root), std::move(right));

  EXPECT_EQ(linkedTree->value, 7);
  EXPECT_EQ(linkedTree->right->value, 10);
}

TEST(BinaryTreeTest, UnlinkNodeLeftTest) {
  std::unique_ptr<node_t<int>> root = std::make_unique<node_t<int>>(7);
  std::unique_ptr<node_t<int>> left = std::make_unique<node_t<int>>(5);
  root->left = std::move(left);

  std::unique_ptr<node_t<int>> unlinkedTree = BinaryTree<int>::unlink_node_left(std::move(root));

  EXPECT_EQ(unlinkedTree->value, 5);
  EXPECT_EQ(unlinkedTree->left, nullptr);
}

TEST(BinaryTreeTest, UnlinkNodeRightTest) {
  std::unique_ptr<node_t<int>> root = std::make_unique<node_t<int>>(7);
  std::unique_ptr<node_t<int>> right = std::make_unique<node_t<int>>(10);
  root->right = std::move(right);

  std::unique_ptr<node_t<int>> unlinkedTree = BinaryTree<int>::unlink_node_right(std::move(root));

  EXPECT_EQ(unlinkedTree->value, 10);
  EXPECT_EQ(unlinkedTree->right, nullptr);
}
