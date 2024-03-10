//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_LINKEDLIST_H
#define DSUN_LINKEDLIST_H

#include <memory>
#include <optional>

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


    /*
      Primary methods
    */
    void push_back(const T& data);
    void push_front(const T& data);
    std::optional<T> pop_back();
    std::optional<T> pop_front();


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
}


#endif