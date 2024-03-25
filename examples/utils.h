#include "linked_list.h"
#include <fstream>
#define LIST_FILE "build/list.bin"

template<class T>
uint8_t serialize_list_on_disk(dsun::LinkedList<T> list, const std::string& file_name = LIST_FILE) {
  auto file = std::fstream(file_name, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    return 1;
  }
  for (auto it = list.begin(); it != list.end(); ++it) {
    file.write(reinterpret_cast<const char*>(&(it.get_mut())), sizeof(T));
  }
  list.clean();
  file.close();
  return 0;
}


template<class T>
dsun::LinkedList<T> deserialize_list_from_disk(const std::string& file_name = LIST_FILE) {
  auto list = dsun::LinkedList<T>();
  std::ifstream file(file_name, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: <" + std::string(file_name) + "> for deserialize the list.");
  }
  T item;
  while (file.read(reinterpret_cast<char*>(&item), sizeof(T))) {
    list.push_back(item);
  }
  file.close();
  return list;
}