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

TEST(VecDequeTest, MakeContiguous) {
  VecDeque<int> deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  deque.pop_front();
  deque.push_back(4);
  // At this point, the internal buffer might look like this: [_, 2, 3, 4]
  // After calling make_contiguous, it should look like this: [2, 3, 4, _]
  auto slice = deque.make_contiguous();
  EXPECT_EQ(slice[0], 2);
  EXPECT_EQ(slice[1], 3);
  EXPECT_EQ(slice[2], 4);
}

TEST(VecDequeTest, PushFrontInFullCap) {
  VecDeque<int> deque = VecDeque<int>::with_capacity(3);
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  deque.push_front(4);
  EXPECT_EQ(deque.pop_front(), 4);
}