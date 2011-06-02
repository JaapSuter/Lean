// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/environment.hpp"
#include "lean/exception.hpp"
#include "lean/eval_special_forms.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <cmath>

#ifdef _MSC_VER
    #pragma warning(disable:4503)
#endif

namespace lean {


environment_t::environment_t()
	: _display_fun_calls()
{
	def("+", &eval_special_forms::add);
	def("-", &eval_special_forms::sub);

	def("%", &eval_special_forms::mod);
	def("/", &eval_special_forms::div);
	def("*", &eval_special_forms::mul);

	def("==", &eval_special_forms::eq);
	def("!=", &eval_special_forms::neq);
	def("<=", &eval_special_forms::lteq);
	def(">=", &eval_special_forms::gteq);
	def("<",  &eval_special_forms::lt);
	def(">",  &eval_special_forms::gt);

	def("&&", &eval_special_forms::and_);
	def("||", &eval_special_forms::or_);
	
	def("nop", &eval_special_forms::nop);

	def("quote", &eval_special_forms::quote);
	
	def("cons",	   boost::function<cons_ptr_t (const cons_ptr_t&)>(boost::bind<cons_ptr_t>(&eval_special_forms::cons,	    _1, boost::ref(*this))));
	def("is_nil",  boost::function<bool		  (const cons_ptr_t&)>(boost::bind<bool  	 >(&eval_special_forms::is_nil,	    _1, boost::ref(*this))));
	def("car",	   boost::function<value_t	  (const cons_ptr_t&)>(boost::bind<value_t	 >(&eval_special_forms::car,	    _1, boost::ref(*this))));	
	def("cdr",	   boost::function<cons_ptr_t (const cons_ptr_t&)>(boost::bind<cons_ptr_t>(&eval_special_forms::cdr,	    _1, boost::ref(*this))));	
	def("print",   boost::function<void    	  (const cons_ptr_t&)>(boost::bind<void	     >(&eval_special_forms::print,	    _1, boost::ref(*this))));	
	def("program", boost::function<value_t 	  (const cons_ptr_t&)>(boost::bind<value_t	 >(&eval_special_forms::program,    _1, boost::ref(*this))));	
	def("if",	   boost::function<value_t 	  (const cons_ptr_t&)>(boost::bind<value_t	 >(&eval_special_forms::if_,	    _1, boost::ref(*this))));
	def("=",	   boost::function<value_t 	  (const cons_ptr_t&)>(boost::bind<value_t	 >(&eval_special_forms::assignment, _1, boost::ref(*this))));
}

struct invokation : boost::static_visitor<value_t>, boost::noncopyable
{
	environment_t& _env;
	cons_ptr_t _arg;

	invokation(environment_t& env, const cons_ptr_t& arg) : _env(env), _arg(arg) {}

	template<class T>
	value_t operator () (const T& t)
	{
		throw runtime_error_t(boost::format("unexpected type (%1%) for function application, with value: %2%") % typeid(T).name() % t);
	}

	value_t operator () (const identifier_t& identifier)
	{
		environment_t::foreign_fun_map::const_iterator iter = _env._foreign_funs.find(identifier.name());
		if (iter == _env._foreign_funs.end())
			return _env.call(_env.get(identifier), _arg);
		else
			return (*iter->second)(_arg);
	}

	value_t operator () (const cons_ptr_t& full_lambda_expression)
	{
		if (!is_lambda(full_lambda_expression->car()))
			throw runtime_error_t(boost::format("expected lambda expression, expr = %1%") % full_lambda_expression);

		cons_ptr_t lambda_exp = full_lambda_expression->cdr();
					
		if (!is_list(lambda_exp->car()))
			throw runtime_error_t(boost::format("lambda expression has no parameter list, expr = %1%") % lambda_exp);
		
		if (!lambda_exp->cdr())
			throw runtime_error_t(boost::format("lambda expression has no body, expr = %1%") % lambda_exp);

		cons_ptr_t param_list = as_list(lambda_exp->car());
		if (length(param_list) != length(_arg))
			throw runtime_error_t(boost::format("mismatch between number of arguments for application, lambda needs %1%, called with %2%") % length(param_list) % length(_arg));

		while (param_list)
		{
			try
			{
				value_t argument = eval(_arg->car(), _env);
				_env.set(boost::get<identifier_t>(param_list->car()), argument, 0);
			}
			catch (const boost::bad_get&)
			{
				throw runtime_error_t(boost::format("lambda parameter list can only consist of identifiers"));
			}

			param_list = param_list->cdr();
			_arg = _arg->cdr();
		}

		return eval(lambda_exp->cdr()->car(), _env);
	}
}; 

value_t environment_t::call(const value_t& fun, const cons_ptr_t& arg)
{
	struct scoped_context_push_pop : boost::noncopyable
	{
		context_stack& _cs;
		scoped_context_push_pop(context_stack& cs) :  _cs(cs) { _cs.push_front(context()); }
		~scoped_context_push_pop() { _cs.pop_front(); }
	};

	scoped_context_push_pop scpp(_context_stack);	
    invokation inv(*this, arg);
	
	if (_display_fun_calls)
	{
		for (std::size_t i = 0; i < _context_stack.size(); ++i)
			*_display_fun_calls << "  ";
		*_display_fun_calls << "function call: " << fun << '\n';
	}
	
	return boost::apply_visitor(inv, fun);
}

value_t environment_t::get(const identifier_t& id) const
{
	for (context_stack::const_iterator c = _context_stack.begin(), end = _context_stack.end(); c != end; ++c)
	{
		context::const_iterator jter = c->find(id.name());
		if (jter != c->end())
			return jter->second;
	}

	throw runtime_error_t(boost::format("identifier not found: %1%") % id.name());
}

void environment_t::set(const identifier_t& id, const value_t& v, int level)
{
	if (level >= static_cast<int>(_context_stack.size()))
		throw runtime_error_t(boost::format("currently not inside a context, can't set variable %1% to value %2%") % id % v);

	_context_stack[level][id.name()] = v;
}

void environment_t::display_fun_calls(std::ostream* ostr)
{ 
	_display_fun_calls = ostr;
}

}
