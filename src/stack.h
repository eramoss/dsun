#ifndef DSUN_STACK
#define DSUN_STACK
#include "linked_list.h"
namespace dsun {
  template <typename T>
  class Stack {
  private:
    dsun::LinkedList<T> stack;
  public:
    Stack() : stack() {};
    Stack(dsun::LinkedList<T> stack) : stack(stack) {}

    void push(T item) {
      stack.push_front(item);
    }
    std::optional<T> pop() {
      return stack.pop_front();
    }
    std::optional<T> peek() {
      return stack.front();
    }
    bool is_empty() {
      return stack.empty();
    }
    size_t size() {
      return stack.len();
    }
  };
}
#endif