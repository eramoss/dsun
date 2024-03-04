//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_VEC_H
#define DSUN_VEC_H

#include <memory>
#include <optional>
template <class T>
class Vec {
private:
    std::unique_ptr<T[]> ptr;
    uint32_t length;
    uint32_t cap;
    static constexpr uint32_t INITIAL_SIZE = 16;
public:
    Vec();
    void from_slice(T* slice, uint32_t len);
    std::optional<T> get(uint32_t index);
    std::optional<T*> get_mut(uint32_t index);
    void push(T value);


    [[nodiscard]] uint32_t len() const {
        return length;
    }
    [[nodiscard]] uint32_t capacity() const {
        return cap;
    }
};

template <class T>
Vec<T>::Vec() : length(0), cap(INITIAL_SIZE) {
    static_assert(std::size_t{ sizeof(T) } > 0, "T must be complete type");
    ptr = std::make_unique<T[]>(cap);
}

template <class T>
void Vec<T>::from_slice(T* slice, uint32_t len) {
    length = len;
    cap = len * 2;
    ptr = std::make_unique<T[]>(cap);
    std::copy(slice, slice + length, ptr.get());
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





#endif //DSUN_VEC_H


