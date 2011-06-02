// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_cond)
{
    environment_t env;

    BOOST_CHECK_EQUAL(eval(parse("(if true  3 4)"), env), value_t(3.));
	BOOST_CHECK_EQUAL(eval(parse("(if false 3 4)"), env), value_t(4.));
	
	BOOST_CHECK_EQUAL(eval(parse("(if (== 2 2) 3 4)"), env), value_t(3.));
	BOOST_CHECK_EQUAL(eval(parse("(if (== 3 4) 3 4)"), env), value_t(4.));

	BOOST_CHECK_EQUAL(eval(parse("(if (!= 2 2) 3 4)"), env), value_t(4.));
	BOOST_CHECK_EQUAL(eval(parse("(if (== 3 4) 3 4)"), env), value_t(4.));    

	BOOST_CHECK_EQUAL(eval(parse("(if (<= 2 2) 3)"), env), value_t(3.));
	BOOST_CHECK_EQUAL(eval(parse("(if (>  2 2) 3)"), env), nil);
}
