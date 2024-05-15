//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_VEC_H
#define DSUN_VEC_H

#include <memory>
#include <optional>
#include <algorithm>
#include <functional>
#include "utils.h"

namespace dsun {
    template <class T>
    class Vec {
    private:
        std::unique_ptr<T[]> ptr;
        uint32_t length;
        uint32_t cap;
        static constexpr uint32_t INITIAL_SIZE = 16;
    public:
        /*
          Constructors
        */
        Vec();
        Vec(const Vec<T>& other) : length(other.length), cap(other.cap) {
            ptr = std::make_unique<T[]>(cap);
            std::copy(other.ptr.get(), other.ptr.get() + length, ptr.get());
        }
        template<std::size_t N>
        static Vec<T> from_slice(const T(&arr)[N]) {
            auto vec = Vec<T>::with_capacity(N * 2);
            for (std::size_t i = 0; i < N; ++i) {
                vec.push(arr[i]);
            }
            return vec;
        }
        static Vec<T> from_dyn_array(T* arr, uint32_t len) {
            auto vec = Vec<T>::with_capacity(len * 2);
            for (uint32_t i = 0; i < len; ++i) {
                vec.push(arr[i]);
            }
            return vec;
        }
        static Vec<T> from_list(std::initializer_list<T> list);
        static Vec<T> with_capacity(uint32_t capacity);

        /*
            Primary methods
        */
        std::unique_ptr<T[]>& as_slice() {
            return ptr;
        }
        std::optional<T> get(uint32_t index) const;
        std::optional<T*> get_mut(uint32_t index);
        std::optional<T> remove(uint32_t index);
        void push(T value);
        void insert(uint32_t index, T value) {
            if (index >= length) {
                throw std::out_of_range("Insertion index (is " + dsun_utils::to_string(index) + ") should be <= len (is " + dsun_utils::to_string(length) + ")");
            }
            if (length == cap) {
                cap *= 2;
                auto new_ptr = std::make_unique<T[]>(cap);
                std::copy(ptr.get(), ptr.get() + index, new_ptr.get());
                new_ptr.get()[index] = value;
                std::copy(ptr.get() + index, ptr.get() + length, new_ptr.get() + index + 1);
                ptr = std::move(new_ptr);
            }
            else {
                std::copy(ptr.get() + index, ptr.get() + length, ptr.get() + index + 1);
                ptr.get()[index] = value;
            }
            length++;
        }

        /*
          Getters
        */
        std::optional<T>  pop() {
            return this->get(--length).value();
        }
        std::optional<T>  last() const {
            return this->get(length - 1).value();
        }
        std::optional<T> first() const {
            return this->get(0);
        }
        T& operator[](uint32_t index) const {
            if (index >= length) {
                throw std::out_of_range("Index out of bounds: " + dsun_utils::to_string(index));
            }
            return this->ptr.get()[index];
        }

        Vec<T> operator=(const Vec<T>& other);

        bool contains(T value) {
            for (uint32_t i = 0; i < length; i++) {
                if (this->get(i).value() == value) {
                    return true;
                }
            }
            return false;
        }
        bool is_empty() {
            return length == 0;
        }
        std::optional<uint32_t> index_of(T value) {
            for (uint32_t i = 0; i < length; i++) {
                if (this->get(i).value() == value) {
                    return std::optional<uint32_t>(i);
                }
            }
            return std::nullopt;
        }
        [[nodiscard]] uint32_t len() const {
            return length;
        }
        [[nodiscard]] uint32_t capacity() const {
            return cap;
        }

        class Iterator {
        private:
            Vec<T>& vec;
            uint32_t index;
        public:
            Iterator(Vec<T>& v, uint32_t i) : vec(v), index(i) {}

            bool operator==(const Iterator& other) const {
                return vec[index] == other.vec[index];
            }

            bool operator!=(const Iterator& other) const {
                return vec[index] != other.vec[index];
            }

            Iterator& operator++() {
                index++;
                return *this;
            }

            T get() const {
                return vec.get(index).value();
            }
            T& get_mut() {
                return *vec.get_mut(index).value();
            }

            T operator*() const {
                return vec.get(index).value();
            }

            uint32_t get_index() const {
                return index;
            }

            bool has_next() {
                return index < vec.len();
            }
        };

        Iterator begin() {
            return Iterator(*this, 0);
        }

        Iterator end() {
            return Iterator(*this, length);
        }

        /*
          Higher order functions
        */
        Vec<T> map(std::function<T(const T&)> f);
        Vec<T> map_mut(std::function<void(T&)> f);
        Vec<T> filter(std::function<bool(T)> f);
    };

    template <class T>
    Vec<T>::Vec() : length(0), cap(INITIAL_SIZE) {
        static_assert(std::size_t{ sizeof(T) } > 0, "T must be complete type");
        ptr = std::make_unique<T[]>(cap);
    }

    template <class T>
    Vec<T> Vec<T>::from_list(std::initializer_list<T> list) {
        auto vec = Vec<T>::with_capacity(list.size() * 2);
        std::copy(list.begin(), list.end(), vec.ptr.get());
        vec.length = list.size();
        return vec;
    }

    template <class T>
    Vec<T> Vec<T>::with_capacity(uint32_t capacity) {
        Vec<T> vec;
        vec.cap = capacity;
        vec.ptr = std::make_unique<T[]>(capacity);
        return vec;
    }

    template <class T>
    std::optional<T> Vec<T>::get(uint32_t index) const {
        return this->ptr.get()[index] ? std::optional<T>(this->ptr.get()[index]) : std::nullopt;
    }

    template <class T>
    std::optional<T*> Vec<T>::get_mut(uint32_t index) {
        return this->ptr.get()[index] ? std::optional<T*>(&this->ptr.get()[index]) : std::nullopt;
    }

    template <class T>
    std::optional<T> Vec<T>::remove(uint32_t index) {
        if (index >= length) {
            return std::nullopt;
        }
        T value = ptr.get()[index];
        std::copy(ptr.get() + index + 1, ptr.get() + length, ptr.get() + index);
        length--;
        return std::optional<T>(value);
    }

    template <class T>
    void Vec<T>::push(T value) {
        if (length == cap) {
            cap *= 2;
            auto new_ptr = std::make_unique<T[]>(cap);
            std::copy(ptr.get(), ptr.get() + length, new_ptr.get());
            ptr = std::move(new_ptr);
        }
        ptr.get()[length++] = value;
    }

    template <class T>
    Vec<T> Vec<T>::operator=(const Vec<T>& other) {
        length = other.length;
        cap = other.cap;
        ptr = std::make_unique<T[]>(cap);
        std::copy(other.ptr.get(), other.ptr.get() + length, ptr.get());
        return *this;
    }

    template <class T>
    Vec<T> Vec<T>::map(std::function<T(const T&)> f) {
        Vec<T> new_vec;
        for (auto it = this->begin(); it.has_next(); ++it) {
            new_vec.push(f(it.get()));
        }
        return new_vec;
    }

    template <class T>
    Vec<T> Vec<T>::map_mut(std::function<void(T&)> f) {
        for (auto it = this->begin(); it.has_next(); ++it) {
            f(it.get_mut());
        }
        return *this;
    }

    template <class T>
    Vec<T> Vec<T>::filter(std::function<bool(T)> f) {
        Vec<T> new_vec;
        for (uint32_t i = 0; i < length; i++) {
            if (f(this->get(i).value())) {
                new_vec.push(this->get(i).value());
            }
        }
        return new_vec;
    }

}

#endif //DSUN_VEC_H


