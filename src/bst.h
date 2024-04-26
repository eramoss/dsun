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
  };
}


#endif // DSUN_BST_H