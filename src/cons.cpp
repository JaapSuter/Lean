// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/cons.hpp"
#include <cassert>

namespace lean {

cons_ptr_t append(const cons_ptr_t& a, const cons_ptr_t& b)
{
	cons_ptr_t p = a;
	if (!a) return b;
	while (p->cdr())
		p = p->cdr();
	p->cdr() = b;	
	return a;
}
    
cons_t::~cons_t()
{ 
    assert(_ref_count == 1); 
}

bool operator == (const cons_t& lhs, const cons_t& rhs)
{
    return lhs.car() == rhs.car() && lhs.cdr() == rhs.cdr();
}

bool operator != (const cons_t& lhs, const cons_t& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator << (std::ostream& ostr, const cons_t& c)
{
    ostr << c.car();
    if (c.cdr())
    {
        ostr << ' ';    
        ostr << *c.cdr();
    }

    return ostr;
}

}
