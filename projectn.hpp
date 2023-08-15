#ifndef PROJECTN_H
#define PROJECTN_H

#include "function.hpp"

template <typename T, typename U>
struct select1st: public unary_function<T, U> {
    const U& operator()(const T& x) const { return x.first; }
};

template <typename T, typename U>
struct ident: public unary_function<T, U> {
    const U& operator()(const T& x) const { return x; }
};

#endif
