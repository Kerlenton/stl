#ifndef FUNCTION_H
#define FUNCTION_H

#include "bool.hpp"

template <typename T1, typename T2>
inline bool operator!=(const T1& x, const T2& y) {
    return !(x == y);
}

template <typename T1, typename T2>
inline bool operator>(const T1& x, const T2& y) {
    return y < x;
}

template <typename T1, typename T2>
inline bool operator<=(const T1& x, const T2& y) {
    return !(y < x);
}

template <typename T1, typename T2>
inline bool operator>=(const T1& x, const T2& y) {
    return !(x < y);
}

template <typename Arg, typename Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

template <typename T>
struct plus: binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x + y; }
};

template <typename T>
struct minus: binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x - y; }
};

template <typename T>
struct times: binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x * y; }
};

template <typename T>
struct divides: binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x / y; }
};

template <typename T>
struct modulus: binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x % y; }
};

template <typename T>
struct negate: unary_function<T, T> {
    T operator()(const T& x) const { return -x; }
};

template <typename T>
struct equal_to: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x == y; }
};

template <typename T>
struct not_equal_to: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x != y; }
};

template <typename T>
struct greater: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x > y; }
};

template <typename T>
struct less: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x < y; }
};

template <typename T>
struct greater_equal: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x >= y; }
};

template <typename T>
struct less_equal: binary_function<T, T, bool> {
    T operator()(const T& x, const T& y) const { return x <= y; }
};

template <typename T>
struct logical_and: binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x && y; }
};

template <typename T>
struct logical_or: binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x || y; }
};

template <typename T>
struct logical_not: unary_function<T, bool> {
    bool operator()(const T& x) const { return !x; }
};

template <typename Predicate>
class unary_negate: public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate pred;
public:
    unary_negate(const Predicate& x): pred(x) {}
    bool operator()(const typename Predicate::argument_type& x) const { return !pred(x); }
};

template <typename Predicate>
unary_negate<Predicate> not1(const Predicate& pred) {
    return unary_negate<Predicate>(pred);
}

template <typename Predicate>
class binary_negate: public binary_function<typename Predicate::first_argument_type,
                                            typename Predicate::second_argument_type, bool> {
protected:
    Predicate pred;
public:
    binary_negate(const Predicate& x): pred(x) {}
    bool operator()(const typename Predicate::first_argument_type& x,
                    const typename Predicate::second_argument_type& y) const {
        return !pred(x);
    }
};

template <typename Predicate>
binary_negate<Predicate> not2(const Predicate& pred) {
    return binary_negate<Predicate>(pred);
}

template <typename Operation>
class binder1st: public unary_function<typename Operation::second_argument_type,
                                       typename Operation::result_type> {
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st(const Operation& x, const typename Operation::first_argument_type& y):
        op(x), value(y) {}
    typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
        return op(value, x);
    }
};

template <typename Operation, typename T>
binder1st<Operation> bind1st(const Operation& op, const T& x) {
    return binder1st<Operation>(op, Operation::first_argument_type(x));
}

template <typename Operation>
class binder2nd: public unary_function<typename Operation::first_argument_type,
                                       typename Operation::result_type> {
    Operation op;
    typename Operation::second_argument_type value;
public:
    binder2nd(const Operation& x, const typename Operation::second_argument_type& y):
        op(x), value(y) {}
    typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
        return op(x, value);
    }
};

template <typename Operation, typename T>
binder2nd<Operation> bind2nd(const Operation& op, const T& x) {
    return binder2nd<Operation>(op, Operation::second_argument_type(x));
}

template <typename Operation1, typename Operation2>
class unary_compose: public unary_function<typename Operation2::argument_type,
                                           typename Operation1::result_type> {
protected:
    Operation1 op1;
    Operation2 op2;
public:
    unary_compose(const Operation1& x, const Operation2& y): op1(x), op2(y) {}
    typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
        return op1(op2(x));
    }
};

template <typename Operation1, typename Operation2>
unary_compose<Operation1, Operation2> compose1(const Operation1& op1,
                                               const Operation2& op2) {
    return unary_compose<Operation1, Operation2>(op1, op2);
}

template <typename Operation1, typename Operation2, typename Operation3>
class binary_compose: public unary_function<typename Operation2::argument_type,
                                            typename Operation1::result_type> {
protected:
    Operation1 op1;
    Operation2 op2;
    Operation3 op3;
public:
    binary_compose(const Operation1& x, const Operation2& y,
                   const Operation3& z): op1(x), op2(y), op3(z) {}
    typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
        return op1(op2(x), op3(x));
    }
};

template <typename Operation1, typename Operation2, typename Operation3>
binary_compose<Operation1, Operation2, Operation3>
compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
    return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}

template <typename Arg, typename Result>
class pointer_to_unary_function: public unary_function<Arg, Result> {
protected:
    Result (*ptr)(Arg);
public:
    pointer_to_unary_function() {}
    pointer_to_unary_function(Result (*x)(Arg)): ptr(x) {}
    Result operator()(Arg x) const { return ptr(x); }
};

template <typename Arg, typename Result>
pointer_to_unary_function<Arg, Result> ptr_fun(Result (*x)(Arg)) {
    return pointer_to_unary_function<Arg, Result>(x);
}

template <typename Arg1, typename Arg2, typename Result>
class pointer_to_binary_function: public binary_function<Arg1, Arg2, Result> {
protected:
    Result (*ptr)(Arg1, Arg2);
public:
    pointer_to_binary_function() {}
    pointer_to_binary_function(Result (*x)(Arg1, Arg2)): ptr(x) {}
    Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};

template <typename Arg1, typename Arg2, typename Result>
pointer_to_binary_function<Arg1, Arg2, Result>
ptr_fun(Result (*x)(Arg1, Arg2)) {
    return pointer_to_binary_function<Arg1, Arg2, Result>(x);
}

#endif
