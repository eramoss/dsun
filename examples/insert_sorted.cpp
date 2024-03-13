#include "../src/linked_list.h"
#include <iostream>

void insert_sorted(dsun::LinkedList<char>& list, char value) {
  if (list.empty()) {
    list.push_back(value);
    return;
  }
  if (value < list.front().value()) {
    list.push_front(value);
    return;
  }
  if (value > list.back().value()) {
    list.push_back(value);
    return;
  }
  for (size_t i = 0; i < list.len(); i++) {
    if (value < list[i].value()) {
      list.insert(i, value);
      return;
    }
  }
}

int main(int argc, char const* argv[]) {
  dsun::LinkedList<char> list;
  insert_sorted(list, 'c');
  insert_sorted(list, 'a');
  insert_sorted(list, 'b');
  insert_sorted(list, 'd');
  insert_sorted(list, 't');
  insert_sorted(list, 'z');
  insert_sorted(list, 'y');
  insert_sorted(list, 'e');
  insert_sorted(list, 'x');
  insert_sorted(list, 'f');
  insert_sorted(list, 'g');

  for (size_t i = 0; i < list.len(); i++) {
    std::cout << list[i].value() << " ";
  }
  return 0;
}
