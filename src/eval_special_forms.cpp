// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/eval_special_forms.hpp"
#include "lean/cons.hpp"
#include "lean/environment.hpp"

namespace lean {

	namespace eval_special_forms {

	double  mod (double  lhs, double  rhs) { return ::std::fmod(lhs, rhs);  }
	double  div (double  lhs, double  rhs) { return lhs / rhs;  }
	double  mul (double  lhs, double  rhs) { return lhs * rhs;  }
	double  sub (double  lhs, double  rhs) { return lhs - rhs;  }
	double  add (double  lhs, double  rhs) { return lhs + rhs;  }
	
	bool eq  (double  lhs, double  rhs) { return lhs == rhs; }
	bool neq (double  lhs, double  rhs) { return lhs != rhs; }
	bool lteq(double  lhs, double  rhs) { return lhs <= rhs; }
	bool gteq(double  lhs, double  rhs) { return lhs >= rhs; }
	bool lt  (double  lhs, double  rhs) { return lhs <  rhs; }
	bool gt  (double  lhs, double  rhs) { return lhs >  rhs; }
	
	bool and_(bool lhs, bool rhs) { return lhs && rhs; }
	bool or_ (bool lhs, bool rhs) { return lhs || rhs; }

	void nop() {}
	
	cons_ptr_t cons(const cons_ptr_t& arg, environment_t& env)
	{
		if (length(arg) != 2)
			throw std::exception("invalid number of arguments for cons");

        return cons(eval(arg->car(), env), as_list(eval(arg->cdr()->car(), env)));
	}

	cons_ptr_t quote(const cons_ptr_t& arg) 
	{ 
		if (length(arg) != 1)
			throw std::exception("invalid number of arguments for quote");

		return as_list(arg->car()); 
	}

	bool is_nil(const cons_ptr_t& arg, environment_t& env) 
	{
		if (length(arg) != 1)
			throw std::exception("invalid number of arguments for is_nil");

		return !as_list(eval(arg->car(), env));
	}

	value_t car(const cons_ptr_t& arg, environment_t& env) 
	{ 
		if (length(arg) != 1)
			throw std::exception("invalid number of arguments for car");

		return as_list(eval(arg->car(), env))->car(); 
	}

	cons_ptr_t cdr(const cons_ptr_t& arg, environment_t& env) 
	{ 
		if (length(arg) != 1)
			throw std::exception("invalid number of arguments for car");

		return as_list(eval(arg->car(), env))->cdr();
	}

	void print(cons_ptr_t arg, environment_t& env)
	{
		while (arg)
		{
			std::cout << eval(arg->car(), env);
			arg = arg->cdr();
			if (arg)
				std::cout << ' ';
		}
		std::cout << '\n';
	}

	value_t if_(const cons_ptr_t& arg, environment_t& env)
	{
		if (length(arg) < 2 || length(arg) > 3)
			throw std::exception("invalid number of arguments for if");

		value_t condition = eval(arg->car(), env);
		try																					
		{
			if (boost::get<bool>(condition))
				return eval(arg->cdr()->car(), env);
			else
			{
				if (arg->cdr()->cdr())
					return eval(arg->cdr()->cdr()->car(), env);
				else
					return nil;
			}
		}
		catch (const boost::bad_get&)														
		{																					
	        throw std::exception("::lean::runtime_error if statement expects boolean");	
		}
	}

	value_t program(cons_ptr_t arg, environment_t& env)
	{
		value_t ret = nil;
		while (arg)
		{
			ret = eval(arg->car(), env);				
			arg = arg->cdr();
		}
		return ret;
	}

	value_t assignment(const cons_ptr_t& arg, environment_t& env)
	{
		if (length(arg) != 2)
			throw std::exception("invalid number of arguments for assignment");

		try																					
		{
			const identifier_t& i = boost::get<identifier_t>(arg->car());
			env.set(i, eval(arg->cdr()->car(), env), 1);
			return arg->cdr()->car();
		}
		catch (const boost::bad_get&)														
		{																					
	        throw std::exception("::lean::runtime_error assignment requires identifier");	
		}
	}

	}
}
