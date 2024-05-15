#ifndef DSUN_RAW_BINARY_TREE
#define DSUN_RAW_BINARY_TREE

#include <memory>
namespace dsun {
  namespace raw {
    template <class T>
    struct node_t {
      T value;
      std::unique_ptr<node_t> left;
      std::unique_ptr<node_t> right;
      node_t(T value) : value(value) {}
      node_t() {}
    };

    template <class T>
    class BinaryTree {
    public:
      static std::unique_ptr<node_t<T>> merge(std::unique_ptr<node_t<T>> left, std::unique_ptr<node_t<T>> right, std::unique_ptr<node_t<T>> root);
      static std::unique_ptr<node_t<T>> link_node_left(std::unique_ptr<node_t<T>> root, std::unique_ptr<node_t<T>> left);
      static std::unique_ptr<node_t<T>> link_node_right(std::unique_ptr<node_t<T>> root, std::unique_ptr<node_t<T>> right);
      static std::unique_ptr<node_t<T>> unlink_node_left(std::unique_ptr<node_t<T>> root);
      static std::unique_ptr<node_t<T>> unlink_node_right(std::unique_ptr<node_t<T>> root);
    };

    template <class T>
    std::unique_ptr<node_t<T>> BinaryTree<T>::merge(std::unique_ptr<node_t<T>> left, std::unique_ptr<node_t<T>> right, std::unique_ptr<node_t<T>> root) {
      if (!left) {
        return right;
      }
      if (!right) {
        return left;
      }
      if (!root) {
        root = std::make_unique<node_t<T>>();
      }
      root->left = std::move(left);
      root->right = std::move(right);
      return root;
    }

    template <class T>
    std::unique_ptr<node_t<T>> BinaryTree<T>::link_node_left(std::unique_ptr<node_t<T>> root, std::unique_ptr<node_t<T>> left) {
      if (!root) {
        return left;
      }
      root->left = std::move(left);
      return root;
    }

    template <class T>
    std::unique_ptr<node_t<T>> BinaryTree<T>::link_node_right(std::unique_ptr<node_t<T>> root, std::unique_ptr<node_t<T>> right) {
      if (!root) {
        return right;
      }
      root->right = std::move(right);
      return root;
    }

    template <class T>
    std::unique_ptr<node_t<T>> BinaryTree<T>::unlink_node_left(std::unique_ptr<node_t<T>> root) {
      if (!root) {
        return nullptr;
      }
      return std::move(root->left);
    }

    template <class T>
    std::unique_ptr<node_t<T>> BinaryTree<T>::unlink_node_right(std::unique_ptr<node_t<T>> root) {
      if (!root) {
        return nullptr;
      }
      return std::move(root->right);
    }
  }
}
#endif