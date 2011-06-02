// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_list_manip)
{
    environment_t env;

	BOOST_CHECK_EQUAL(eval(parse("(quote (3 4))"), env), cons(3., cons(4., nil)));
    BOOST_CHECK_EQUAL(eval(parse("(car (quote(3 4)))"), env), value_t(3.));
	BOOST_CHECK_EQUAL(eval(parse("(cdr (quote(3 4)))"), env), cons(4., nil));
	BOOST_CHECK_EQUAL(eval(parse("(cons 3 (quote ()))"), env), cons(3., nil));
}
