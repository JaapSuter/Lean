// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_EVAL_SPECIAL_FORMS_HPP_INCLUDED
#define FILE_LEAN_EVAL_SPECIAL_FORMS_HPP_INCLUDED

#include "lean/cons_ptr.hpp"
#include "lean/value.hpp"

namespace lean {

	class environment_t;

	namespace eval_special_forms {

		double  mod (double  lhs, double  rhs);
		double  div (double  lhs, double  rhs);
		double  mul (double  lhs, double  rhs);
		double  sub (double  lhs, double  rhs);
		double  add (double  lhs, double  rhs);
		
		bool eq  (double  lhs, double  rhs);
		bool neq (double  lhs, double  rhs);
		bool lteq(double  lhs, double  rhs);
		bool gteq(double  lhs, double  rhs);
		bool lt  (double  lhs, double  rhs);
		bool gt  (double  lhs, double  rhs);
		
		bool and_(bool lhs, bool rhs);
		bool or_ (bool lhs, bool rhs);

		void nop();
		
		value_t	   car(  const cons_ptr_t& arg, environment_t& env);
		cons_ptr_t cdr(  const cons_ptr_t& arg, environment_t& env);
		cons_ptr_t cons( const cons_ptr_t& arg, environment_t& env);
		cons_ptr_t quote(const cons_ptr_t& arg);
		
		bool is_nil(const cons_ptr_t& arg, environment_t& env);
		
		void print(cons_ptr_t arg, environment_t& env);
		
		value_t if_(const cons_ptr_t& arg, environment_t& env);
		value_t program(cons_ptr_t arg, environment_t& env);
		value_t assignment(const cons_ptr_t& arg, environment_t& env);

	}
}

#endif
