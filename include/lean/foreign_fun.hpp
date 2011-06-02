// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_FOREIGN_FUN_HPP_INCLUDED
#define FILE_LEAN_FOREIGN_FUN_HPP_INCLUDED

#include "lean/value.hpp"
#include "lean/cons_ptr.hpp"
#include "lean/cons.hpp"
#include "lean/eval.hpp"
#include <boost/noncopyable.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/function_types/is_function_type.hpp>
#include <boost/function_types/function_type_arity.hpp>
#include <boost/function_types/function_type_result.hpp>
#include <boost/function_types/function_type_parameter.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#ifndef LEAN_MAX_NUM_FOREIGN_FUN_PARAMS
	#define LEAN_MAX_NUM_FOREIGN_FUN_PARAMS 8
#endif
            
namespace lean {

	namespace {
		template <class Fun>
		struct get_signature
		{
			template<class FunObj, int Arity>
			struct get_nested_signature_impl;

			template<class FunObj>
			struct get_nested_signature_impl<FunObj, 0>
			{
				typedef typename FunObj::result_type (type) (void);
			};

			template<class FunObj>
			struct get_nested_signature_impl<FunObj, 1>
			{
				typedef typename FunObj::result_type (type) (typename FunObj::arg1_type);
			};

			template<class FunObj>
			struct get_nested_signature_impl<FunObj, 2>
			{
				typedef typename FunObj::result_type (type) (typename FunObj::arg1_type, typename FunObj::arg2_type);
			};

			template<class FunObj>
			struct get_nested_signature_impl<FunObj, 3>
			{
				typedef typename FunObj::result_type (type) (typename FunObj::arg1_type, typename FunObj::arg2_type, typename FunObj::arg3_type);
			};

			template<class FunObj>
			struct get_nested_signature_impl<FunObj, 4>
			{
				typedef typename FunObj::result_type (type) (typename FunObj::arg1_type, typename FunObj::arg2_type, typename FunObj::arg3_type, typename FunObj::arg4_type);
			};

			template<class FunObj>
			struct get_nested_signature
			{
				typedef typename get_nested_signature_impl<FunObj, FunObj::arity>::type type;
			};

			typedef typename boost::mpl::eval_if
			<
				typename boost::is_function_type<boost::function_pointer, Fun>::type,
				boost::mpl::identity<Fun>,
				get_nested_signature<Fun>
			>::type type;			
		};
	}
   
struct foreign_fun_base
{
    virtual ~foreign_fun_base() {}
    virtual value_t operator () (const cons_ptr_t&) const = 0;
};

template<class Fun>
struct foreign_fun : foreign_fun_base, boost::noncopyable
{
    foreign_fun(const Fun& fun, environment_t& env) : _fun(fun), _env(env) {}

	typedef typename get_signature<Fun>::type signature;

    virtual value_t operator () (const cons_ptr_t& arg) const
    {
        return invoke
            (
                arg,
				boost::mpl::or_
				<
					boost::is_same<const cons_ptr_t&, boost::function_type_parameter_c<signature, 0>::type>::type,
					boost::is_same<cons_ptr_t, boost::function_type_parameter_c<signature, 0>::type>::type
				>(),
                boost::is_same<void, boost::function_type_result<signature>::type>::type(), 
                boost::function_type_arity<signature>::type()
            );
    }

	value_t invoke(const cons_ptr_t& arg, boost::mpl::bool_<true>, boost::mpl::bool_<false>, boost::mpl::size_t<1>) const
	{
		return _fun(arg);
	}

	value_t invoke(const cons_ptr_t& arg, boost::mpl::bool_<true>, boost::mpl::bool_<true>, boost::mpl::size_t<1>) const
	{
		_fun(arg);
		return nil;
	}

	#define LEAN_FOREIGN_FUNCTION_UNWRAP(_z_, _n_, _ignored_) \
		boost::get<boost::remove_reference<boost::function_type_parameter_c<signature, _n_>::type>::type>(eval(nth(arg, _n_), _env))

	#define LEAN_FOREIGN_FUNCTION_DEFINE_INVOKE(_z_, _num_params_, _ignored_)									\
	    value_t invoke(const cons_ptr_t& arg, boost::mpl::bool_<false>, boost::mpl::bool_<false>, boost::mpl::size_t<_num_params_>) const	\
	    {																										\
			if (length(arg) != _num_params_)																	\
	            throw std::exception("::lean::runtime_error not enough arguments for function call");			\
																												\
			try																									\
			{																									\
	            return _fun(BOOST_PP_ENUM(_num_params_, LEAN_FOREIGN_FUNCTION_UNWRAP, ignored));				\
			}																									\
			catch (const boost::bad_get&)																		\
			{																									\
	            throw std::exception("::lean::runtime_error type mismatch in function-call");					\
			}																									\
		}																										\
																												\
		value_t invoke(const cons_ptr_t& arg, boost::mpl::bool_<false>, boost::mpl::bool_<true>, boost::mpl::size_t<_num_params_>) const	\
		{																										\
			if (length(arg) != _num_params_)																	\
				throw std::exception("::lean::runtime_error not enough arguments for function call");			\
																												\
			try																									\
			{																									\
				_fun(BOOST_PP_ENUM(_num_params_, LEAN_FOREIGN_FUNCTION_UNWRAP, ignored));						\
				return nil;																						\
			}																									\
			catch (const boost::bad_get&)																		\
			{																									\
				throw std::exception("::lean::runtime_error type mismatch in function-call");					\
			}																									\
		}

	BOOST_PP_REPEAT(LEAN_MAX_NUM_FOREIGN_FUN_PARAMS, LEAN_FOREIGN_FUNCTION_DEFINE_INVOKE, ignored)

	#undef LEAN_FOREIGN_FUNCTION_INVOKE
	#undef LEAN_FOREIGN_FUNCTION_UNWRAP

    Fun _fun;
    environment_t& _env;
};

}

#endif
