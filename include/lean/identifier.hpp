// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_IDENTIFIER_HPP_INCLUDED
#define FILE_LEAN_IDENTIFIER_HPP_INCLUDED

#include <string>
#include <iosfwd>

namespace lean {

struct identifier_t
{
    explicit identifier_t(const std::string& name) : _name(name) {}

    const std::string& name() const { return _name; }

private:
    std::string _name;
};


bool operator == (const identifier_t& lhs, const identifier_t& rhs);
bool operator != (const identifier_t& lhs, const identifier_t& rhs);

std::ostream& operator << (std::ostream&, const identifier_t&);

}

#endif
