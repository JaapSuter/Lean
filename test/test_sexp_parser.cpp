// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/cons.hpp"
#include "lean/exception.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_sexp_parser)
{
    BOOST_CHECK_EQUAL(parse("(()())"), cons(nil, cons(nil, nil)));

    BOOST_CHECK_EQUAL(parse("true"), value_t(true));
    BOOST_CHECK_EQUAL(parse("3"), value_t(3.));
    BOOST_CHECK_EQUAL(parse("3.14"), value_t(3.14));
    BOOST_CHECK_EQUAL(parse("()"), value_t(nil));

    BOOST_CHECK_EQUAL(parse("(-432)"), cons(-432., nil));
    
    BOOST_CHECK_EQUAL(parse("(true)"), cons(true, nil));    
    BOOST_CHECK_EQUAL(parse("(true false true)"), cons(true, cons(false, cons(true, nil))));

    BOOST_CHECK_EQUAL(parse("(-432)"), cons(-432., nil));
    BOOST_CHECK_EQUAL(parse("(-1)"), cons(-1., nil));
    BOOST_CHECK_EQUAL(parse("(0)"), cons(0., nil));
    BOOST_CHECK_EQUAL(parse("(1)"), cons(1., nil));
    BOOST_CHECK_EQUAL(parse("(765)"), cons(765., nil));

    BOOST_CHECK_EQUAL(parse("(1 2 3 4)"), cons(1., cons(2., cons(3., cons(4., nil)))));

    BOOST_CHECK_EQUAL(parse("foo"), value_t(identifier_t("foo")));
    BOOST_CHECK_EQUAL(parse("f12"), value_t(identifier_t("f12")));
    BOOST_CHECK_EQUAL(parse("_f12-_foobar-blah/mwah"), value_t(identifier_t("_f12-_foobar-blah/mwah")));
    BOOST_CHECK_EQUAL(parse("(foo bar bar foo)"), cons(identifier_t("foo"), cons(identifier_t("bar"), cons(identifier_t("bar"), cons(identifier_t("foo"), nil)))));
    
    BOOST_CHECK_EQUAL(parse("(-432.14)"), cons(-432.14, nil));
    BOOST_CHECK_EQUAL(parse("(-1.23)"), cons(-1.23, nil));
    BOOST_CHECK_EQUAL(parse("(0.19)"), cons(0.19, nil));
    BOOST_CHECK_EQUAL(parse("(0.0)"), cons(0.0f, nil));
    BOOST_CHECK_EQUAL(parse("(1.234)"), cons(1.234, nil));
    BOOST_CHECK_EQUAL(parse("(765.18)"), cons(765.18, nil));

    BOOST_CHECK_EQUAL(parse("(\"foobar\")"), cons("foobar", nil));
    BOOST_CHECK_EQUAL(parse("(\"\")"), cons("", nil));
    BOOST_CHECK_EQUAL(parse("(\"\t\r\n \")"), cons("\t\r\n ", nil));

    BOOST_CHECK_EQUAL(parse("(())"), cons(nil, nil));

    BOOST_CHECK_EQUAL(parse("(true 3 3.14 \"foobar\" ())"),
        cons(true, cons(3., cons(3.14, cons("foobar", cons(nil, nil))))));

    #define LEAN_TEST_FOR_PARSE_ERROR(_expr_)   \
    {                                           \
        bool exception_expected = false;        \
        try { parse(_expr_); }                  \
        catch (const parse_error_t&) { exception_expected = true; } \
        BOOST_CHECK_EQUAL(exception_expected, true); \
    }

    LEAN_TEST_FOR_PARSE_ERROR("1identifier_cannot_start_with_number")
    LEAN_TEST_FOR_PARSE_ERROR("1 ()")
    LEAN_TEST_FOR_PARSE_ERROR("()()")
    LEAN_TEST_FOR_PARSE_ERROR("() true")
    LEAN_TEST_FOR_PARSE_ERROR("(((")
    LEAN_TEST_FOR_PARSE_ERROR(")))")
    LEAN_TEST_FOR_PARSE_ERROR("(())())")

    #undef LEAN_TEST_FOR_PARSE_ERROR
}
