#ifndef DSUN_VEC_DEQUE_H
#define DSUN_VEC_DEQUE_H
#include "vec.h"

template <typename T>
class VecDeque {
private:
  dsun::Vec<T> buf;
  uint32_t head;
  uint32_t len;
public:
  VecDeque() : head(0), len(0) {}
  VecDeque(const VecDeque<T>& other) : buf(other.buf), head(other.head), len(other.len) {}

  void push_back(T value) {
    if (len == buf.capacity()) {
      auto new_buf = dsun::Vec<T>::with_capacity(std::max(1u, buf.capacity() * 2));
      for (uint32_t i = 0; i < len; ++i) {
        new_buf.push(buf.get((head + i) % buf.capacity()).value());
      }
      buf = std::move(new_buf);
      head = 0;
    }
    buf.push(value);
    len += 1;
  }

  std::optional<T> pop_back() {
    if (len == 0) {
      return std::nullopt;
    }
    len -= 1;
    return buf.get((head + len) % buf.capacity());
  }

  void push_front(T value) {
    if (len == buf.capacity()) {
      auto new_buf = dsun::Vec<T>::with_capacity(std::max(1u, buf.capacity() * 2));
      for (uint32_t i = 0; i < len; ++i) {
        new_buf.push(buf.get((head + i) % buf.capacity()).value());
      }
      buf = std::move(new_buf);
      head = 0;
    }
    head = (head - 1) % buf.capacity();
    buf.insert(head, value);
    len += 1;
  }

  std::optional<T> pop_front() {
    if (len == 0) {
      return std::nullopt;
    }
    len -= 1;
    auto value = buf.get(head);
    head = (head + 1) % buf.capacity();
    return value;
  }

  T& operator[](uint32_t index) {
    if (index >= len) {
      throw std::out_of_range("Index out of range");
    }
    return buf[(head + index) % buf.capacity()];
  }

  [[nodiscard]] uint32_t size() const {
    return len;
  }
  [[nodiscard]] uint32_t capacity() const {
    return buf.capacity();
  }

};


#endif