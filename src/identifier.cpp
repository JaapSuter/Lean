// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/identifier.hpp"

namespace lean {

bool operator == (const identifier_t& lhs, const identifier_t& rhs) { return lhs.name() == rhs.name(); }
bool operator != (const identifier_t& lhs, const identifier_t& rhs) { return lhs.name() != rhs.name(); }

std::ostream& operator << (std::ostream& ostr, const identifier_t& i)
{
    ostr << i.name();
    return ostr;
}

}
