#include <iostream>
#include "dsun.h"
using namespace dsun;
template<typename T>

LinkedList<T> DeMorgan(LinkedList<T>& self, LinkedList<T>& other) {
  auto result = LinkedList<T>();
  auto intersec = self.intersection(other);
  for (auto it = intersec.begin(); it != intersec.end(); ++it) {
    self.remove(*it);
    other.remove(*it);
  }
  result = LinkedList<T>::from_parts(self, other);
  return result;
}


int main(int argc, char const* argv[]) {
  dsun::LinkedList<int> list1 = dsun::LinkedList<int>::from_list({ 1, 2, 3, 4, 5 });
  dsun::LinkedList<int> list2 = dsun::LinkedList<int>::from_list({ 6, 7, 3, 9, 10 });

  auto result = DeMorgan(list1, list2);
  for (auto it = result.begin(); it.has_next(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  return 0;
}
