// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/value.hpp"
#include "lean/cons.hpp"

namespace lean {
    
bool operator == (const value_t& v, const cons_ptr_t& ptr) { return v == value_t(ptr); }
bool operator != (const value_t& v, const cons_ptr_t& ptr) { return !(v == ptr); }
bool operator == (const cons_ptr_t& ptr, const value_t& v) { return v == value_t(ptr); }
bool operator != (const cons_ptr_t& ptr, const value_t& v) { return !(v == ptr); }

namespace {

    struct stream_output_visitor : boost::static_visitor<>, boost::noncopyable
    {
        explicit stream_output_visitor(std::ostream& ostr) : _ostr(ostr) {}

        template<class T>
        void operator () (const T& t) const
        {
            _ostr << t;
        }

        void operator () (empty) const
        {
            assert(false && "unreachable code, not a well formed s-expression");
            _ostr << "empty";
        }

		void operator () (lambda) const
        {
            _ostr << "lambda";
        }

        void operator () (const std::string& s) const
        {
            _ostr << '"' << s << '"';
        }

        void operator () (bool b) const
        {
            _ostr << (b ? "true" : "false");
        }

    private:
        std::ostream& _ostr;
    };
}

std::ostream& operator << (std::ostream& ostr, const value_t& v)
{
    stream_output_visitor ov(ostr);
    boost::apply_visitor(ov, v);    
    return ostr;
}

const value_t& nth(const cons_ptr_t& ptr, int n)
{
	cons_ptr_t p = ptr;
	while (p)
	{
		if (n == 0)
			return p->car();
        
		p = p->cdr();
		--n;		
	}
	
	throw std::exception("not enough cells in list to get nth value");
}

namespace {
	struct as_list_visitor : boost::static_visitor<const cons_ptr_t*>
	{
		template<class T>
		const cons_ptr_t* operator () (const T&) const { return 0; }

		const cons_ptr_t* operator () (const cons_ptr_t& t) const { return &t; }
	};
}

const cons_ptr_t& as_list(const value_t& v)
{
	if (!is_list(v))
	{	
		assert(is_list(v));
	}
	return *boost::apply_visitor(as_list_visitor(), v);
}

}
