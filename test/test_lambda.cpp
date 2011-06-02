// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_lambda)
{
    environment_t env;

    BOOST_CHECK_EQUAL(eval(parse("((lambda (x) x) 4)"), env), value_t(4.));
	BOOST_CHECK_EQUAL(eval(parse("((lambda (x) (+ x x)) 4)"), env), value_t(8.));
	BOOST_CHECK_EQUAL(eval(parse("((lambda (x) (* 2 x)) 4)"), env), value_t(8.));
	BOOST_CHECK_EQUAL(eval(parse("((lambda (x y) (/ x y)) 12 4)"), env), value_t(3.));

	BOOST_CHECK_EQUAL(eval(parse(
		"("
			"(lambda (m n o) (* m (* n o)))"
			
			"((lambda (x y) (/ x y)) 12 4)"
			"((lambda (a b) (+ a b)) 20 2)"
			"3"
		")"
		), env), value_t(198.));
}

