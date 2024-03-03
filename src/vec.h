//
// Created by eramoss on 03/03/24.
//
#ifndef DSUN_VEC_H
#define DSUN_VEC_H

#include <memory>

template <class T>
class Vec {
private:
    std::unique_ptr<T[]> data;
    uint32_t length;
    uint32_t cap;
    static constexpr uint32_t INITIAL_SIZE = 16;
public:
    Vec();
    void from_slice(T* slice, uint32_t len);
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
    data = std::make_unique<T[]>(cap);
}

template <class T>
void Vec<T>::from_slice(T* slice, uint32_t len) {
    length = len;
    cap = len * 2;
    data = std::make_unique<T[]>(cap);
    std::copy(slice, slice + length, data.get());
}


#endif //DSUN_VEC_H


