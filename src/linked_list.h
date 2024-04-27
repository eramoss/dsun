//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_LINKEDLIST_H
#define DSUN_LINKEDLIST_H

#include <memory>
#include <optional>
#include <functional>
#include <iostream>
#include "utils.h"
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
        list.push_back(first.at(i).value());
      }
      for (size_t i = 0; i < second.len(); i++) {
        list.push_back(second.at(i).value());
      }

      return list;
    }
    LinkedList<T> operator=(const LinkedList<T>& other) {
      clean();
      for (auto it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
      }
      return *this;
    }
    void clean() {
      head = std::nullopt;
      tail = std::nullopt;
      size = 0;
    }
    ~LinkedList() {
      clean();
    }

    /*
      Primary methods
    */
    void push_back(const T& data) {
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
    void push_front(const T& data) {
      auto new_node = std::make_shared<Node>();
      new_node->data = data;
      new_node->next = head;

      if (!head) {
        tail = new_node;
      }
      head = new_node;
      size++;
    }

    void insert(size_t index, const T& data) {
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
    std::optional<T> pop_back() {
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
    std::optional<T> pop_front() {
      if (!head) {
        return std::nullopt;
      }

      auto current = head;
      head = current.value()->next;
      size--;
      return current.value()->data;
    }
    std::optional<T> remove_at(size_t index) {
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
    std::optional<T> remove(const T& data) {
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

    size_t len() const {
      return size;
    }
    std::optional<T> front() const {
      if (is_empty()) {
        return std::nullopt;
      }
      return head.value()->data;
    }
    std::optional<T> back() const {
      if (is_empty()) {
        return std::nullopt;
      }
      return tail.value()->data;
    }

    std::optional<T> get(const T& data) const {
      for (const auto& it : *this) {
        if (it == data) {
          return std::optional<T>(it);
        }
      }
      return std::nullopt;
    }

    std::optional<T*> get_mut(const T& data) {
      for (auto it = begin(); it != end(); ++it) {
        if (*it == data) {
          return std::optional<T*>(&it.get_mut());
        }
      }
      return std::nullopt;
    }

    std::optional<T> at(size_t index) const {
      if (index >= size || is_empty()) {
        return std::nullopt;
      }
      T result;
      map_index([&](T& item, size_t i) {
        if (i == index)
          result = item;
        });
      return std::optional<T>(result);
    }

    std::optional<T*> at_mut(size_t index) {
      if (index >= size || is_empty()) {
        return std::nullopt;
      }
      T* result;
      map_index([&](T& item, size_t i) {
        if (i == index)
          result = &item;
        });
      return std::optional<T*>(result);
    }

    T& operator[](size_t index) {
      if (index >= size) {
        throw std::out_of_range("Index out of range" + dsun_utils::to_string(index));
      }
      return *at_mut(index).value();
    }

    bool is_empty() const {
      return size == 0;
    }

    LinkedList<T> map(std::function<T(const T&)> f) const {
      auto list = LinkedList<T>();
      for (const auto& it : *this) {
        list.push_back(f(it));
      }
      return list;
    }

    LinkedList<T> map_index(std::function<void(T&, size_t)> f) const {
      for (auto it = begin(); it != end(); ++it) {
        f(it.get_mut(), it.index());
      }
      return *this;
    }

    void map_mut(std::function<void(T&)> f) {
      for (auto it = begin(); it != end(); ++it) {
        f(it.get_mut());
      }
    }

    LinkedList<T> filter(std::function<bool(const T&)> f) const {
      auto list = LinkedList<T>();
      for (const auto& it : *this) {
        if (f(it)) {
          list.push_back(it);
        }
      }
      return list;
    }

    class Iterator {
    private:
      node_ptr_opt current;
      size_t index_;
    public:
      Iterator(node_ptr_opt current) : current(current), index_(0) {}
      T operator*() {
        return current.value()->data;
      }
      T& get_mut() {
        return current.value()->data;
      }
      Iterator& operator++() {
        current = current.value()->next;
        index_++;
        return *this;
      }
      bool operator!=(const Iterator& other) const {
        return current != other.current;
      }
      bool operator==(const Iterator& other) const {
        return current == other.current;
      }
      size_t index() {
        return index_;
      }
      bool has_next() {
        return current.value()->next.has_value();
      }
    };
    Iterator begin() const {
      return Iterator(head);
    }
    Iterator end() const {
      return Iterator(std::nullopt);
    }
  };
}


#endif