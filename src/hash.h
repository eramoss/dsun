#ifndef DSUN_HASH_H
#define DSUN_HASH_H

#include <concepts>
#include <functional>
#include "vec.h"

namespace dsun {
  template <typename T>
  concept Hashable = requires(T a) {
    {
      std::hash<T>{}(a)
    } -> std::convertible_to<std::size_t>;
  };

  template <Hashable T>
  struct Hash {
    std::size_t operator()(const T& value) const {
      return std::hash<T>{}(value);
    }
  };
#define DERIVE_HASH(class_name) \
namespace std { \
template<> \
struct hash<class_name> { \
    size_t operator()(const class_name& obj) const { \
        size_t result = 0;

#define HASH_CLASS_ATTRIBUTE(attribute) \
        result ^= std::hash<decltype(obj.attribute)>()(obj.attribute) + 0x9e3779b9 + (result<<6) + (result>>2);

#define END_DERIVE_HASH() \
        return result; \
    } \
}; \
}
  template<typename Hashable, typename V>
  class HashMap {
  private:
    using K = Hashable;
    using T = V;
    struct Node {
      K key;
      T value;
      Node* next;
      Node(K key, T value, Node* next) : key(key), value(value), next(next) {}
    };
    dsun::Vec<Node*> table;

    std::size_t hash(const K& key) const {
      return Hash<K>{}(key) % table.capacity();
    }
  public:
    HashMap() : table(dsun::Vec<Node*>::with_capacity(16)) {}

    void insert(const K& key, const T& value) {
      std::size_t index = hash(key);
      Node* new_node = new Node(key, value, nullptr);
      if (table.get(index).has_value() == false) {
        table.as_slice()[index] = new_node;
        return;
      }
      Node* node = table.get(index).value();
      while (node->next != nullptr) {
        node = node->next;
      }
      node->next = new_node;
    }

    std::optional<T> remove(const K& key) {
      std::size_t index = hash(key);
      if (table.get(index).has_value() == false) {
        return std::nullopt;
      }
      Node* node = table.get(index).value();
      if (node->key == key) {
        table.as_slice()[index] = node->next;
        T value = node->value;
        delete node;
        return value;
      }
      while (node->next != nullptr) {
        if (node->next->key == key) {
          Node* next = node->next;
          node->next = next->next;
          T value = next->value;
          delete next;
          return value;
        }
        node = node->next;
      }
      return std::nullopt;
    }

    std::optional<T> get(const K& key) {
      std::size_t index = hash(key);
      if (table.get(index).has_value() == false) {
        return std::nullopt;
      }
      Node* node = table.get(index).value();
      while (node != nullptr) {
        if (node->key == key) {
          return node->value;
        }
        node = node->next;
      }
      return std::nullopt;
    }

    std::optional<T*> get_mut(const K& key) {
      std::size_t index = hash(key);
      if (table.get(index).has_value() == false) {
        return std::nullopt;
      }
      Node* node = table.get(index).value();
      while (node != nullptr) {
        if (node->key == key) {
          return &node->value;
        }
        node = node->next;
      }
      return std::nullopt;
    }

    bool contains_key(const K& key) {
      std::size_t index = hash(key);
      if (table.get(index).has_value() == false) {
        return false;
      }
      Node* node = table.get(index).value();
      while (node != nullptr) {
        if (node->key == key) {
          return true;
        }
        node = node->next;
      }
      return false;
    }


  };
}

template <typename Hashable, typename V>
class Entry {
  struct Occupied {
    Hashable key;
    V value;
    Occupied(Hashable key, V value) : key(key), value(value) {}
  };
  struct Vacant {
    Hashable key;
    Vacant(Hashable key) : key(key) {}
  };

  std::variant<Occupied, Vacant> entry;
public:
};



#endif // DSUN_HASH_H