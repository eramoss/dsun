#include "utils.h"

int main() {
  auto list = dsun::LinkedList<int>();
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.push_back(5);

  serialize_list_on_disk(list);
  auto des_list = deserialize_list_from_disk<int>();

  for (auto it = des_list.begin(); it != des_list.end(); ++it) {
    std::cout << *it << std::endl;
  }

  return 0;
}