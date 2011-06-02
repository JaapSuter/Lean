// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_ENVIRONMENT_HPP_INCLUDED
#define FILE_LEAN_ENVIRONMENT_HPP_INCLUDED

#include "lean/value.hpp"
#include "lean/cons_ptr.hpp"
#include "lean/cons.hpp"
#include "lean/foreign_fun.hpp"
#include <cassert>
#include <map>
#include <deque>
#include <boost/function.hpp>
#include <iosfwd>

namespace lean {
   
class environment_t : boost::noncopyable
{

public:

    environment_t();

    template<class Fun>
    void def(const char* name, const Fun& fun)
    {
        _foreign_funs[name] = boost::shared_ptr<foreign_fun_base>(new foreign_fun<Fun>(fun, *this));
    }
	
	value_t call(const value_t& fun, const cons_ptr_t& arg);
	value_t get(const identifier_t& id) const;
	void	set(const identifier_t& id, const value_t& v, int level);

	void display_fun_calls(std::ostream* ostr = 0);
	
private:

	friend struct invokation;

	typedef std::map<std::string, boost::shared_ptr<foreign_fun_base> > foreign_fun_map;
	typedef std::map<std::string, value_t> context;
	typedef std::deque<context> context_stack;

    foreign_fun_map _foreign_funs;	
	context_stack	_context_stack;

	std::ostream*   _display_fun_calls;
};

}

#endif
