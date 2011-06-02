// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_CONS_HPP_INCLUDED
#define FILE_LEAN_CONS_HPP_INCLUDED

#include "lean/value.hpp"
#include "lean/cons_ptr.hpp"
#include <cassert>
#include <iosfwd>

namespace lean {

struct cons_t
{
    cons_t() : _car(), _cdr(), _ref_count(1) {}
    
    cons_t(const cons_t& other)
        : _car(other._car), _cdr(other._cdr), _ref_count(1)
    {
    }

    cons_t& operator = (const cons_t& other)
    {
        _car = other._car;
        _cdr = other._cdr;
        return *this;
    }

    template<class T>
    explicit cons_t(const T& car, cons_ptr_t cdr = 0)
        : _car(car), _cdr(cdr), _ref_count(1) {}

    explicit cons_t(const char* car, cons_ptr_t cdr = 0)
        : _car(std::string(car)), _cdr(cdr), _ref_count(1) {}

    ~cons_t();

    const value_t&    car() const { return _car; }
    const cons_ptr_t& cdr() const { return _cdr; }
          value_t&    car()       { return _car; }
          cons_ptr_t& cdr()       { return _cdr; }

    void add_ref() { ++_ref_count; }
    void release() { if (_ref_count == 1) delete this; }

private:
    value_t     _car;
    cons_ptr_t  _cdr;
    mutable int _ref_count;    
};

template<class T>
cons_ptr_t cons(const T& car, const cons_ptr_t& cdr)
{
    return cons_ptr_t(new cons_t(car, cdr));
}

bool operator == (const cons_t& lhs, const cons_t& rhs);
bool operator != (const cons_t& lhs, const cons_t& rhs);

std::ostream& operator << (std::ostream&, const cons_t&);

cons_ptr_t append(const cons_ptr_t& a, const cons_ptr_t& b);

}

#endif
