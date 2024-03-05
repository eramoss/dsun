//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_VEC_H
#define DSUN_VEC_H

#include <memory>
#include <optional>
#include <algorithm>
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
        static Vec<T> from_slice(T* slice);
        static Vec<T> with_capacity(uint32_t capacity);

        /*
          Primary methods
        */
        std::optional<T> get(uint32_t index);
        std::optional<T*> get_mut(uint32_t index);
        void push(T value);
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
    };

    template <class T>
    Vec<T>::Vec() : length(0), cap(INITIAL_SIZE) {
        static_assert(std::size_t{ sizeof(T) } > 0, "T must be complete type");
        ptr = std::make_unique<T[]>(cap);
    }

    template <class T>
    Vec<T> Vec<T>::from_slice(T* slice) {
        size_t len = 0;
        while (slice[len] != T{}) {
            len++;
        }
        auto vec = Vec<T>::with_capacity(len * 2);
        std::copy(slice, slice + len, vec.ptr.get());
        vec.length = len;
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
    std::optional<T> Vec<T>::get(uint32_t index) {
        return this->ptr.get()[index] ? std::optional<T>(this->ptr.get()[index]) : std::nullopt;
    }

    template <class T>
    std::optional<T*> Vec<T>::get_mut(uint32_t index) {
        return this->ptr.get()[index] ? std::optional<T*>(&this->ptr.get()[index]) : std::nullopt;
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

}

#endif //DSUN_VEC_H


