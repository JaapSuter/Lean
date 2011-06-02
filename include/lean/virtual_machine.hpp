// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_VIRTUAL_MACHINE_HPP_INCLUDED
#define FILE_LEAN_VIRTUAL_MACHINE_HPP_INCLUDED

#include "lean/cons_ptr.hpp"
#include "lean/value.hpp"
#include "lean/instruction.hpp"
#include <stack>
#include <deque>

namespace lean {

class virtual_machine_t
{
public:

	virtual_machine_t();

	value_t execute(const cons_ptr_t& compiled_sexp);

private:

	std::deque<value_t>		_stack;
	std::stack<bool>		_if_stack;
	std::stack<int>			_frame_ptr;
	std::vector<value_t>	_memory;
};

}

#endif
