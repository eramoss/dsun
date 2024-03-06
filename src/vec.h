//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_VEC_H
#define DSUN_VEC_H

#include <memory>
#include <optional>
#include <algorithm>
#include <functional>

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
        template<std::size_t N>
        static Vec<T> from_slice(const T(&arr)[N]) {
            auto vec = Vec<T>::with_capacity(N);
            for (std::size_t i = 0; i < N; ++i) {
                vec.push(arr[i]);
            }
            return vec;
        }
        static Vec<T> from_list(std::initializer_list<T> list);
        static Vec<T> with_capacity(uint32_t capacity);

        /*
          Primary methods
        */
        std::optional<T> get(uint32_t index) const;
        std::optional<T*> get_mut(uint32_t index);
        std::optional<T> remove(uint32_t index);
        void push(T value);

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
        std::optional<T> operator[](uint32_t index) const {
            return this->get(index);
        }
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

        /*
          Higher order functions
        */
        Vec<T> map(std::function<T(T)> f);
        Vec<T>* map_mut(std::function<void(T*)> f);
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
    Vec<T> Vec<T>::map(std::function<T(T)> f) {
        Vec<T> new_vec;
        for (uint32_t i = 0; i < length; i++) {
            new_vec.push(f(this->get(i).value()));
        }
        return new_vec;
    }

    template <class T>
    Vec<T>* Vec<T>::map_mut(std::function<void(T*)> f) {
        for (uint32_t i = 0; i < length; i++) {
            f(this->get_mut(i).value());
        }
        return this;
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


