
#include "../examples/utils.h"
#include "gtest/gtest.h"



class MockSerializable : public Serializable {
public:
  int data;
  MockSerializable(int data = 0) : data(data) {}

  void serialize(std::ostream& os) const override {
    os.write(reinterpret_cast<const char*>(&data), sizeof(int));
  }

  bool deserialize(std::istream& is) override {
    if (!is.read(reinterpret_cast<char*>(&data), sizeof(int)))
      return false;
    return true;
  }
};

class LinkedListTest : public ::testing::Test {
protected:
  dsun::LinkedList<MockSerializable> list;
};

TEST(SerializeTest, SerializeListOnDisk) {
  dsun::LinkedList<MockSerializable> list;
  auto list_copy = list;
  for (int i = 0; i < 10; ++i) {
    list.push_back(MockSerializable(i));
  }

  serialize_list_on_disk(list, LIST_FILE);

  std::ifstream file(LIST_FILE, std::ios::in | std::ios::binary);
  ASSERT_TRUE(file.good());

  auto des_list = deserialize_list_from_disk<MockSerializable>(LIST_FILE);
  ASSERT_EQ(des_list.len(), 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(des_list.at(i).value().data, i);
  }
}