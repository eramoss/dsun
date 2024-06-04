#include "gtest/gtest.h"
#include "../src/stack.h"

TEST(Stack, Push) {
  dsun::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.push(5);
  EXPECT_EQ(stack.size(), 5);
}

TEST(Stack, Pop) {
  dsun::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.push(5);
  EXPECT_EQ(stack.pop(), 5);
  EXPECT_EQ(stack.pop(), 4);
  EXPECT_EQ(stack.pop(), 3);
  EXPECT_EQ(stack.pop(), 2);
  EXPECT_EQ(stack.pop(), 1);
  EXPECT_EQ(stack.size(), 0);
}

TEST(Stack, PopEmpty) {
  dsun::Stack<int> stack;
  EXPECT_EQ(stack.pop(), std::nullopt);
}

TEST(Stack, Peek) {
  dsun::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.push(5);
  EXPECT_EQ(stack.peek(), 5);
  stack.pop();
  EXPECT_EQ(stack.peek(), 4);
  stack.pop();
  EXPECT_EQ(stack.peek(), 3);
  stack.pop();
  EXPECT_EQ(stack.peek(), 2);
  stack.pop();
  EXPECT_EQ(stack.peek(), 1);
}

TEST(Stack, PeekEmpty) {
  dsun::Stack<int> stack;
  EXPECT_EQ(stack.peek(), std::nullopt);
}

TEST(Stack, IsEmpty) {
  dsun::Stack<int> stack;
  EXPECT_TRUE(stack.is_empty());
  stack.push(1);
  EXPECT_FALSE(stack.is_empty());
  stack.pop();
  EXPECT_TRUE(stack.is_empty());
}

TEST(Stack, Size) {
  dsun::Stack<int> stack;
  EXPECT_EQ(stack.size(), 0);
  stack.push(1);
  EXPECT_EQ(stack.size(), 1);
  stack.push(2);
  EXPECT_EQ(stack.size(), 2);
  stack.push(3);
  EXPECT_EQ(stack.size(), 3);
  stack.push(4);
  EXPECT_EQ(stack.size(), 4);
  stack.push(5);
  EXPECT_EQ(stack.size(), 5);
  stack.pop();
  EXPECT_EQ(stack.size(), 4);
  stack.pop();
  EXPECT_EQ(stack.size(), 3);
  stack.pop();
  EXPECT_EQ(stack.size(), 2);
  stack.pop();
  EXPECT_EQ(stack.size(), 1);
  stack.pop();
  EXPECT_EQ(stack.size(), 0);
}