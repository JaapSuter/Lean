// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/compiler.hpp"
#include "lean/value.hpp"
#include "lean/cons.hpp"
#include "lean/exception.hpp"
#include <deque>
#include <vector>
#include <algorithm>

namespace lean {

class compiler_visitor_t : public boost::static_visitor<cons_ptr_t>, boost::noncopyable
{
	compiler_t& _compiler;

public:

	compiler_visitor_t(compiler_t& comp) : _compiler(comp) {}

	template<class T>
    cons_ptr_t operator () (const T& t) const
    {
		return cons(LOAD_CONST, cons(t, nil));
    }

	cons_ptr_t operator () (const identifier_t& identifier) const
    {
		for (std::size_t i = 0; i < _compiler._as.size(); ++i)
		{
			if (_compiler._as[i] == identifier)
			{
				return cons(LOAD_ARG, cons(static_cast<double>(i), nil));
			}
		}

		return cons(LOAD_VAR, cons(_compiler.find_identifier(identifier), nil));
    }

    cons_ptr_t operator () (const cons_ptr_t& ptr) const
    {
		if (!ptr)
		{
			return cons(QUOTE, cons(nil, nil));
		}

		struct scoped_context_push_pop : boost::noncopyable
		{
			compiler_t& _cs;
			scoped_context_push_pop(compiler_t& cs) :  _cs(cs) { _cs.push_context(); }
			~scoped_context_push_pop() {  _cs.pop_context(); }
		};

		scoped_context_push_pop scpp(_compiler);	

		if (is<identifier_t>(ptr->car()))
		{
			const identifier_t& fun = boost::get<identifier_t>(ptr->car());
			
			#define LEAN_binary_function_impl(_name_, _instruction_)	\
				if (fun.name() == _name_)								\
				{														\
					assert(length(ptr->cdr()) == 2);					\
					return append(_compiler.compile(ptr->cdr()->car()), append(_compiler.compile(ptr->cdr()->cdr()->car()), cons(_instruction_, nil)));	\
				} else

			LEAN_binary_function_impl("+", ADD)
			LEAN_binary_function_impl("-", SUB)
			LEAN_binary_function_impl("*", MUL)
			LEAN_binary_function_impl("/", DIV)
			LEAN_binary_function_impl("%", MOD)
			LEAN_binary_function_impl("&&", AND)
			LEAN_binary_function_impl("||", OR)
			LEAN_binary_function_impl("==", EQ)
			LEAN_binary_function_impl("!=", NEQ)
			LEAN_binary_function_impl("<=", LEQ)
			LEAN_binary_function_impl(">=", GEQ)
			LEAN_binary_function_impl("<", LE)
			LEAN_binary_function_impl(">", GE)
			#undef LEAN_binary_function_impl

			if (fun.name() == "quote")
			{
				assert(length(ptr->cdr()) == 1);
				return cons(QUOTE, cons(ptr->cdr()->car(), nil));
			}
			else
			if (fun.name() == "if")
			{
				const int num_args = length(ptr->cdr());
				assert(num_args == 2 || num_args == 3);
				value_t cond = ptr->cdr()->car();
				value_t then = ptr->cdr()->cdr()->car();
				value_t els  = num_args == 3 ? ptr->cdr()->cdr()->cdr()->car() : nil;
				
				cons_ptr_t compiled_cond = _compiler.compile(cond);
				cons_ptr_t compiled_then = _compiler.compile(then);
				cons_ptr_t compiled_else = _compiler.compile(els );

				return append(
							append(
								append(
									append(cons(IF, compiled_cond),
				   						cons(SKIP, cons(static_cast<double>(length(compiled_then) + 3), nil))),
									compiled_then),
										cons(SKIP, cons(static_cast<double>(length(compiled_else) + 1), nil))),
									compiled_else);
			}
			else
			if (fun.name() == "program")
			{
				cons_ptr_t p;
				cons_ptr_t arg = ptr->cdr();
				while (arg)
				{
					p = append(p, _compiler.compile(arg->car()));
                    arg = arg->cdr();
				}
				return p;
			}
			else
			if (fun.name() == "=")
			{
				assert(length(ptr->cdr()) == 2);
				identifier_t id = boost::get<identifier_t>(ptr->cdr()->car());
				value_t value = ptr->cdr()->cdr()->car();
				return append(_compiler.compile(value), cons(ASSIGNMENT, cons(_compiler.find_identifier(id), nil)));
			}
			else
			{
                cons_ptr_t ret;
			    cons_ptr_t args = as_list(ptr->cdr());
			    while (args)
			    {
				    ret = append(_compiler.compile(args->car()), ret);
				    args = args->cdr();
			    }

				return cons(FUN_LOAD_ARG, append(ret, cons(LOAD_VAR, cons(_compiler.find_identifier(fun), cons(FUN_CALL, nil)))));
			}
		}
		else
		if (is<lambda>(ptr->car()))
		{
			cons_ptr_t lambda = ptr->cdr();
			assert(length(lambda) == 2);

			cons_ptr_t parameters = as_list(lambda->car());
			value_t	   body		  = lambda->cdr()->car();

			while (parameters)
			{
				assert(is<identifier_t>(parameters->car()));
				_compiler._as.push_front(boost::get<identifier_t>(parameters->car()));
				parameters = parameters->cdr();
			}
			
			cons_ptr_t ret = cons(LAMBDA, 
                
                cons(append(_compiler.compile(body), cons(FUN_RET, nil)), nil)); 

			_compiler._as.clear();

			return ret;
		}
		else
		if (is<cons_ptr_t>(ptr->car()))
		{
			cons_ptr_t ret;
			cons_ptr_t args = as_list(ptr->cdr());
			while (args)
			{
				ret = append(_compiler.compile(args->car()), ret);
				args = args->cdr();
			}

			return cons(FUN_LOAD_ARG, append(append(ret, _compiler.compile(ptr->car())), cons(FUN_CALL, nil)));
		}
		else
		{
			throw runtime_error_t(boost::format("beginning of sexp should be a lambda expression or an identifier that names a function"));
		}
    }
};


void compiler_t::push_context()
{
	_vr.push_front(context());
}

void compiler_t::pop_context()
{
	_vr.pop_front();
}

double compiler_t::find_identifier(const identifier_t& id)
{
	int location = 0;

	for (var_registry::const_iterator iter = _vr.begin(), end = _vr.end(); iter != end; ++iter)
	{
		context::const_iterator jter = std::find(iter->begin(), iter->end(), id);
		if (jter != iter->end())
		{
			return location + std::distance(iter->begin(), jter);
		}

		location += static_cast<int>(iter->size());
	}

	assert(!_vr.empty());
	_vr[_vr.size() - 1].push_back(id);
	return location;
}

cons_ptr_t compiler_t::compile(const value_t& v)
{
	return boost::apply_visitor(compiler_visitor_t(*this), v);
}

}
