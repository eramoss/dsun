#ifndef DSUN_SWISSTABLE_H
#define DSUN_SWISSTABLE_H

#include <concepts>
#include <functional>
#include <iostream>
#include <emmintrin.h>

namespace SwissTables {

  template <typename T>
  concept Hashable = requires(T a) {
    {
      std::hash<T>{}(a)
    } -> std::convertible_to<std::size_t>;
  };
  namespace {
    typedef char ctrl_t;

    // ctrl_t is a 8-bit type
    // this is needed to be added SIMD instructions in the future
    // the first bit is used to indicate the status of the slot
    // the rest of the bits are used to store the top 7 bits of the hash
    enum Ctrl : ctrl_t {
      Empty = -128, // 0b10000000
      Deleted = -2, // 0b11111110
      // Full = 0b0xxxxxxx
    };

    size_t H1(size_t hash) {
      return hash >> 7;
    }
    ctrl_t H2(size_t hash) {
      auto top7 = hash >> (sizeof(size_t) * 8 - 7);
      return (ctrl_t)(top7 & 0x7F);
    }
    template <typename T>
    uint32_t TrailingZeros(T x) {
      return static_cast<uint32_t>(std::countr_zero(x));
    }


    // in future should be implemented as a hash that can
    // distribute the entropy uniformly
    // if the entropy is distributed in the last 7 bits at most
    // the ctrl bytes will have more collisions
    // if the entropy is distributed in the first 57 bits at most
    // the table will have more collisions
    // [1,2,3,4,5...57 bits][1,2,3...7 bits]
    //      raw table           metadata
    template <Hashable K>
    size_t swiss_hash(const K& key) {
      return std::hash<K>{}(key);
    }
    class NonZeroUnsignedInt {
    private:
      unsigned int value;

    public:
      NonZeroUnsignedInt(unsigned int val) {
        if (val == 0) {
          std::cerr << "Error: Value cannot be zero." << std::endl;
          value = 1; // Default to 1 if zero is provided
        }
        else {
          value = val;
        }
      }
      unsigned int getValue() const {
        return value;
      }
      void setValue(unsigned int val) {
        if (val == 0) {
          std::cerr << "Error: Value cannot be zero." << std::endl;
          return;
        }
        value = val;
      }
    };
    const uint64_t BITMASK_MASK = 0x8080808080808080;
    typedef NonZeroUnsignedInt non_zero_bit_mask;
    struct BitMask {
      uint32_t mask;
      BitMask() = default;
      BitMask(int mask) : mask(mask) {}

      BitMask invert() {
        return BitMask{ static_cast<int>(mask ^ BITMASK_MASK) };
      }

      uint32_t trailing_zeros() const {
        TrailingZeros(mask);
      }
      uint32_t highest_bit_set() const {
        return static_cast<uint32_t>(std::bit_width(mask) - 1);
      }

      uint32_t lowest_bit_set() const {
        return trailing_zeros();
      }

      using iterator = BitMask;
      using const_iterator = BitMask;
      BitMask& operator++() {
        mask &= mask - 1;
        return *this;
      }
      uint32_t operator*() const {
        return lowest_bit_set();
      }
      BitMask begin() const {
        return *this;
      }
      BitMask end() const {
        return BitMask(0);
      }
    };

    // abstraction over a group of control bytes which can be scanned in
    // parallel.
    //
    // this implementation uses a 128-bit SSE value.
    struct Group {
      __m128i data;

      BitMask match_byte(u_int8_t byte) {
        auto cmp = _mm_cmpeq_epi8(data, _mm_set1_epi8(byte));
        return BitMask{ _mm_movemask_epi8(cmp) };
      }

      BitMask match_empty() {
        return match_byte(Empty);
      }

      BitMask match_empty_or_deleted() {
        // A byte is EMPTY or DELETED if the high bit is set
        return BitMask{ _mm_movemask_epi8(data) };
      }

      BitMask match_full() {
        return match_empty_or_deleted().invert();
      }
    };

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
      return find(key, swiss_hash(key));
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
      size_t hash = swiss_hash(key);
      size_t pos_ = find_insert_slot(hash);
      ctrl_[pos_] = H2(hash);
      entries_[pos_] = { key, value };
      len_++;
    }



  private:
    void rehash() {
      size_t new_capacity = capacity_ * 2;
      ctrl_t* new_ctrl = new ctrl_t[new_capacity];
      Entry* new_entries = new Entry[new_capacity];
      std::fill(new_ctrl, new_ctrl + new_capacity, Empty);

      for (size_t i = 0; i < capacity_; i++) {
        if (ctrl_[i] == Empty || ctrl_[i] == Deleted) {
          continue;
        }
        size_t hash = swiss_hash(entries_[i].key);
        size_t pos_ = H1(hash) % capacity_;

        while (new_ctrl[pos_] != Empty) {
          pos_ = (pos_ + 1) % new_capacity;
        }

        new_entries[pos_] = entries_[i];
        new_ctrl[pos_] = ctrl_[i];
      }

      delete[] ctrl_;
      delete[] entries_;
      ctrl_ = new_ctrl;
      entries_ = new_entries;
      capacity_ = new_capacity;
    }
  };



}

#endif // DSUN_SWISSTABLE_H