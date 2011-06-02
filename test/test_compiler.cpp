// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/parse.hpp"
#include "lean/virtual_machine.hpp"
#include "lean/compiler.hpp"
#include "lean/cons.hpp"
 
using namespace lean;

BOOST_AUTO_UNIT_TEST(test_compiler)
{
	virtual_machine_t vm;
	compiler_t cm;
	
    /*
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(quote ())"))), nil);
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(quote (1 2 3))"))), cons(1., cons(2., cons(3., nil))));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(* -3 (- (* 1 2) (+ 3 4)))"))), value_t(15.));	
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(== 3 4)"))), value_t(false));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(!= 3 4)"))), value_t(true));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(&& (== 4 4) (== (+ 3 1) (+ 1 3)))"))), value_t(true));
	
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if true  false true)"))), value_t(false));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if false false true)"))), value_t(true));
	
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if true   3.)"))), value_t(3.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if false  3.)"))), nil);
	
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if (== (+ 3 4) (- 9 2)) (+ 5 6) (- 4 2))"))), value_t(11.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(if (if (if false false true) false true) false true)"))), value_t(true));

	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(= a 3)"))), value_t(3.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(program (= a 4) a)"))), value_t(4.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(program (= a (+ 3 4)) (= b (- 3 1)) (* a b))"))), value_t(14.));

    BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(- 3 4)"))), value_t(-1.));
    */
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("((lambda (x) x) 4)"))), value_t(4.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("((lambda (x y) (+ x y)) 4 3)"))), value_t(7.));
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("((lambda (x y) (- x y)) 4 3)"))), value_t(1.));
    //*/
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("((lambda (x) ((lambda (x) x) x)) 4)"))), value_t(4.));
	
    /*
	BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(program (= add (lambda (x y) (+ x y))) (add 3 4))"))), value_t(7.));
    BOOST_CHECK_EQUAL(vm.execute(cm.compile(parse("(program (= add (lambda (x y) (+ x y))) (= sub (lambda (x y) (- x y))) (sub 9 (add 3 4)))"))), value_t(2.));
    */
}
