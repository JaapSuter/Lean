// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"

using namespace lean;

namespace
{
	bool _b = false;
	double _d = 0;
	
	void nullary() { _b = true; }
	double nullary_wr() { return 4.; }

	void unary_d(double i) { _d = i; }
	double unary_d_wr(double i) { return i; }

	std::string stringary(const std::string& s) { return s + s; }

	double lotsary(bool b, double d) { if (b) return d; else return 0.; }

	boost::function<void (void)> funned_nullary = nullary;
}

BOOST_AUTO_UNIT_TEST(test_foreign_function)
{
    environment_t env;

	env.def("nullary", &nullary);
	env.def("nullary_wr", &nullary_wr);
	env.def("lotsary", &lotsary);
	env.def("unary_d", &unary_d);
	env.def("unary_d_wr", &unary_d_wr);
	env.def("stringary", &stringary);

	env.def("funned_nullary", funned_nullary);	
	
	_b = false;
	eval(parse("(nullary)"), env);
    BOOST_CHECK_EQUAL(_b, true);

	_b = false;
	eval(parse("(funned_nullary)"), env);
    BOOST_CHECK_EQUAL(_b, true);

    BOOST_CHECK_EQUAL(eval(parse("(nullary_wr)"), env), value_t(4.));

	_d = 0;
	eval(parse("(unary_d 4)"), env);
    BOOST_CHECK_EQUAL(_d, 4);

    BOOST_CHECK_EQUAL(eval(parse("(unary_d_wr 4)"), env), value_t(4.));

	BOOST_CHECK_EQUAL(eval(parse("(stringary \"foo\")"), env), value_t(std::string("foofoo")));

	BOOST_CHECK_EQUAL(eval(parse("(lotsary true 3.0)"), env), value_t(3.0));
	BOOST_CHECK_EQUAL(eval(parse("(lotsary false 3.0)"), env), value_t(0.0));
}
