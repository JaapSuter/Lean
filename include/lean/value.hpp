// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_VALUE_HPP_INCLUDED
#define FILE_LEAN_VALUE_HPP_INCLUDED

#include "lean/cons_ptr.hpp"
#include "lean/empty.hpp"
#include "lean/lambda.hpp"
#include "lean/identifier.hpp"
#include "lean/instruction.hpp"
#include <string>
#include <iosfwd>
#include <boost/variant.hpp>

namespace lean {

typedef boost::variant
<
    empty,
	lambda,
	bool,
    double,
    std::string,
    identifier_t,
    cons_ptr_t,
	instruction

> value_t;

bool operator == (const value_t& v, const cons_ptr_t& ptr);
bool operator != (const value_t& v, const cons_ptr_t& ptr);
bool operator == (const cons_ptr_t& ptr, const value_t& v);
bool operator != (const cons_ptr_t& ptr, const value_t& v);

std::ostream& operator << (std::ostream&, const value_t&);

const value_t& nth(const cons_ptr_t& ptr, int n);

const cons_ptr_t& as_list(const value_t&);

namespace detail {
	template<class T>
	struct is_type_t : boost::static_visitor<bool>
	{
		template<class U>
		bool operator () (const U&) const { return false; }

		bool operator () (const T&) const { return true; }
	};
}

template <class T>
bool is(const value_t& t)
{
	return boost::apply_visitor(detail::is_type_t<T>(), t);
}

inline bool is_list(const value_t& v) { return is<cons_ptr_t>(v); }
inline bool is_empty(const value_t& v) { return is<empty>(v); }
inline bool is_lambda(const value_t& v) { return is<lambda>(v); }

}

#endif
