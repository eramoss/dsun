#ifndef DSUN_BSTree_H
#define DSUN_BSTree_H

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
  class BSTree {
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
    BSTree() : root(std::nullopt) {}
    BSTree(const BSTree<T>& other) {
      root = other.root;
    }
    static BSTree<T> from_slice(const T* data, size_t len) {
      BSTree<T> BSTree;
      for (size_t i = 0; i < len; i++) {
        BSTree.insert(data[i]);
      }
      return BSTree;
    }
    static BSTree<T> from_list(const std::initializer_list<T> list) {
      BSTree<T> BSTree;
      for (const auto& item : list) {
        BSTree.insert(item);
      }
      return BSTree;
    }

    void insert(const T& value, bool allow_duplicates = false) {
      if (is_empty()) {
        root = std::make_shared<Node>(value);
        return;
      }
      traverse_find(&root, value, [&](node_ptr_opt* current) {
        if (!allow_duplicates && current->has_value()) {
          return;
        }
        *current = std::make_shared<Node>(value);
        });
    }

    bool contains(const T& value) {
      bool found = false;
      traverse_find(&root, value, [&](node_ptr_opt* current) {
        found = current->has_value();
        });
      return found;
    }

    std::optional<T> find(const T& value) {
      std::optional<T> result;
      traverse_find(&root, value, [&](node_ptr_opt* current) {
        if (current->has_value()) {
          result = current->value()->value;
        }
        });
      return result;
    }

    std::optional<T> min() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto node = traverse_extreme(true, &root);
      std::optional<T> result = node->value()->value;
      return result;
    }
    std::optional<T> extract_min() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto node = traverse_extreme(true, &root);
      std::optional<T> result = node->value()->value;
      *node = std::nullopt;
      return result;
    }
    std::optional<T> max() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto node = traverse_extreme(false, &root);
      std::optional<T> result = node->value()->value;
      return result;
    }
    std::optional<T> extract_max() {
      if (is_empty()) {
        return std::nullopt;
      }
      auto node = traverse_extreme(false, &root);
      std::optional<T> result = node->value()->value;
      *node = std::nullopt;
      return result;
    }
    /*Helpers*/
    bool is_empty() const {
      return !root.has_value();
    }


    ///  traverse find in the tree and apply the function f to the node that contains the value.
    ///  If the value is not found, apply the function f to the node where the value should be inserted.
    ///
    ///  # Example
    ///  ```cpp
    ///    bool found = false;
    ///    traverse_find(&root, value, [&](node_ptr_opt* current) {
    ///      found = current->has_value();
    ///       });
    ///     return found;
    ///  ``` 
    template<typename Func>
    void traverse_find(node_ptr_opt* root, const T& value, Func f) {
      node_ptr_opt* current = root;
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
            f(current);
            return;
        }
      }
      f(current);
    }

    node_ptr_opt* traverse_extreme(bool left, node_ptr_opt* current) {
      if (is_empty()) {
        return nullptr;
      }
      auto next = left ? &current->value()->left : &current->value()->right;
      while (next->has_value()) {
        current = next;
        next = left ? &current->value()->left : &current->value()->right;
      }
      return current;
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


#endif // DSUN_BSTree_H