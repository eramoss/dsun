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


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}