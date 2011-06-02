// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_EVAL_HPP_INCLUDED
#define FILE_LEAN_EVAL_HPP_INCLUDED

#include "lean/value.hpp"

namespace lean {

    class environment_t;

value_t eval(const value_t&, environment_t& env);

}

#endif
