#include "gtest/gtest.h"
#include <optional>
#include <string>

#define private public
#include "../src/avltree.h"


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


TEST(AVLTest, test_insert_duplicates) {
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(5);
  EXPECT_TRUE(avl.search(5) == 5);
}

TEST(AVLTest, test_search_empty) {
  AVLTree<int> avl;
  EXPECT_TRUE(avl.search(5) == std::nullopt);
}

TEST(AVLTest, test_remove){
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(3);
  avl.insert(7);
  avl.insert(1);
  avl.insert(4);
  avl.insert(6);
  avl.insert(8);
  avl.remove(5);
  EXPECT_TRUE(avl.search(5) == std::nullopt);
  EXPECT_TRUE(avl.search(3) == 3);
  EXPECT_TRUE(avl.search(7) == 7);
  EXPECT_TRUE(avl.search(1) == 1);
  EXPECT_TRUE(avl.search(4) == 4);
  EXPECT_TRUE(avl.search(6) == 6);
  EXPECT_TRUE(avl.search(8) == 8);
}


TEST(AVLTest, test_remove_root){
  AVLTree<int> avl;
  avl.insert(5);
  avl.remove(5);
  EXPECT_TRUE(avl.search(5) == std::nullopt);
}


TEST(AVLTest, test_remove_not_found){
  AVLTree<int> avl;
  avl.insert(5);
  avl.remove(6);
  EXPECT_TRUE(avl.search(5) == 5);
  EXPECT_TRUE(avl.len() == 1);
}

TEST(AVLTest, test_contains){
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(3);
  avl.insert(7);
  avl.insert(1);
  avl.insert(4);
  avl.insert(6);
  avl.insert(8);
  EXPECT_TRUE(avl.contains(5));
  EXPECT_TRUE(avl.contains(3));
  EXPECT_TRUE(avl.contains(7));
  EXPECT_TRUE(avl.contains(1));
  EXPECT_TRUE(avl.contains(4));
  EXPECT_TRUE(avl.contains(6));
  EXPECT_TRUE(avl.contains(8));
  avl.remove(5);
  EXPECT_FALSE(avl.contains(5));
  EXPECT_TRUE(avl.contains(3));
  EXPECT_FALSE(avl.contains(2));
  EXPECT_FALSE(avl.contains(9));
}


TEST(AVLTest, test_len){
  AVLTree<int> avl;
  avl.insert(5);
  avl.insert(3);
  avl.insert(7);
  avl.insert(1);
  avl.insert(4);
  avl.insert(6);
  avl.insert(8);
  EXPECT_TRUE(avl.len() == 7);
  avl.remove(5);
  EXPECT_TRUE(avl.len() == 6);
  avl.remove(3);
  EXPECT_TRUE(avl.len() == 5);
  avl.remove(7);
  EXPECT_TRUE(avl.len() == 4);
  avl.remove(1);
  EXPECT_TRUE(avl.len() == 3);
  avl.remove(4);
  EXPECT_TRUE(avl.len() == 2);
  avl.remove(6);
  EXPECT_TRUE(avl.len() == 1);
  avl.remove(8);
  EXPECT_TRUE(avl.len() == 0);
}





void print(AVLNode<int>* p, int indent){
        if(p != NULL) {
        if(p->right) {
            print(p->right, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->value << "\n ";
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            print(p->left, indent+4);
        }
    }
}

const std::string expected_out =  "            10\n          /\n        9\n      /\n    8\n      \\\n            7\n          /\n        6\n          \\\n            5\n  /\n 4\n   \\\n        3\n      /\n    2\n      \\\n        1\n ";
//             10
//           /
//         9
//       /
//     8
//       \
//             7
//           /
//         6
//           \
//             5
//   /
//  4
//    \
//         3
//       /
//     2
//       \
//         1
TEST(AVLTest, print){
  AVLTree<int> avl;
  avl.insert(1);
  avl.insert(2);
  avl.insert(3);
  avl.insert(4);
  avl.insert(5);
  avl.insert(6);
  avl.insert(7);
  avl.insert(8);
  avl.insert(9);
  avl.insert(10);
  testing::internal::CaptureStdout();
  print(avl.root,0);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, expected_out);
}