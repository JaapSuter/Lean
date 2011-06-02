// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_INSTRUCTION_HPP_INCLUDED
#define FILE_LEAN_INSTRUCTION_HPP_INCLUDED

#include "lean/cons_ptr.hpp"

namespace lean {

enum instruction
{
	LOAD_CONST,
	LOAD_VAR,
	LOAD_ARG,
	
	QUOTE,

	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	
	AND,
	OR,
	
	EQ,
	NEQ,
	LEQ,
	GEQ,
	LE,
	GE,

	IF,
	ASSIGNMENT,
    FUN_LOAD_ARG,
	FUN_CALL,
	FUN_RET,

    LAMBDA,

	SKIP,
};

inline std::ostream& operator << (std::ostream& ostr, instruction i)
{
	#define LEAN_output_instruction(_x_) if (i == _x_) return ostr << #_x_;
		LEAN_output_instruction(LOAD_CONST)
		LEAN_output_instruction(LOAD_VAR)
		LEAN_output_instruction(LOAD_ARG)
		LEAN_output_instruction(LAMBDA)
		LEAN_output_instruction(FUN_CALL)
		LEAN_output_instruction(FUN_RET)
        LEAN_output_instruction(FUN_LOAD_ARG)
        LEAN_output_instruction(ADD)
		LEAN_output_instruction(SUB)
		LEAN_output_instruction(MUL)
		LEAN_output_instruction(DIV)
		LEAN_output_instruction(MOD)
		LEAN_output_instruction(AND)
		LEAN_output_instruction(OR)
		LEAN_output_instruction(EQ)
		LEAN_output_instruction(NEQ)
		LEAN_output_instruction(LEQ)
		LEAN_output_instruction(GEQ)
		LEAN_output_instruction(LE)
		LEAN_output_instruction(GE)
		LEAN_output_instruction(IF)
		LEAN_output_instruction(ASSIGNMENT)
		LEAN_output_instruction(SKIP)
		LEAN_output_instruction(QUOTE)
		
	#undef LEAN_output_instruction
		assert(false);
	return ostr;
}

}

#endif
