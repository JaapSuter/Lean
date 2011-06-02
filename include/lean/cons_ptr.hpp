// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_CONS_PTR_HPP
#define FILE_LEAN_CONS_PTR_HPP

#include <boost/intrusive_ptr.hpp>
#include <iosfwd>

namespace lean {

    struct cons_t;
	
    typedef boost::intrusive_ptr<cons_t> cons_ptr_t;

    void intrusive_ptr_add_ref(cons_t* p);
    void intrusive_ptr_release(cons_t* p);

    bool operator == (const cons_ptr_t& lhs, const cons_ptr_t& rhs);
    bool operator != (const cons_ptr_t& lhs, const cons_ptr_t& rhs);

    extern cons_ptr_t nil;
    const char* nil_repr();

    std::ostream& operator << (std::ostream& ostr, const cons_ptr_t& ptr);

	int length(const cons_ptr_t& ptr);
}

#endif
