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
      std::optional<std::shared_ptr<Node>> prev;

      Node map(std::function<void(const T&)> f) {
        while (this->next.has_value()) {
          f(this->data);
          this = this->next.value().get();
        }
      }
    };
    using node_ptr_opt = std::optional<std::shared_ptr<Node>>;

    node_ptr_opt head;
    node_ptr_opt tail;
    size_t size;

  public:
    LinkedList() : head(std::nullopt), tail(std::nullopt), size(0) {}

    LinkedList(const LinkedList<T>& other) {
      head = other.head;
      tail = other.tail;
      size = other.size;
    }

    void push_back(const T& data) {
      auto new_node = std::make_shared<Node>();
      new_node->data = data;
      push_back_node(new_node);
    }

    std::optional<T> pop_back() {
      return pop_back_node();
    }

    void push_front(const T& data) {
      auto new_node = std::make_shared<Node>();
      new_node->data = data;
      push_front_node(new_node);
    }

    std::optional<T> pop_front() {
      return pop_front_node();
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
  private:

    void push_back_node(node_ptr_opt node) {
      if (is_empty()) {
        head = node;
        tail = node;
      }
      else {
        tail.value()->next = node;
        node.value()->prev = tail;
        tail = node;
      }
      size++;
    }

    std::optional<T> pop_back_node() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto data = tail.value()->data;
      tail = tail.value()->prev;
      if (tail.has_value()) {
        tail.value()->next = std::nullopt;
      }
      size--;
      return std::optional<T>(data);
    }

    void push_front_node(node_ptr_opt node) {
      if (is_empty()) {
        head = node;
        tail = node;
      }
      else {
        head.value()->prev = node;
        node.value()->next = head;
        head = node;
      }
      size++;
    }

    std::optional<T> pop_front_node() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto data = head.value()->data;
      head = head.value()->next;
      if (head.has_value()) {
        head.value()->prev = std::nullopt;
      }
      size--;
      return std::optional<T>(data);
    }

    void unlink_node(node_ptr_opt node) {
      if (node.value()->prev.has_value()) {
        node.value()->prev.value()->next = node.value()->next;
      }
      if (node.value()->next.has_value()) {
        node.value()->next.value()->prev = node.value()->prev;
      }
    }

    void link_node_in_the_middle(node_ptr_opt node, node_ptr_opt prev, node_ptr_opt next) {
      node.value()->prev = prev;
      node.value()->next = next;
      prev.value()->next = node;
      next.value()->prev = node;
    }

  };
}


#endif