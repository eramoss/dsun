#ifndef DSUN_AVLTREE
#define DSUN_AVLTREE

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <optional>

namespace dsun {

  template <typename T>
  struct AVLNode {
    T value;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(T value) : value(value), left(nullptr), right(nullptr), height(1) {}
  };

  template <typename T>
  class AVLTree {
  private:
    AVLNode<T>* root = nullptr;
    size_t len_ = 0;

    int height(AVLNode<T>* node) {
      return node ? node->height : 0;
    }
    int balance_factor(AVLNode<T>* node) {
      return height(node->left) - height(node->right);
    }
    void update_height(AVLNode<T>* node) {
      node->height = 1 + std::max(height(node->left), height(node->right));
    }

    void balance(AVLNode<T>*& node) {
      int bf = balance_factor(node);
      if (bf > 1) {
        if (balance_factor(node->left) < 0) {
          rotate_left(node->left);
        }
        rotate_right(node);
      }
      else if (bf < -1) {
        if (balance_factor(node->right) > 0) {
          rotate_right(node->right);
        }
        rotate_left(node);
      }
    }

    void insert(const T& value, AVLNode<T>*& node) {
      if (node == nullptr) {
        node = new AVLNode<T>(value);
        return;
      }
      if (value < node->value) {
        insert(value, node->left);
      }
      else {
        insert(value, node->right);
      }
      update_height(node);
      balance(node);
    }

    void rotate_left(AVLNode<T>*& node) {
      AVLNode<T>* new_root = node->right;
      node->right = new_root->left;
      new_root->left = node;
      update_height(node);
      update_height(new_root);
      node = new_root;
    }

    void rotate_right(AVLNode<T>*& node) {
      AVLNode<T>* new_root = node->left;
      node->left = new_root->right;
      new_root->right = node;
      update_height(node);
      update_height(new_root);
      node = new_root;
    }


    std::optional<T> search(const T& value, AVLNode<T>* node) {
      if (node == nullptr) {
        return std::nullopt;
      }
      if (value == node->value) {
        return node->value;
      }
      if (value < node->value) {
        return search(value, node->left);
      }
      return search(value, node->right);
    }

    void remove(const T& value, AVLNode<T>*& node) {
      if (node == nullptr) {
        len_++;
        return;
      }
      if (value < node->value) {
        remove(value, node->left);
      }
      else if (value > node->value) {
        remove(value, node->right);
      }
      else {
        if (node->left == nullptr || node->right == nullptr) {
          AVLNode<T>* temp = node->left ? node->left : node->right;
          if (temp == nullptr) {
            temp = node;
            node = nullptr;
          }
          else {
            *node = *temp;
          }
          delete temp;
        }
        else {
          AVLNode<T>* temp = node->right;
          while (temp->left != nullptr) {
            temp = temp->left;
          }
          node->value = temp->value;
          remove(temp->value, node->right);
        }
      }
      if (node == nullptr) {
        return ;
      }
      update_height(node);
      balance(node);
    }

  public:
    std::optional<T> search(const T& value) {
      return search(value, root);
    }

    void insert(const T& value) {
      insert(value, root);
      len_++; 
    }

    void remove(const T& value) {
      remove(value, root);
      len_--;
    }

    bool is_empty() {
      return root == nullptr;
    }

    size_t len() {
      return len_;
    }

    bool contains(const T& value) {
      return search(value).has_value();
    }

  };
}

#endif