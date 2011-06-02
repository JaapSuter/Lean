// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/eval.hpp"
#include "lean/environment.hpp"
#include "lean/read.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_simple_programs)
{
    environment_t env;

	#define LEAN_test(_name_) BOOST_CHECK_EQUAL(eval(parse(read(#_name_).c_str()), env), value_t(true));

	LEAN_test(../../test/test_trivial_program.sexp)
	LEAN_test(../../test/test_boolean_logic.sexp)
	LEAN_test(../../test/test_reverse_list.sexp)
	LEAN_test(../../test/test_map.sexp)
	LEAN_test(../../test/test_quick_sort.sexp)
	
	#undef LEAN_test
    
}
