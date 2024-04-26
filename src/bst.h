#ifndef DSUN_BST_H
#define DSUN_BST_H

#include <memory>
#include <optional>
#include <functional>

namespace dsun {
  template <typename T>
  class BST {
  private:
    struct Node {
      T value;
      std::optional<std::unique_ptr<Node>> left;
      std::optional<std::unique_ptr<Node>> right;
      Node(const T& value) : value(value), left(std::nullopt), right(std::nullopt) {}
    };
    using node_ptr_opt = std::optional<std::unique_ptr<Node>>;
    node_ptr_opt root;

  public:
    /*Constructors*/
    BST() : root(std::nullopt) {}
    BST(const BST<T>& other) {
      root = clone(other.root);
    }
    BST<T>& operator=(const BST<T>& other) {
      root = clone(other.root);
      return *this;
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


  };
}


#endif // DSUN_BST_H