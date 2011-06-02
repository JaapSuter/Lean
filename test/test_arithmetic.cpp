// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_arithmetic)
{
    environment_t env;
    
    BOOST_CHECK_EQUAL(eval(parse("(+ 0 1)"), env), value_t(1.));
    BOOST_CHECK_EQUAL(eval(parse("(+ 0 6)"), env), value_t(6.));
    BOOST_CHECK_EQUAL(eval(parse("(+ -123 123)"), env), value_t(0.));
    BOOST_CHECK_EQUAL(eval(parse("(+ -10 -10)"), env), value_t(-20.));
    
    BOOST_CHECK_EQUAL(eval(parse("(- 1 1)"), env), value_t(0.));
    BOOST_CHECK_EQUAL(eval(parse("(- 0 1)"), env), value_t(-1.));
    BOOST_CHECK_EQUAL(eval(parse("(- 0 -1)"), env), value_t(1.));
    BOOST_CHECK_EQUAL(eval(parse("(- 100 50)"), env), value_t(50.));

    BOOST_CHECK_EQUAL(eval(parse("(+ (- 100 50) (+ 300 200))"), env), value_t(550.));
    BOOST_CHECK_EQUAL(eval(parse("(- (- 100 50) (+ 300 200))"), env), value_t(-450.));

	BOOST_CHECK_EQUAL(eval(parse("(* (+ 2 2) (/ 9 3))"), env), value_t(12.));

	BOOST_CHECK_EQUAL(eval(parse("(% 12 5)"), env), value_t(2.));
}
