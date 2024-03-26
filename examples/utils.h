#include "dsun.h"
#include <fstream>
#define LIST_FILE "build/list.bin"

template<class T>
uint8_t serialize_list_on_disk(const dsun::LinkedList<T>& list, const std::string& file_name = LIST_FILE) {
  std::ofstream file(file_name, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    return 1;
  }
  for (const auto& item : list) {
    item.serialize(file);
  }
  file.close();
  return 0;
}

template<class T>
dsun::LinkedList<T> deserialize_list_from_disk(const std::string& file_name = LIST_FILE) {
  dsun::LinkedList<T> list;
  std::ifstream file(file_name, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for deserialization.");
  }
  while (true) {
    T item;
    if (!item.deserialize(file))
      break;
    list.push_back(item);
  }
  file.close();
  return list;
}