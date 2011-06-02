// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_trivial_eval)
{
    environment_t env;

    
    BOOST_CHECK_EQUAL(eval(parse("true"), env), value_t(true));
    BOOST_CHECK_EQUAL(eval(parse("false"), env), value_t(false));
    
    BOOST_CHECK_EQUAL(eval(parse("3.14"), env), value_t(3.14));
	BOOST_CHECK_EQUAL(eval(parse("-3.14"), env), value_t(-3.14));
    
    BOOST_CHECK_EQUAL(eval(parse("\"foobar\""), env), value_t(std::string("foobar")));

    BOOST_CHECK_EQUAL(eval(parse("(nop)"), env), nil);
}
