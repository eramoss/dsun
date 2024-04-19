#include "gtest/gtest.h"
#include "../src/vec_deque.h"

TEST(VecDequeTest, PushBackAndPopBack) {
  VecDeque<int> deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  EXPECT_EQ(deque.pop_back(), 3);
  EXPECT_EQ(deque.pop_back(), 2);
  EXPECT_EQ(deque.pop_back(), 1);
}

TEST(VecDequeTest, PushBackAndPopFront) {
  VecDeque<int> deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  EXPECT_EQ(deque.pop_front(), 1);
  EXPECT_EQ(deque.pop_front(), 2);
  EXPECT_EQ(deque.pop_front(), 3);
}

TEST(VecDequeTest, OperatorSquareBrackets) {
  VecDeque<int> deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(deque[2], 3);
}

TEST(VecDequeTest, PopBackOnEmptyDeque) {
  VecDeque<int> deque;
  ASSERT_FALSE(deque.pop_back().has_value());
}

TEST(VecDequeTest, PopFrontOnEmptyDeque) {
  VecDeque<int> deque;
  ASSERT_FALSE(deque.pop_front().has_value());
}

TEST(VecDequeTest, OperatorSquareBracketsOutOfRange) {
  VecDeque<int> deque;
  deque.push_back(1);
  EXPECT_THROW(deque[1], std::out_of_range);
}