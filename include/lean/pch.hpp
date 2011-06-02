// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_PCH_HPP_INCLUDED
#define FILE_LEAN_PCH_HPP_INCLUDED

#include <iostream>
#include <iosfwd>
#include <stack>
#include <exception>
#include <map>
#include <string>
#include <cstring>
#include <cassert>

#ifdef _MSC_VER
    #pragma warning(push, 0)
    #pragma warning(disable:4267)
    #pragma warning(disable:4511)
    #pragma warning(disable:4701)
    #pragma warning(disable:4244)
	#pragma warning(disable:4503)
#endif

#include <boost/variant.hpp>
#include <boost/spirit/core.hpp>
#include <boost/format.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function_types/is_function_type.hpp> 
#include <boost/function_types/function_type_arity.hpp>
#include <boost/function_types/function_type_parameter.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#ifdef _MSC_VER
    #pragma warning(pop)
#endif


#endif

