#ifndef DSUN_SWISSTABLE_H
#define DSUN_SWISSTABLE_H

#include <concepts>
#include <functional>
#include <iostream>
#include <optional>

#ifdef _MSC_VER
#include <intrin.h>
#endif
#ifndef _MSC_VER
#include <immintrin.h>
#endif
#include <bit>

namespace SwissTables {

  template <typename T>
  concept Hashable = requires(T a) {
    {
      std::hash<T>{}(a)
    } -> std::convertible_to<std::size_t>;
  };
  namespace {
    // ctrl_t is a 8-bit type
    // this is needed to be added SIMD instructions in the future
    // the first bit is used to indicate the status of the slot
    // the rest of the bits are used to store the top 7 bits of the hash
    enum class ctrl_t : int8_t {
      Empty = -128, // 0b10000000
      Deleted = -2, // 0b11111110
      // Full = 0b0xxxxxxx
    };

    inline size_t PerTableSalt(const ctrl_t* ctrl) {
      return reinterpret_cast<uintptr_t>(ctrl) >> 12;
    }
    inline size_t H1(size_t hash, const ctrl_t* ctrl) {
      return (hash >> 7) ^ PerTableSalt(ctrl);
    }

    inline uint8_t H2(size_t hash) {
      return hash & 0x7F;
    }
    inline bool IsEmpty(ctrl_t c) {
      return c == ctrl_t::Empty;
    }
    inline bool IsFull(ctrl_t c) {
      return c >= static_cast<ctrl_t>(0);
    }
    inline bool IsDeleted(ctrl_t c) {
      return c == ctrl_t::Deleted;
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
      size_t hash = 5381;
      auto k = key;
      while ((int)k != 0) {
        int remainder = key % 10;
        hash = ((hash << 5) + hash) + remainder; // hash * 33 + remainder
        k = k / 10;
      }
      return hash;
    }
    struct BitMask {
      uint16_t mask_;
      BitMask() = default;
      BitMask(uint16_t mask) : mask_(mask) {}

      uint32_t trailing_zeros() const {
        return TrailingZeros(mask_);
      }
      uint32_t highest_bit_set() const {
        return static_cast<uint32_t>(std::bit_width(mask_) - 1);
      }
      uint32_t lowest_bit_set() const {
        return trailing_zeros();
      }
      explicit operator bool() const {
        return this->mask_ != 0;
      }
      using iterator = BitMask;
      using const_iterator = BitMask;
      BitMask& operator++() {
        mask_ &= mask_ - 1;
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
    private:
      friend bool operator==(const BitMask& a, const BitMask& b) {
        return a.mask_ == b.mask_;
      }
      friend bool operator!=(const BitMask& a, const BitMask& b) {
        return a.mask_ != b.mask_;
      }
    };

    // abstraction over a group of control bytes which can be scanned in
    // parallel.
    //
    // this implementation uses a 128-bit SSE value.
    struct Group {
      __m128i data;

      BitMask match_byte(int8_t byte) {
        auto match = _mm_set1_epi8(static_cast<char>(byte));
        return BitMask{ static_cast<uint16_t>(_mm_movemask_epi8(_mm_cmpeq_epi8(match, data))) };
      }
      BitMask match_empty() {
        auto match = _mm_set1_epi8(static_cast<char>(ctrl_t::Empty));
        return BitMask{ static_cast<uint16_t>(_mm_movemask_epi8(_mm_cmpeq_epi8(match, data))) };
      }
      static Group load(const ctrl_t* ptr) {
        return Group{ _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr)) };
      }
    };

  }
  template <Hashable K, typename V>
  class FlatHashMap {
  private:
    struct slots_t {
      K key;
      V value;
    };
    ctrl_t* ctrl_;
    slots_t* slots_;
    size_t capacity_;
    size_t len_ = 0;
  public:

    FlatHashMap(size_t capacity = 16) : capacity_(capacity) {
      ctrl_ = new ctrl_t[capacity_];
      slots_ = new slots_t[capacity_];
      std::fill(ctrl_, ctrl_ + capacity_, ctrl_t::Empty);
    }

    ~FlatHashMap() {
      delete[] ctrl_;
      delete[] slots_;
    }

    bool insert(const K& key, const V& value) {
      size_t hash = swiss_hash(key);
      size_t index = hash % capacity_;
      size_t perturb = hash;
      size_t i = 0;
      while (!IsEmpty(ctrl_[index]) && !IsDeleted(ctrl_[index])) {
        if (slots_[index].key == key) {
          return false;
        }
        i++;
        index = (index * 5 + 1 + i) % capacity_;
        perturb >>= 5;
        index = (index + perturb) % capacity_;
      }
      slots_[index].key = key;
      slots_[index].value = value;
      ctrl_[index] = static_cast<ctrl_t>(hash >> 7);
      len_++;
      return true;
    }

    class iterator {
    private:
      slots_t* slots_;
      ctrl_t* ctrl_;
      size_t index_;
      size_t capacity_;
    public:
      iterator(slots_t* slots, ctrl_t* ctrl, size_t index, size_t capacity) : slots_(slots), ctrl_(ctrl), index_(index), capacity_(capacity) {}
      iterator& operator++() {
        do {
          index_++;
        } while (index_ < capacity_ && IsEmpty(ctrl_[index_]) || IsDeleted(ctrl_[index_]));
        return *this;
      }
      slots_t& operator*() {
        return slots_[index_];
      }
      slots_t* operator->() {
        return &slots_[index_];
      }
      bool operator==(const iterator& other) const {
        return index_ == other.index_;
      }
      bool operator!=(const iterator& other) const {
        return !(*this == other);
      }
    };
    iterator begin() const {
      size_t index = 0;
      while (index < capacity_ && (IsEmpty(ctrl_[index]) || IsDeleted(ctrl_[index]))) {
        index++;
      }
      return iterator(slots_, ctrl_, index, capacity_);
    }
    iterator end() const {
      return iterator(slots_, ctrl_, capacity_, capacity_);
    }
    iterator iterator_at(size_t index)const {
      return iterator(slots_, ctrl_, index, capacity_);
    }
  };
}

#endif // DSUN_SWISSTABLE_H