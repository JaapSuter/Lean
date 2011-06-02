// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/cons_ptr.hpp"
#include "lean/cons.hpp"
#include <cassert>

namespace lean {
    
cons_ptr_t nil;

const char* nil_repr()
{
    return "nil";
}

void intrusive_ptr_add_ref(cons_t* ptr)
{
    assert(ptr);
    ptr->add_ref();
}

void intrusive_ptr_release(cons_t* ptr)
{
    assert(ptr);
    ptr->release();
}

bool operator == (const cons_ptr_t& lhs, const cons_ptr_t& rhs)
{
    return (!lhs && !rhs)
         || (lhs && rhs && *lhs == *rhs);
}

bool operator != (const cons_ptr_t& lhs, const cons_ptr_t& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator << (std::ostream& ostr, const cons_ptr_t& ptr)
{
    ostr << '(';
    
    if (ptr)
        ostr << *ptr;
    ostr << ')';    
    
    return ostr;
}

int length(const cons_ptr_t& ptr)
{
	int len = 0;
	cons_ptr_t p = ptr;
	while (p)
	{
		++len;
		p = p->cdr();
	}
	return len;
}

}