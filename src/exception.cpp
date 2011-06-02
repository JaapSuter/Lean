// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/exception.hpp"
#include <boost/format.hpp>

namespace lean {

parse_error_t::parse_error_t(const std::string& at)
    : _what((boost::format("::lean::parse_error_t at %1%") % at).str())
{
}

const char* parse_error_t::what() const
{
    return _what.c_str();
}

runtime_error_t::runtime_error_t(const boost::format& what)
	: _what((boost::format("::lean::runtime_error_t: \"%1%\"") % what.str()).str())
{
}

const char* runtime_error_t::what() const
{
    return _what.c_str();
}

}
