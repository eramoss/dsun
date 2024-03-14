//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_LINKEDLIST_H
#define DSUN_LINKEDLIST_H

#include <memory>
#include <optional>
#include <utils.h>
#include <iostream>
namespace dsun {

  template<typename T>
  class LinkedList {
  private:
    struct Node {
      T data;
      std::optional<std::shared_ptr<Node>> next;
      ~Node() {
        if (std::getenv("DEBUG_CLEAN") && std::string(std::getenv("DEBUG_CLEAN")) == "true") {
          std::cout << "Deleting Node with data: " << data << std::endl;
        }
      }
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
        list.push_back(first.get_at(i).value());
      }
      for (size_t i = 0; i < second.len(); i++) {
        list.push_back(second.get_at(i).value());
      }

      return list;
    }

    void clean() {
      head = std::nullopt;
      tail = std::nullopt;
      size = 0;
    }

    /*
      Primary methods
    */
    void push_back(const T& data);
    void push_front(const T& data);
    void insert(size_t index, const T& data);
    std::optional<T> pop_back();
    std::optional<T> pop_front();
    std::optional<T> remove_at(size_t index);
    std::optional<T> remove(const T& data);

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
    std::optional<T> get_at(size_t index) const {
      if (index >= size) {
        return std::nullopt;
      }

      auto current = head;
      for (size_t i = 0; i < index; i++) {
        current = current.value()->next;
      }
      return current.value()->data;
    }
    std::optional<T*> get_at_mut(size_t index) {
      if (index >= size) {
        return std::nullopt;
      }

      auto current = head;
      for (size_t i = 0; i < index; i++) {
        current = current.value()->next;
      }
      return &current.value()->data;
    }

    std::optional<T> operator[](size_t index) const {
      return get_at(index);
    }
    bool empty() const {
      return size == 0;
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
  void LinkedList<T>::insert(size_t index, const T& data) {
    if (index > size) {
      return;
    }

    if (index == 0) {
      push_front(data);
      return;
    }

    if (index == size) {
      push_back(data);
      return;
    }

    auto new_node = std::make_shared<Node>();
    new_node->data = data;

    auto current = head;
    for (size_t i = 0; i < index - 1; i++) {
      current = current.value()->next;
    }

    new_node->next = current.value()->next;
    current.value()->next = new_node;
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
  std::optional<T> LinkedList<T>::remove_at(size_t index) {
    if (index >= size) {
      return std::nullopt;
    }

    if (index == 0) {
      return pop_front();
    }

    if (index == size - 1) {
      return pop_back();
    }

    auto current = head;
    std::optional<std::shared_ptr<Node>> prev = std::nullopt;
    for (size_t i = 0; i < index; i++) {
      prev = current;
      current = current.value()->next;
    }

    prev.value()->next = current.value()->next;
    size--;
    return current.value()->data;
  }

  template<typename T>
  std::optional<T> LinkedList<T>::remove(const T& data) {
    auto current = head;
    std::optional<std::shared_ptr<Node>> prev = std::nullopt;
    while (current) {
      if (current.value()->data == data) {
        if (prev) {
          prev.value()->next = current.value()->next;
        }
        else {
          head = current.value()->next;
        }
        size--;
        return current.value()->data;
      }
      prev = current;
      current = current.value()->next;
    }
    return std::nullopt;
  }

}


#endif