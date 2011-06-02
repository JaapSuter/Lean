// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/eval.hpp"
#include "lean/cons.hpp"
#include "lean/environment.hpp"
#include <cassert>

namespace lean {

namespace {

    struct evaluator_t : boost::static_visitor<value_t>, boost::noncopyable
    {
        environment_t& _env;
        evaluator_t(environment_t& env) : _env(env) {}

        template<class T>
        value_t operator () (const T& t) const
        {
            return t;
        }

		value_t operator () (const identifier_t& identifier) const
        {
			return _env.get(identifier);
        }

        value_t operator () (const cons_ptr_t& ptr) const
        {
            assert(ptr);
			if (ptr && is_lambda(ptr->car()))
				return ptr;
			else
				return _env.call(ptr->car(), ptr->cdr());
        }
    };
}

value_t eval(const value_t& t, environment_t& env)
{
	return boost::apply_visitor(evaluator_t(env), t);
}

}
