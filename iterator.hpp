#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include <cstddef>
#include "bool.hpp"
#include "function.hpp"

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag {};
struct bidirectional_iterator_tag {};
struct random_access_iterator_tag {};

template <typename T, typename Distance> struct input_iterator {};
struct output_iterator {};
template <typename T, typename Distance> struct forward_iterator {};
template <typename T, typename Distance> struct bidirectional_iterator {};
template <typename T, typename Distance> struct random_access_iterator {};

template <typename T, typename Distance>
inline input_iterator_tag
iterator_category(const input_iterator<T, Distance>&) {
    return input_iterator_tag();
}

inline output_iterator_tag iterator_category(const output_iterator&) {
    return output_iterator_tag();
}

template <typename T, typename Distance>
inline forward_iterator_tag
iterator_category(const forward_iterator<T, Distance>&) {
    return forward_iterator_tag();
}

template <typename T, typename Distance>
inline bidirectional_iterator_tag
iterator_category(const bidirectional_iterator<T, Distance>&) {
    return bidirectional_iterator_tag();
}

template <typename T, typename Distance>
inline random_access_iterator_tag
iterator_category(const random_access_iterator<T, Distance>&) {
    return random_access_iterator_tag();
}

template <typename T, typename Distance>
inline T* value_type(const input_iterator<T, Distance>&) {
    return (T*)(0);
}

template <typename T, typename Distance>
inline T* value_type(const forward_iterator<T, Distance>&) {
    return (T*)(0);
}

template <typename T, typename Distance>
inline T* value_type(const bidirectional_iterator<T, Distance>&) {
    return (T*)(0);
}

template <typename T, typename Distance>
inline T* value_type(const random_access_iterator<T, Distance>&) {
    return (T*)(0);
}

template <typename T>
inline T* value_type(const T*) { return (T*)(0); }

template <typename T, typename Distance>
inline Distance* distance_type(const input_iterator<T, Distance>&) {
    return (Distance*)(0);
}

template <typename T, typename Distance>
inline Distance* distance_type(const forward_iterator<T, Distance>&) {
    return (Distance*)(0);
}

template <typename T, typename Distance>
inline Distance* distance_type(const bidirectional_iterator<T, Distance>&) {
    return (Distance*)(0);
}

template <typename T, typename Distance>
inline Distance* distance_type(const random_access_iterator<T, Distance>&) {
    return (Distance*)(0);
}

template <typename T>
inline ptrdiff_t* distance_type(const T*) { return (ptrdiff_t*)(0); }

template <typename Container>
class back_insert_iterator: public output_iterator {
protected:
    Container& container;
public:
    back_insert_iterator(Container& x): container(x) {}
    back_insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        container.push_backt(value);
        return *this;
    }
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
    back_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container> 
back_insert_iterator<Container> back_inserter(Container& x) {
    return back_insert_iterator<Container>(x);
}

template <typename Container>
class front_insert_iterator: public output_iterator {
protected:
    Container& container;
public:
    front_insert_iterator(Container& x): container(x) {}
    front_insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        container.push_front(value);
        return *this;
    }
    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container> 
front_insert_iterator<Container> front_inserter(Container& x) {
    return front_insert_iterator<Container>(x);
}

template <typename Container>
class insert_iterator: public output_iterator {
protected:
    Container& container;
    typename Container::iterator iter;
public:
    insert_iterator(Container& x, typename Container::iterator i):
        container(x), iter(i) {}
    insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        iter = container.insert(iter, value);
        ++iter;
        return *this;
    }
    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container, typename Iterator>
insert_iterator<Container> inserter(Container& x, Iterator i) {
    return insert_iterator<Container>(x, Container::iterator(i));
}

template <typename BidirectionalIterator, typename T, typename Reference,
          typename Distance>
class reverse_bidirectional_iterator:
    public bidirectional_iterator<T, Distance> {
    typedef reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                           Distance> self;
protected:
    BidirectionalIterator current;
public:
    reverse_bidirectional_iterator() {}
    reverse_bidirectional_iterator(BidirectionalIterator x): current(x) {}
    BidirectionalIterator base() { return current; }
    Reference operator*() const {
        BidirectionalIterator tmp = current;
        return *--tmp;
    }
    self& operator++() {
        --current;
        return *this;
    }
    self& operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--() {
        ++current;
        return *this;
    }
    self& operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }
};

template <typename BidirectionalIterator, typename T, typename Reference,
          typename Distance>
inline bool operator==(
    const reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                         Distance>& x, 
    const reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                         Distance>& y) {
    return x.current == y.current;
}

template <class RandomAccessIterator, class T, class Reference,
          class Distance> 
class reverse_iterator: public random_access_iterator<T, Distance> {
    typedef reverse_iterator<RandomAccessIterator, T, Reference, Distance>
	self;
protected:
    RandomAccessIterator current;
public:
    reverse_iterator() {}
    reverse_iterator(RandomAccessIterator x): current(x) {}
    RandomAccessIterator base() { return current; }
    Reference operator*() const { return *(current - 1); }
    self& operator++() {
        --current;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--() {
        ++current;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }
    self operator+(Distance n) const {
        return self(current - n);
    }
    self& operator+=(Distance n) {
        current -= n;
        return *this;
    }
    self operator-(Distance n) const {
        return self(current + n);
    }
    self& operator-=(Distance n) {
        current += n;
        return *this;
    }
    Reference operator[](Distance n) { return *(*this + n); }
};

template <class RandomAccessIterator, class T, class Reference, class Distance>
inline bool operator==(
    const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& x, 
    const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& y) {
    return x.current == y.current;
}

template <class RandomAccessIterator, class T, class Reference, class Distance>
inline bool operator<(
    const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& x, 
    const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& y) {
    return y.current < x.current;
}

template <class RandomAccessIterator, class T, class Reference, class Distance>
inline Distance operator-(
    const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& x, 
	const reverse_iterator<RandomAccessIterator, T,
                           Reference, Distance>& y) {
    return y.current - x.current;
}

template <class RandomAccessIterator, class T, class Reference, class Distance>
inline reverse_iterator<RandomAccessIterator, T, Reference, Distance> 
operator+(Distance n,
          const reverse_iterator<RandomAccessIterator, T, Reference,
          Distance>& x) {
    return reverse_iterator<RandomAccessIterator, T, Reference, Distance>
    (x.current - n);
}

template <class OutputIterator, class T>
class raw_storage_iterator: public output_iterator {
protected:
    OutputIterator iter;
public:
    raw_storage_iterator(OutputIterator x): iter(x) {}
    raw_storage_iterator<OutputIterator, T>& operator*() { return *this; }
    raw_storage_iterator<OutputIterator, T>& operator=(const T& element) {
        construct(iter, element);
        return *this;
    }        
    raw_storage_iterator<OutputIterator, T>& operator++() {
        ++iter;
        return *this;
    }
    raw_storage_iterator<OutputIterator, T> operator++(int) {
        raw_storage_iterator<OutputIterator, T> tmp = *this;
        ++iter;
        return tmp;
    }
};

template <class T, class Distance>
class istream_iterator: public input_iterator<T, Distance> {
protected:
    std::istream* stream;
    T value;
    bool end_marker;
    void read() {
        end_marker = (*stream) ? true : false;
        if (end_marker) *stream >> value;
        end_marker = (*stream) ? true : false;
    }
public:
    istream_iterator(): stream(&std::cin), end_marker(false) {}
    istream_iterator(std::istream& s): stream(&s) { read(); }
    const T& operator*() const { return value; }
    istream_iterator<T, Distance>& operator++() { 
        read(); 
        return *this;
    }
    istream_iterator<T, Distance> operator++(int) {
        istream_iterator<T, Distance> tmp = *this;
        read();
        return tmp;
    }
};

template <class T, class Distance>
bool operator==(const istream_iterator<T, Distance>& x,
                const istream_iterator<T, Distance>& y) {
    return x.stream == y.stream && x.end_marker == y.end_marker ||
    x.end_marker == false && y.end_marker == false;
}

template <class T>
class ostream_iterator: public output_iterator {
protected:
    std::ostream* stream;
    char* string;
public:
    ostream_iterator(std::ostream& s): stream(&s), string(0) {}
    ostream_iterator(std::ostream& s, char* c): stream(&s), string(c)  {}
    ostream_iterator<T>& operator=(const T& value) { 
        *stream << value;
        if (string) *stream << string;
        return *this;
    }
    ostream_iterator<T>& operator*() { return *this; }
    ostream_iterator<T>& operator++() { return *this; } 
    ostream_iterator<T>& operator++(int) { return *this; } 
};

#endif
