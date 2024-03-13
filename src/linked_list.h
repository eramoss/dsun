//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_LINKEDLIST_H
#define DSUN_LINKEDLIST_H

#include <memory>
#include <optional>
#include <utils.h>
namespace dsun {

  template<typename T>
  class LinkedList {
  private:
    struct Node {
      T data;
      std::optional<std::shared_ptr<Node>> next;
    };
    using node_ptr_opt = std::optional<std::shared_ptr<Node>>;

    node_ptr_opt head;
    node_ptr_opt tail;
    size_t size;

  public:

    /*
      Constructors
    */
    LinkedList() : size(0) {
      head = std::nullopt;
      tail = std::nullopt;
    }
    static LinkedList<T> from_arr(const T* data, size_t len) {
      LinkedList<T> list;
      for (size_t i = 0; i < len; i++) {
        list.push_back(data[i]);
      }
      return list;
    }
    template<std::size_t N>
    static LinkedList<T> from_slice(const T(&arr)[N]) {
      auto list = LinkedList<T>();
      for (std::size_t i = 0; i < N; ++i) {
        list.push_back(arr[i]);
      }
      return list;
    }
    static LinkedList<T> from_list(const std::initializer_list<T> list) {
      LinkedList<T> linked_list;
      for (const auto& item : list) {
        linked_list.push_back(item);
      }
      return linked_list;
    }

    static LinkedList<T> from_parts(const LinkedList<T>& first, const LinkedList<T>& second) {
      auto list = LinkedList<T>();
      for (size_t i = 0; i < first.len(); i++) {
        list.push_back(first.get(i).value());
      }
      for (size_t i = 0; i < second.len(); i++) {
        list.push_back(second.get(i).value());
      }

      return list;
    }


    /*
      Primary methods
    */
    void push_back(const T& data);
    void push_front(const T& data);
    std::optional<T> pop_back();
    std::optional<T> pop_front();
    std::pair<LinkedList<T>, LinkedList<T>> split_at(size_t index);

    /*
      Getters
    */
    size_t len() const {
      return size;
    }
    std::optional<T> front() const {
      if (!head) {
        return std::nullopt;
      }
      return head.value()->data;
    }
    std::optional<T> back() const {
      if (!tail) {
        return std::nullopt;
      }
      return tail.value()->data;
    }
    std::optional<T> get(size_t index) const {
      if (index >= size) {
        return std::nullopt;
      }

      auto current = head;
      for (size_t i = 0; i < index; i++) {
        current = current.value()->next;
      }
      return current.value()->data;
    }
    std::optional<T> operator[](size_t index) const {
      return get(index);
    }
    bool empty() const {
      return size == 0;
    }

    /*
      Secondary functions
    */
    LinkedList<T> merge(const LinkedList<T>& other) const {
      auto list = LinkedList<T>::from_parts(*this, other);
      return list;
    }
    std::optional<T> insert(size_t index, const T& data) {
      auto [list1, list2] = split_at(index - 1);
      list1.push_back(data);
      *this = from_parts(list1, list2);
      return data;
    }

  };


  template<typename T>
  void LinkedList<T>::push_back(const T& data) {
    auto new_node = std::make_shared<Node>();
    new_node->data = data;
    new_node->next = std::nullopt;

    if (!head) {
      head = new_node;
    }
    else {
      tail.value()->next = new_node;
    }
    tail = new_node;
    size++;
  }

  template<typename T>
  void LinkedList<T>::push_front(const T& data) {
    auto new_node = std::make_shared<Node>();
    new_node->data = data;
    new_node->next = head;

    if (!head) {
      tail = new_node;
    }
    head = new_node;
    size++;
  }

  template<typename T>
  std::optional<T> LinkedList<T>::pop_back() {
    if (!head) {
      return std::nullopt;
    }

    auto current = head;
    std::optional<std::shared_ptr<Node>> prev = std::nullopt;
    while (current.value()->next) {
      prev = current;
      current = current.value()->next;
    }

    if (prev) {
      prev.value()->next = std::nullopt;
      tail = prev;
    }
    else {
      head = std::nullopt;
      tail = std::nullopt;
    }
    size--;
    return current.value()->data;
  }

  template<typename T>
  std::optional<T> LinkedList<T>::pop_front() {
    if (!head) {
      return std::nullopt;
    }

    auto current = head;
    head = current.value()->next;
    size--;
    return current.value()->data;
  }

  template<typename T>
  std::pair<LinkedList<T>, LinkedList<T>> LinkedList<T>::split_at(size_t index) {
    LinkedList<T> first;
    LinkedList<T> second;
    index = index + 1;
    if (index >= size) {
      return { first, second };
    }

    auto current = head;
    for (size_t i = 0; i < index; i++) {
      first.push_back(current.value()->data);
      current = current.value()->next;
    }

    while (current) {
      second.push_back(current.value()->data);
      current = current.value()->next;
    }

    return { first, second };
  }

}


#endif