// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_EXPRESSION_BUILDER_STACK_HPP
#define FILE_EXPRESSION_BUILDER_STACK_HPP

#include <stack>
#include "lean/value.hpp"
#include "lean/cons_ptr.hpp"

namespace lean {

struct expression_builder_t
{
    void open_paren();
    void close_paren();
    void add(const value_t& value);

    value_t get() const;    

private:

    std::stack<cons_ptr_t*> _eol;
    value_t                 _expr;
};


}

#endif
