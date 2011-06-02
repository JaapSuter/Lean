// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/cons.hpp"

using namespace lean;

BOOST_AUTO_UNIT_TEST(test_cons_equal)
{
    BOOST_CHECK_EQUAL(cons(0., nil), cons(0., nil));
    BOOST_CHECK_EQUAL(cons(0., cons(1., nil)), cons(0., cons(1., nil)));
    BOOST_CHECK(cons(0., nil) == cons(0., nil));
    BOOST_CHECK(cons(0., nil) != cons(1., nil));
    BOOST_CHECK(cons(true, nil) == cons(true, nil));
    
    BOOST_CHECK(cons(true, cons(4., nil)) != cons(true, nil));
    BOOST_CHECK(cons(true, nil) != cons(true, cons(4., nil)));

    BOOST_CHECK(cons(cons(identifier_t("foobar"), nil), nil) == cons(cons(identifier_t("foobar"), nil), nil));

    BOOST_CHECK(cons(cons(0., nil), nil) == cons(cons(0., nil), nil));
    
    BOOST_CHECK_EQUAL(cons(true, cons(3., cons("foobar", nil))),
                      cons(true, cons(3., cons("foobar", nil))));
}
