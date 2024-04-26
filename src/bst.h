#ifndef DSUN_BST_H
#define DSUN_BST_H

#include <memory>
#include <optional>
#include <functional>
#include <concepts>

namespace dsun {
  template <typename T>
  concept Comparable = requires(const T a, const T b) {
    {
      a < b
    } -> std::convertible_to<bool>;
    {
      a > b
    } -> std::convertible_to<bool>;
    {
      a == b
    } -> std::convertible_to<bool>;
    {
      a <=> b
    } -> std::convertible_to<std::partial_ordering>;

  };

  template <Comparable T>
  class BST {
  private:
    struct Node {
      T value;
      std::optional<std::shared_ptr<Node>> left;
      std::optional<std::shared_ptr<Node>> right;
      Node(const T& value) : value(value), left(std::nullopt), right(std::nullopt) {}
    };
    using node_ptr_opt = std::optional<std::shared_ptr<Node>>;
    node_ptr_opt root;

  public:
    /*Constructors*/
    BST() : root(std::nullopt) {}
    BST(const BST<T>& other) {
      root = other.root;
    }
    static BST<T> from_slice(const T* data, size_t len) {
      BST<T> bst;
      for (size_t i = 0; i < len; i++) {
        bst.insert(data[i]);
      }
      return bst;
    }
    static BST<T> from_list(const std::initializer_list<T> list) {
      BST<T> bst;
      for (const auto& item : list) {
        bst.insert(item);
      }
      return bst;
    }

    void insert(const T& value, bool allow_duplicates = false) {
      if (is_empty()) {
        root = std::make_shared<Node>(value);
        return;
      }
      node_ptr_opt* current = &root;
      while (current->has_value()) {
        int cmp = strong_ord_to_int(current->value()->value <=> value);
        switch (cmp) {
          case -1:
            current = &current->value()->right;
            break;
          case 1:
            current = &current->value()->left;
            break;
          case 0:
            if (allow_duplicates) {
              current = &current->value()->right;
              break;
            }
            else {
              return;
            }
        }
      }
      *current = std::make_shared<Node>(value);
    }

    bool contains(const T& value) {
      node_ptr_opt* current = &root;
      while (current->has_value()) {
        int cmp = strong_ord_to_int(current->value()->value <=> value);
        switch (cmp) {
          case -1:
            current = &current->value()->right;
            break;
          case 1:
            current = &current->value()->left;
            break;
          case 0:
            return true;
        }
      }
      return false;
    }

    /*Helpers*/
    bool is_empty() const {
      return !root.has_value();
    }

    int strong_ord_to_int(std::strong_ordering ord) const {
      if (ord == std::strong_ordering::less) {
        return -1;
      }
      else if (ord == std::strong_ordering::greater) {
        return 1;
      }
      else {
        return 0;
      }
    }
  };
}


#endif // DSUN_BST_H