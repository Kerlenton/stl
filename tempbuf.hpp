#ifndef TEMPBUF_H
#define TEMPBUF_H

#include <cstddef>
#include "pair.hpp"

template <typename T>
pair<T*, std::ptrdiff_t> get_temporary_buffer(std::ptrdiff_t count) {
    T* buffer = new T[count];
    std::ptrdiff_t size = count * sizeof(T);
    return make_pair<T*, std::ptrdiff_t>(buffer, size);
}

template <typename T>
void return_temporary_buffer(T* buffer) {
    delete[] buffer;
}

#endif
