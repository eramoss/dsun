#ifndef DSUN_HASH_H
#define DSUN_HASH_H

#include <concepts>
#include <functional>
#include "vec.h"
#include <variant>


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

}
namespace dsun {
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
        table.set_len(table.len() + 1);
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
        table.set_len(table.len() - 1);
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

    class Entry {
      struct Occupied {
        K key;
        T value;
        HashMap<K, T>* base;
        Occupied(K key, T value, HashMap<K, T>* base) : key(key), value(value), base(base) {}
      };
      struct Vacant {
        K key;
        HashMap<K, T>* base;
        Vacant(K key, HashMap<K, T>* base) : key(key), base(base) {}
      };

      std::variant<Occupied, Vacant> entry;
    public:
      Entry(K key, T value, HashMap<K, T>* base) : entry(Occupied(key, value, base)) {}
      Entry(K key, HashMap<K, T>* base) : entry(Vacant(key, base)) {}

      T or_insert(T value) {
        if (std::holds_alternative<Occupied>(entry)) {
          return std::get<Occupied>(entry).base->get(std::get<Occupied>(entry).key).value();
        }
        Vacant vacant = std::get<Vacant>(entry);
        vacant.base->insert(vacant.key, value);
        return value;
      }

      Entry& and_modify(std::function<void(T&)> f) {
        if (std::holds_alternative<Occupied>(entry)) {
          f(*std::get<Occupied>(entry).base->get_mut(std::get<Occupied>(entry).key).value());
        }
        return *this;
      }
    };

    Entry entry(K key) {
      std::optional<T> entry = this->get(key);
      if (entry.has_value()) {
        return Entry(key, entry.value(), this);
      }
      return Entry(key, this);
    }

    class Iterator {
    private:
      HashMap<K, T>& map;
      uint32_t index;
      Node* node;
    public:
      Iterator(HashMap<K, T>& map, uint32_t index, Node* node) : map(map), index(index), node(node) {}

      bool operator==(const Iterator& other) const {
        return index == other.index;
      }

      bool operator!=(const Iterator& other) const {
        return index != other.index;
      }

      Iterator& operator++() {
        if (node->next != nullptr) {
          node = node->next;
          return *this;
        }
        index++;
        while (index < map.table.capacity() && map.table.get(index).has_value() == false) {
          index++;
        }
        if (index < map.table.capacity()) {
          node = map.table.get(index).value();
        }
        return *this;
      }

      std::pair<K, T> get() const {
        return { node->key, node->value };
      }
      std::pair<K, T>& get_mut() {
        return { node->key, node->value };
      }
      std::pair<K, T> operator*() const {
        return get();
      }

      uint32_t get_index() const {
        return index;
      }
    };

    Iterator begin() {
      uint32_t index = 0;
      while (index < table.capacity() && table.get(index).has_value() == false) {
        index++;
      }
      if (index < table.capacity()) {
        return Iterator(*this, index, table.get(index).value());
      }
      return Iterator(*this, index, nullptr);
    }

    Iterator end() {
      return Iterator(*this, table.capacity(), nullptr);
    }
  };
}

#endif // DSUN_HASH_H