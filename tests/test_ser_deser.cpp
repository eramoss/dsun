#include "gtest/gtest.h"
#include "../examples/utils.h"

class SerDeSerTest : public ::testing::Test {
protected:
  dsun::LinkedList<int> list;
};

TEST_F(SerDeSerTest, serialize_list_null) {
  serialize_list_on_disk(list, "/tmp/list_null.bin");
  auto file = std::ifstream("/tmp/list_null.bin", std::ios::in | std::ios::binary);
  ASSERT_TRUE(file.good());
  EXPECT_EQ(file.peek(), std::ifstream::traits_type::eof());
  while (file.read(reinterpret_cast<char*>(&list), sizeof(int))) {
    FAIL() << "File should be empty";
  }
}

TEST_F(SerDeSerTest, serialize_list) {
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.push_back(5);
  serialize_list_on_disk(list, "/tmp/list.bin");


  auto file = std::ifstream("/tmp/list.bin", std::ios::in | std::ios::binary);
  file.seekg(0, std::ios::end); // move to the end
  std::streampos length = file.tellg();


  ASSERT_TRUE(file.good());
  EXPECT_NE(length, 4 * sizeof(int));
  EXPECT_EQ(length, 5 * sizeof(int));
  EXPECT_NE(length, 6 * sizeof(int));
}


TEST_F(SerDeSerTest, ser_and_then_deser) {
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.push_back(5);
  auto list_copy = list; // list is cleaned inside serialize_list_on_disk
  serialize_list_on_disk(list, "/tmp/list.bin");

  auto des_list = deserialize_list_from_disk<int>("/tmp/list.bin");

  for (auto it = des_list.begin(); it != des_list.end(); ++it) {
    EXPECT_EQ(*it, list_copy.front());
    list_copy.pop_front();
  }
}

TEST_F(SerDeSerTest, ser_and_then_deser_empty) {
  auto list_copy = list; // list is cleaned inside serialize_list_on_disk
  serialize_list_on_disk(list, "/tmp/list.bin");

  auto des_list = deserialize_list_from_disk<int>("/tmp/list.bin");

  auto file_len = std::ifstream("/tmp/list.bin", std::ios::in | std::ios::binary).tellg();

  EXPECT_EQ(file_len, 0);
  for (auto it = des_list.begin(); it != des_list.end(); ++it) {
    FAIL() << "List should be empty";
  }
}