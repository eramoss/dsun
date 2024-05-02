#ifndef DSUN_VEC_DEQUE_H
#define DSUN_VEC_DEQUE_H
#include "vec.h"
#include <assert.h>

template <typename T>
class VecDeque {
private:
  dsun::Vec<T> buf;
  uint32_t head;
  uint32_t len;
public:
  VecDeque() : head(0), len(0) {}
  VecDeque(const VecDeque<T>& other) : buf(other.buf), head(other.head), len(other.len) {}

  static VecDeque<T> with_capacity(uint32_t capacity) {
    VecDeque<T> deque;
    deque.buf = dsun::Vec<T>::with_capacity(capacity);
    return deque;
  }


  void push_back(T value) {
    if (is_full()) {
      grow();
    }
    buffer_write(to_physical_idx(len), value);
    len++;
  }

  std::optional<T> pop_back() {
    if (is_empty()) {
      return std::nullopt;
    }
    len--;
    return buffer_read(to_physical_idx(len));
  }


  void push_front(T value) {
    if (is_full()) {
      grow();
    }
    head = wrap_sub(head, 1);
    buffer_write(head, value);
    len++;
  }

  std::optional<T> pop_front() {
    if (is_empty()) {
      return std::nullopt;
    }
    uint32_t old_head = head;
    head = to_physical_idx(1);
    len--;
    return buffer_read(old_head);
  }

  void insert(uint32_t index, T value) {
    if (index > len) {
      throw std::out_of_range("Index out of bounds");
    }
    if (is_empty() || index == 0) {
      push_front(value);
      return;
    }
    if (index == len) {
      push_back(value);
      return;
    }
    if (is_full()) {
      grow();
    }
    if (index < len / 2) {
      shift_elements_left(index);
    }
    else {
      shift_elements_right(index);
    }

    buffer_write(to_physical_idx(index), value);
    len++;
  }

  std::optional<T> remove(uint32_t index) {
    if (index >= len || is_empty()) {
      return std::nullopt;
    }
    auto value = buffer_read(to_physical_idx(index));
    if (index < len / 2) {
      for (uint32_t i = index; i > 0; --i) {
        buffer_write(to_physical_idx(i), buffer_read(to_physical_idx(i - 1)));
      }
      head = wrap_add(head, 1);
    }
    else {
      for (uint32_t i = index; i < len - 1; ++i) {
        buffer_write(to_physical_idx(i), buffer_read(to_physical_idx(i + 1)));
      }
    }
    len--;
    return value;
  }

  T* make_contiguous() {
    if (is_contiguous()) {
      return buf.as_slice().get() + head;
    }
    auto new_buf = dsun::Vec<T>::with_capacity(capacity());
    for (uint32_t i = 0; i < len; ++i) {
      new_buf.push(buf.get(to_physical_idx(i)).value());
    }
    buf = std::move(new_buf);
    head = 0;
    return buf.as_slice().get();
  }

  /*Getters*/
  [[nodiscard]] uint32_t size() const {
    return len;
  }
  [[nodiscard]] uint32_t capacity() const {
    return buf.capacity();
  }
  [[nodiscard]] bool is_empty() const {
    return len == 0;
  }
  [[nodiscard]] bool is_full() const {
    return len == buf.capacity();
  }
  [[nodiscard]] bool is_contiguous() const {
    return head <= capacity() - size();
  }
  T operator[](uint32_t index) const {
    if (index >= len) {
      throw std::out_of_range("Index out of bounds");
    }
    return buf.get(to_physical_idx(index)).value();
  }

  std::optional<T> back() const {
    if (is_empty()) {
      return std::nullopt;
    }
    return buf.get(to_physical_idx(len - 1)).value();
  }
  std::optional<T> front() const {
    if (is_empty()) {
      return std::nullopt;
    }
    return buf.get(head).value();
  }

  /*Helpers*/
private:
  void grow() {
    buffer_resize(capacity() == 0 ? 1 : capacity() * 2);
  }

  void buffer_write(uint32_t index, T value) {
    buf.as_slice()[index] = value;
  }

  T buffer_read(uint32_t index) {
    return buf.as_slice()[index];
  }

  void buffer_resize(uint32_t new_capacity) {
    auto new_buf = dsun::Vec<T>::with_capacity(new_capacity);
    for (uint32_t i = 0; i < len; ++i) {
      new_buf.push(buf.get(to_physical_idx(i)).value());
    }
    buf = std::move(new_buf);
    head = 0;
  }

  uint32_t wrap_index(uint32_t logical_idx, uint32_t cap) const {
    assert((logical_idx == 0 && cap == 0)
      || logical_idx < cap
      || (logical_idx - cap) < cap);
    if (logical_idx >= cap) {
      return logical_idx - cap;
    }
    else {
      return logical_idx;
    }
  }

  uint32_t wrap_add(uint32_t index, uint32_t append) const {
    return wrap_index(index + append, capacity());
  }

  uint32_t wrap_sub(uint32_t index, uint32_t sub) const {
    return wrap_index(index - sub + capacity(), capacity());
  }

  uint32_t to_physical_idx(uint32_t index) const {
    return wrap_add(head, index);
  }

  void shift_elements_left(uint32_t index) {
    head = wrap_sub(head, 1);
    for (uint32_t i = 0; i < index; ++i) {
      buffer_write(wrap_add(head, i), buffer_read(wrap_add(head, i + 1)));
    }
  }

  void shift_elements_right(uint32_t index) {
    for (uint32_t i = len; i > index; --i) {
      buffer_write(to_physical_idx(i), buffer_read(to_physical_idx(i - 1)));
    }
  }

};


#endif