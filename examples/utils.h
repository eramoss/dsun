#include <fstream>
#include <stdexcept>
#include <type_traits>
#include "dsun.h" 

#define LIST_FILE "list.bin"

class Serializable {
public:
  virtual void serialize(std::ostream&) const = 0;
  virtual bool deserialize(std::istream&) = 0;
};

template<class T>
typename std::enable_if<std::is_base_of<Serializable, T>::value, uint8_t>::type
serialize_list_on_disk(const dsun::LinkedList<T>& list, const std::string& file_name = LIST_FILE) {
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
typename std::enable_if<std::is_base_of<Serializable, T>::value, dsun::LinkedList<T>>::type
deserialize_list_from_disk(const std::string& file_name = LIST_FILE) {
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
