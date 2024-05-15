#ifndef DSUN_RAW_BINARY_TREE
#define DSUN_RAW_BINARY_TREE

#include <memory>
namespace dsun {
  namespace raw {
    template <class T>
    struct node_t {
      T value;
      node_t* left;
      node_t* right;
      node_t(T value) : value(value) {}
      node_t() {}
    };

    template <class T>
    class BinaryTree {
    public:
      static node_t<T>* merge(node_t<T>* left, node_t<T>* right, node_t<T>* root);
      static node_t<T>* link_node_left(node_t<T>* root, node_t<T>* left);
      static node_t<T>* link_node_right(node_t<T>* root, node_t<T>* right);
      static node_t<T>* unlink_node_left(node_t<T>* root);
      static node_t<T>* unlink_node_right(node_t<T>* root);
      template <class F>
      static void pre_order_traversal(node_t<T>*& root, F f) {
        if (!root) {
          return;
        }
        f(root->value);
        pre_order_traversal(root->left, f);
        pre_order_traversal(root->right, f);
      }
    };

    template <class T>
    node_t<T>* BinaryTree<T>::merge(node_t<T>* left, node_t<T>* right, node_t<T>* root) {
      if (!left) {
        return right;
      }
      if (!right) {
        return left;
      }
      if (!root) {
        root = new node_t<T>();
      }
      root->left = left;
      root->right = right;
      return root;
    }

    template <class T>
    node_t<T>* BinaryTree<T>::link_node_left(node_t<T>* root, node_t<T>* left) {
      if (!root) {
        return left;
      }
      root->left = left;
      return root;
    }

    template <class T>
    node_t<T>* BinaryTree<T>::link_node_right(node_t<T>* root, node_t<T>* right) {
      if (!root) {
        return right;
      }
      root->right = right;
      return root;
    }

    template <class T>
    node_t<T>* BinaryTree<T>::unlink_node_left(node_t<T>* root) {
      if (!root) {
        return nullptr;
      }
      return root->left;
    }

    template <class T>
    node_t<T>* BinaryTree<T>::unlink_node_right(node_t<T>* root) {
      if (!root) {
        return nullptr;
      }
      return root->right;
    }
  }
}
#endif