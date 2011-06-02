// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_COMPILER_HPP_INCLUDED
#define FILE_LEAN_COMPILER_HPP_INCLUDED

#include "lean/cons_ptr.hpp"
#include "lean/value.hpp"
#include "lean/instruction.hpp"
#include <vector>
#include <deque>

namespace lean {

class compiler_t
{
private:

	typedef std::vector<identifier_t> context;
	typedef std::deque<context> var_registry;
	typedef std::deque<identifier_t> arg_stack;

	var_registry _vr;
	arg_stack	 _as;

	void push_context();
	void pop_context();
	double find_identifier(const identifier_t& id);

	friend class compiler_visitor_t;

public:

	cons_ptr_t compile(const value_t& v);
	
};


}

#endif
