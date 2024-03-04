#include "gtest/gtest.h"
#include "../src/vec.h"

class VecTest : public ::testing::Test {
protected:
  Vec<int> vec;
};

TEST_F(VecTest, ConstructorTest) {
  EXPECT_EQ(vec.len(), 0);
  EXPECT_EQ(vec.capacity(), 16); //  INITIAL_SIZE
}

TEST_F(VecTest, FromSliceTest) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  vec.from_slice(arr, 5);

  EXPECT_EQ(vec.len(), 5);
  EXPECT_EQ(vec.capacity(), 10);
}

TEST_F(VecTest, GetVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  vec.from_slice(arr, 5);

  EXPECT_EQ(vec.get(0).value(), 1);
}

TEST_F(VecTest, GetMutVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  vec.from_slice(arr, 5);

  int* ptr = vec.get_mut(0).value();
  *ptr = 10;
  EXPECT_EQ(vec.get(0).value(), 10);
}

TEST_F(VecTest, PushVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  vec.from_slice(arr, 5);
  vec.push(6);

  EXPECT_EQ(vec.get(5).value(), 6);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}