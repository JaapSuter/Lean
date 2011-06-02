// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/virtual_machine.hpp"
#include "lean/value.hpp"
#include "lean/cons.hpp"
#include <list>

namespace lean {

namespace {

	cons_ptr_t skip(cons_ptr_t ip, double num_instructions)
	{
		while (num_instructions-- && ip)
			ip = ip->cdr();
		
		return ip;
	}

    void print(const std::deque<value_t>& st)
    {
        std::cout << "\nstack\n";
        for (int i = static_cast<int>(st.size()) - 1; i >= 0; --i)
        {
            std::cout << '\t' << st[i] << '\n';
        }
    }
}

virtual_machine_t::virtual_machine_t()
{
	const int memory_size = 1024;
	_memory.resize(memory_size);
}

value_t virtual_machine_t::execute(const cons_ptr_t& ip_)
{
	cons_ptr_t ip = ip_;
    while (ip)
	{
        std::cout << '\n' << ip;

		instruction inst = boost::get<instruction>(ip->car());
		if (inst == LOAD_CONST)
		{
			_stack.push_back(ip->cdr()->car());
            ip = ip->cdr()->cdr();			
		}
		else
		
		#define LEAN_binary_function_call(_name_, _operand_, _type_) \
			if (inst == _name_)										 \
			{														 \
				value_t a = _stack.back();							 \
				_stack.pop_back();									 \
				value_t b = _stack.back();							 \
				_stack.pop_back();									 \
				_stack.push_back(boost::get<_type_>(b) _operand_ boost::get<_type_>(a));	\
                ip = ip->cdr();                                      \
			} else

		LEAN_binary_function_call(ADD, +, double)
		LEAN_binary_function_call(SUB, -, double)
		LEAN_binary_function_call(MUL, *, double)
		LEAN_binary_function_call(DIV, /, double)
		LEAN_binary_function_call(AND, &&, bool)
		LEAN_binary_function_call(OR, ||, bool)
		LEAN_binary_function_call(EQ, ==, double)
		LEAN_binary_function_call(NEQ, !=, double)
		LEAN_binary_function_call(LEQ, <=, double)
		LEAN_binary_function_call(GEQ, >=, double)
		LEAN_binary_function_call(LE, <, double)
		LEAN_binary_function_call(GE, >, double)
		#undef LEAN_binary_function_call
		
		if (inst == QUOTE)
		{
			_stack.push_back(ip->cdr()->car());
			ip = ip->cdr()->cdr();
		}
		else
		if (inst == IF)
		{
			_if_stack.push(true);
            ip = ip->cdr();
		}
		else
		if (inst == SKIP)
		{
			if (_if_stack.empty())
			{
				ip = skip(ip, boost::get<double>(ip->cdr()->car()));
			}
			else
			{
				_if_stack.pop();
				if (boost::get<bool>(_stack.back()))
				{
                    ip = ip->cdr();
				}
				else
				{
					ip = skip(ip, boost::get<double>(ip->cdr()->car()));
				}

				_stack.pop_back();
			}

            ip = ip->cdr();
		}
		else
		if (inst == ASSIGNMENT)
		{
			assert(!_stack.empty());
			_memory[static_cast<int>(boost::get<double>(ip->cdr()->car()))] = _stack.back();
			ip = ip->cdr()->cdr();
		}
		else
		if (inst == LOAD_VAR)
		{
			_stack.push_back(_memory[static_cast<int>(boost::get<double>(ip->cdr()->car()))]);
			ip = ip->cdr()->cdr();
		}
		else
		if (inst == FUN_CALL)
		{
            ip = append(as_list(_stack.back()), ip->cdr());
            _stack.pop_back();           
		}
		else
        if (inst == LAMBDA)
		{
			_stack.push_back(ip->cdr()->car());
            ip = ip->cdr()->cdr();
		}
        else
		if (inst == FUN_RET)
		{
            value_t fun_ret = _stack.back();
            _stack.resize(_frame_ptr.top());
            _stack.push_back(fun_ret);
            _frame_ptr.pop();            
            ip = ip->cdr();
		}
		else
        if (inst == FUN_LOAD_ARG)
        {
            _frame_ptr.push(static_cast<int>(_stack.size()));
            ip = ip->cdr();
        }
        else
		if (inst == LOAD_ARG)
		{
            const int idx = _frame_ptr.top() - 1 - static_cast<int>(boost::get<double>(ip->cdr()->car()));
            assert(idx >= 0 && idx < static_cast<int>(_stack.size()));
            _stack.push_back(_stack[idx]);
            ip = ip->cdr()->cdr();
		}
        else
        {
            assert(false);
        }

        print(_stack);
	}

	if (_stack.empty())
		return nil;
	else
    	return _stack.back();
}

}
