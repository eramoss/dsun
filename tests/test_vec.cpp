#include "gtest/gtest.h"
#include "../src/vec.h"

class VecTest : public ::testing::Test {
protected:
  dsun::Vec<int> vec;
};

TEST_F(VecTest, ConstructorTest) {
  EXPECT_EQ(vec.len(), 0);
  EXPECT_EQ(vec.capacity(), 16); //  INITIAL_SIZE
}

TEST_F(VecTest, TestFromSlice) {
  int arr[] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  EXPECT_EQ(vec.len(), 5);

  for (size_t i = 0; i < vec.len(); ++i) {
    EXPECT_EQ(vec.get(i).value(), arr[i]);
  }
}

TEST_F(VecTest, TestFromList) {
  auto vec = dsun::Vec<int>::from_list({ 1, 2, 3, 4, 5 });
  EXPECT_EQ(vec.len(), 5);

  for (size_t i = 0; i < vec.len(); ++i) {
    EXPECT_EQ(vec.get(i).value(), i + 1);
  }
}

TEST_F(VecTest, TestWithCapacity) {
  auto vec = dsun::Vec<int>::with_capacity(10);

  EXPECT_EQ(vec.capacity(), 10);
}

TEST_F(VecTest, GetVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);

  EXPECT_EQ(vec.get(0).value(), 1);
}

TEST_F(VecTest, GetMutVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);

  int* ptr = vec.get_mut(0).value();
  *ptr = 10;
  EXPECT_EQ(vec.get(0).value(), 10);
}

TEST_F(VecTest, RemoveVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec.remove(2);

  EXPECT_EQ(vec.get(2).value(), 4);
  EXPECT_EQ(vec.len(), 4);
}

TEST_F(VecTest, PushVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec.push(6);

  EXPECT_EQ(vec.get(5).value(), 6);
}

TEST_F(VecTest, PopVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec.pop();

  EXPECT_EQ(vec.len(), 4);
  EXPECT_EQ(vec.get(3).value(), 4);
}

TEST_F(VecTest, LastVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  auto last = vec.last();

  EXPECT_EQ(last.value(), 5);
}

TEST_F(VecTest, FirstVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  auto first = vec.first();

  EXPECT_EQ(first.value(), 1);
}

TEST_F(VecTest, IndexVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);

  EXPECT_EQ(vec[0].value(), 1);
  EXPECT_EQ(vec[1].value(), 2);
  EXPECT_EQ(vec[2].value(), 3);
  EXPECT_EQ(vec[3].value(), 4);
  EXPECT_EQ(vec[4].value(), 5);
}


TEST_F(VecTest, MapVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  auto new_vec = vec.map([](int x) { return x + 1; });

  EXPECT_EQ(new_vec.get(4).value(), 6);
  EXPECT_EQ(new_vec.get(3).value(), 5);
  EXPECT_EQ(new_vec.get(2).value(), 4);
  EXPECT_EQ(new_vec.get(1).value(), 3);
  EXPECT_EQ(new_vec.get(0).value(), 2);
}


TEST_F(VecTest, MapMutVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec.map_mut([](int* x) {  *x = *x + 1; });

  EXPECT_EQ(vec.get(4).value(), 6);
  EXPECT_EQ(vec.get(3).value(), 5);
  EXPECT_EQ(vec.get(2).value(), 4);
  EXPECT_EQ(vec.get(1).value(), 3);
  EXPECT_EQ(vec.get(0).value(), 2);
}

TEST_F(VecTest, FilterVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  auto new_vec = vec.filter([](int x) { return x % 2 == 0; });

  EXPECT_EQ(new_vec.get(0).value(), 2);
  EXPECT_EQ(new_vec.get(1).value(), 4);
}



int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}