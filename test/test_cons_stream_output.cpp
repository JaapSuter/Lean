// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/cons.hpp"
#include <sstream>

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_cons_stream_output)
{
    #define LEAN_TEST_STREAM_OUTPUT(_cons_expr_, _str_repr_)\
    {                                                       \
        std::stringstream strstr;                           \
        strstr << _cons_expr_;                              \
        BOOST_CHECK_EQUAL(strstr.str(), _str_repr_);        \
    }

    LEAN_TEST_STREAM_OUTPUT(value_t(true), "true")
    LEAN_TEST_STREAM_OUTPUT(value_t(false), "false")
    LEAN_TEST_STREAM_OUTPUT(value_t(16.), "16")
    LEAN_TEST_STREAM_OUTPUT(value_t(16.3), "16.3")
    LEAN_TEST_STREAM_OUTPUT(value_t(std::string("foobar")), "\"foobar\"")
    LEAN_TEST_STREAM_OUTPUT(value_t(identifier_t("hello")), "hello")

    LEAN_TEST_STREAM_OUTPUT(cons(true,  nil), "(true)")
    LEAN_TEST_STREAM_OUTPUT(cons(3.,     nil), "(3)")
    LEAN_TEST_STREAM_OUTPUT(cons(3.14, nil), "(3.14)")
    LEAN_TEST_STREAM_OUTPUT(cons(identifier_t("hello"), nil), "(hello)")
    LEAN_TEST_STREAM_OUTPUT(cons("fo", nil), "(\"fo\")")
    LEAN_TEST_STREAM_OUTPUT(cons("fo", cons(3., nil)), "(\"fo\" 3)")
    LEAN_TEST_STREAM_OUTPUT(cons(cons("foobar", cons(3., nil)), cons(3., nil)), "((\"foobar\" 3) 3)")

    
    LEAN_TEST_STREAM_OUTPUT(cons(true, cons(3., cons(4.14, cons("foobar", cons(identifier_t("foobar"), nil))))),
                           "(true 3 4.14 \"foobar\" foobar)")

    #undef LEAN_TEST_STREAM_OUTPUT
}
