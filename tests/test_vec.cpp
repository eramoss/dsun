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

TEST_F(VecTest, CopyConstructorTest) {
  dsun::Vec<int> vec2 = vec;
  EXPECT_EQ(vec2.len(), 0);
  EXPECT_EQ(vec2.capacity(), 16); //  INITIAL_SIZE
}

TEST_F(VecTest, ConstructorByConstRefTest) {
  dsun::Vec<int> vec2(vec);
  EXPECT_EQ(vec2.len(), 0);
  EXPECT_EQ(vec2.capacity(), 16); //  INITIAL_SIZE
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

TEST_F(VecTest, InsertVecEmpty) {
  const int init_cap = 10;
  auto vec = dsun::Vec<int>::with_capacity(init_cap);
  EXPECT_THROW(vec.insert(init_cap / 2 + 1, 10), std::out_of_range);
}

TEST_F(VecTest, InsertVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec.insert(2, 10);


  EXPECT_EQ(vec.get(0).value(), 1);
  EXPECT_EQ(vec.get(1).value(), 2);
  EXPECT_EQ(vec.get(2).value(), 10);
  EXPECT_EQ(vec.get(3).value(), 3);
  EXPECT_EQ(vec.get(4).value(), 4);
  EXPECT_EQ(vec.get(5).value(), 5);
  EXPECT_EQ(vec.len(), 6);
}

TEST_F(VecTest, PushAndGrowVec) {
  auto vec = dsun::Vec<int>::from_list({ 1 });
  EXPECT_EQ(vec.capacity(), 2);
  // vec has capacity of 2 <- 2 * len in from_list
  vec.push(2);
  vec.push(3);
  EXPECT_EQ(vec.capacity(), 4); // initial * 2
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

  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 5);
}

TEST_F(VecTest, AssignmentOperatorIndexVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);
  vec[0] = 10;
  EXPECT_EQ(vec[0], 10);
}

TEST_F(VecTest, ContainsVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);

  EXPECT_TRUE(vec.contains(1));
  EXPECT_TRUE(vec.contains(2));
  EXPECT_TRUE(vec.contains(3));
  EXPECT_TRUE(vec.contains(4));
  EXPECT_TRUE(vec.contains(5));
  EXPECT_FALSE(vec.contains(6));
}

TEST_F(VecTest, IsEmptyVec) {
  auto vec = dsun::Vec<int>::with_capacity(10);

  EXPECT_TRUE(vec.is_empty());
  vec.push(1);
  EXPECT_FALSE(vec.is_empty());
}

TEST_F(VecTest, IndexOfVec) {
  int arr[5] = { 1, 2, 3, 4, 5 };
  auto vec = dsun::Vec<int>::from_slice(arr);

  EXPECT_EQ(vec.index_of(1).value(), 0);
  EXPECT_EQ(vec.index_of(2).value(), 1);
  EXPECT_EQ(vec.index_of(3).value(), 2);
  EXPECT_EQ(vec.index_of(4).value(), 3);
  EXPECT_EQ(vec.index_of(5).value(), 4);
  EXPECT_EQ(vec.index_of(6), std::nullopt);
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
  vec.map_mut([](int& x) {  x = x + 1; });

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



class IteratorTest : public testing::Test {
protected:
  dsun::Vec<int> vec;

  void SetUp() override {
    vec.push(1);
    vec.push(2);
    vec.push(3);
  }

};

TEST_F(IteratorTest, EqualityOperator) {
  dsun::Vec<int> vec_2 = vec;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    auto it_2 = vec_2.begin();
    EXPECT_EQ(it, it_2);
    ++it_2;
  }
}

TEST_F(IteratorTest, InequalityOperator) {
  dsun::Vec<int> vec_2 = vec;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    auto it_2 = vec_2.begin();
    ASSERT_EQ(it, it_2);
    ++it_2;
  }
}

TEST_F(IteratorTest, PreIncrementOperator) {
  auto it = vec.begin();
  EXPECT_EQ(it.get_index(), 0);
  ++it;
  EXPECT_EQ(it.get_index(), 1);
}

TEST_F(IteratorTest, GetMethod) {
  auto it = vec.begin();
  int value = it.get();
  EXPECT_EQ(value, 1);
  ++it;
  value = it.get();
  EXPECT_EQ(value, 2);
}

TEST_F(IteratorTest, GetMutMethod) {
  auto it = vec.begin();
  it.get_mut() = 10;
  EXPECT_EQ(it.get(), 10);
  it.get_mut() = 2;
  EXPECT_EQ(it.get(), 2);
}