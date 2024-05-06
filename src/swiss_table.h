#ifndef DSUN_SWISSTABLE_H
#define DSUN_SWISSTABLE_H

#include <concepts>
#include <functional>
#include <iostream>
namespace SwissTables {

  template <typename T>
  concept Hashable = requires(T a) {
    {
      std::hash<T>{}(a)
    } -> std::convertible_to<std::size_t>;
  };
  namespace {
    typedef char ctrl_t;
    enum Ctrl : ctrl_t {
      Empty = -128, // 0b10000000
      Deleted = -2, // 0b11111110
      Sentinel = -1, // 0b11111111
      // Full = 0b0xxxxxxx
    };

    size_t H1(size_t hash) {
      return hash >> 7;
    }
    ctrl_t H2(size_t hash) {
      auto top7 = hash >> (sizeof(size_t) * 8 - 7);
      return (ctrl_t)(top7 & 0x7F);
    }

  }


  template <Hashable K, typename V>
  class FlatHashMap {
  private:
    struct Entry {
      K key;
      V value;
    };

    ctrl_t* ctrl_;
    Entry* entries_;
    size_t capacity_;
    size_t len_ = 0;


  public:
    FlatHashMap(size_t capacity_ = 16) : capacity_(capacity_) {
      ctrl_ = new ctrl_t[capacity_];
      entries_ = new Entry[capacity_];
      std::fill(ctrl_, ctrl_ + capacity_, Empty);
    }

    ~FlatHashMap() {
      delete[] ctrl_;
      delete[] entries_;
    }

    V* find(const K& key, size_t hash) {
      size_t pos_ = H1(hash) % capacity_;
      while (true) {
        if (H2(hash) == ctrl_[pos_] && entries_[pos_].key == key) {
          return &entries_[pos_].value;
        }
        if (ctrl_[pos_] == Empty) {
          return nullptr;
        }
        pos_ = pos_ + 1 % capacity_;
      }
    }

    V* find(const K& key) {
      return find(key, std::hash<K>{}(key));
    }


    // this function need at least one empty slot or deleted slot
    // if there is no empty slot, it will never returns
    size_t find_insert_slot(size_t hash) {
      size_t pos_ = H1(hash) % capacity_;
      while (true) {
        if (ctrl_[pos_] == Empty || ctrl_[pos_] == Deleted) {
          return pos_;
        }
        pos_ = pos_ + 1 % capacity_;
      }
    }

    void insert(const K& key, const V& value) {
      if (len_ * 2 > capacity_) {
        rehash();
      }
      size_t hash = std::hash<K>{}(key);
      size_t pos_ = find_insert_slot(hash);
      ctrl_[pos_] = H2(hash);
      entries_[pos_] = { key, value };
      len_++;
    }



  private:
    void rehash() {
      size_t new_capacity = capacity_ * 2;
      ctrl_t* new_ctrl_bytes = new ctrl_t[new_capacity];
      Entry* new_entries = new Entry[new_capacity];
      std::fill(new_ctrl_bytes, new_ctrl_bytes + new_capacity, Empty);

      for (size_t i = 0; i < capacity_; i++) {
        if (ctrl_[i] == Empty || ctrl_[i] == Deleted) {
          continue;
        }
        size_t hash = std::hash<K>{}(entries_[i].key);
        size_t pos_ = H1(hash) % capacity_;

        while (new_ctrl_bytes[pos_] != Empty) {
          pos_ = (pos_ + 1) % new_capacity;
        }

        new_entries[pos_] = entries_[i];
        new_ctrl_bytes[pos_] = ctrl_[i];
      }

      delete[] ctrl_;
      delete[] entries_;
      ctrl_ = new_ctrl_bytes;
      entries_ = new_entries;
      capacity_ = new_capacity;
    }
  };



}

#endif // DSUN_SWISSTABLE_H