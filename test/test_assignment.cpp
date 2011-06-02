// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_assignment)
{
    environment_t env;
    
	BOOST_CHECK_EQUAL(eval(parse("(program (= a 1))"), env), value_t(1.));
    BOOST_CHECK_EQUAL(eval(parse("(program (= a 1) a)"), env), value_t(1.));
	BOOST_CHECK_EQUAL(eval(parse("(program (= a 1) a)"), env), value_t(1.));
	BOOST_CHECK_EQUAL(eval(parse("(program (= a 1) (= b 3) (+ a b))"), env), value_t(4.));

	BOOST_CHECK_EQUAL(eval(parse("(program (= add (lambda (x y) (+ x y))) (add 3 4))"), env), value_t(7.));
}
