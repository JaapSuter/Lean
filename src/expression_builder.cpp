// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/expression_builder.hpp"
#include "lean/cons.hpp"
#include <cassert>

namespace lean {

namespace
{
    struct get_ptr_to_cons_ptr : boost::static_visitor<cons_ptr_t*>
    {
        template<class T>
        cons_ptr_t* operator () (T&) const
        {
            assert(false && "unreachable code, invariant violation in expression builder");
            return 0;
        }

        cons_ptr_t* operator () (cons_ptr_t& t) const
        {
            return &t;
        }
    };
}

value_t expression_builder_t::get() const
{
    return _expr;
}

void expression_builder_t::open_paren()
{
    if (_eol.empty())
    {
        assert(is_empty(_expr));

        _expr = cons_ptr_t();
        _eol.push(boost::apply_visitor(get_ptr_to_cons_ptr(), _expr));
    }
    else
    {
        cons_ptr_t& ptr = *_eol.top();
        assert(!ptr);
        ptr = cons(cons_ptr_t(), nil);
        _eol.top() = &ptr->cdr();
        _eol.push(boost::apply_visitor(get_ptr_to_cons_ptr(), ptr->car()));
    }
}

void expression_builder_t::add(const value_t& t)
{
    if (_eol.empty())
    {
        assert(is_empty(_expr));
        _expr = t;
    }
    else
    {
        cons_ptr_t& ptr = *_eol.top();
        assert(!ptr);
        ptr = cons(t, nil);
        _eol.top() = &ptr->cdr();
    }
}

void expression_builder_t::close_paren()
{
    assert(!_eol.empty());
    _eol.pop();       
}

}
