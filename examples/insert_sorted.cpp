#include "dsun.h"
#include <iostream>
#include <iostream>
#include <sys/resource.h>

bool is_sorted(const dsun::LinkedList<char>& list) {
  if (list.empty()) return true;
  for (size_t i = 0; i < list.len() - 1; i++) {
    if (list[i].value() > list[i + 1].value()) {
      return false;
    }
  }
  return true;
}

void stupid_sort(dsun::LinkedList<char>& list) {
  for (size_t i = 0; i < list.len(); i++) {
    for (size_t j = i + 1; j < list.len(); j++) {
      if (list[i].value() > list[j].value()) {
        char temp = list[i].value();
        *list.at_mut(i).value() = list[j].value();
        *list.at_mut(j).value() = temp;
      }
    }
  }
}

void insert_sorted(dsun::LinkedList<char>& list, char value) {
  if (!is_sorted(list)) {
    stupid_sort(list);
  }
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
  auto list = dsun::LinkedList<char>::from_slice({ 'a', 'b', 'd', 'e' });

  list.clean();


  return 0;
}
