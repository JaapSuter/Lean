// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_EXCEPTION_HPP_INCLUDED
#define FILE_LEAN_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <string>

namespace lean {

struct parse_error_t : public std::exception
{
    parse_error_t(const std::string& at);
    virtual const char* what() const;

private:

    std::string _what;
};

struct runtime_error_t : public std::exception
{
	runtime_error_t(const boost::format& what);
    virtual const char* what() const;

private:

    std::string _what;
};


}

#endif
