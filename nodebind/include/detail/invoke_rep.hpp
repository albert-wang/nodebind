/* 
 * Generates nodebind::detail::freeInvoke and detail::memberInvoke
 */

#ifndef BOOST_PP_IS_ITERATING
#	ifndef NODEBIND_INVOKEREP_HPP
#	define NODEBIND_INVOKEREP_HPP
#		include <boost/preprocessor/repetition.hpp>
#		include <boost/preprocessor/punctuation/comma_if.hpp>
#		include <boost/preprocessor/iteration/iterate.hpp>
#		define BOOST_PP_ITERATION_LIMITS (0, 9)
#		define BOOST_PP_FILENAME_1 "detail/invoke_rep.hpp"
#		include BOOST_PP_ITERATE()
#	endif
#else
#define N BOOST_PP_ITERATION()
#define GET_ARGUMENT(z, n, data) boost::get<n>(data)
#define GET_ARGUMENT_THIS(z, n, data) boost::get<n + 1>(data)


template<>
struct FreeInvoke<N>
{
	template<typename Function, typename Tuple>
	static typename
		boost::enable_if<
			boost::is_same<typename FunctionTraits<Function>::Return, void>,
			typename GetReturnStorage<typename FunctionTraits<Function>::Return>::type
		>::type
	invoke(Function ptr, Tuple& args)
	{
		(*ptr)(BOOST_PP_ENUM(N, GET_ARGUMENT, args));
		return 0;
	}

	template<typename Function, typename Tuple>
	static typename
		boost::disable_if<
			boost::is_same<typename FunctionTraits<Function>::Return, void>,
			typename GetReturnStorage<typename FunctionTraits<Function>::Return>::type
		>::type
	invoke(Function ptr, Tuple& args)
	{
		return (*ptr)(BOOST_PP_ENUM(N, GET_ARGUMENT, args));
	}
};

template<>
struct MemberInvoke<N>
{
	template<typename Function, typename Tuple>
	static typename
		boost::enable_if<
			boost::is_same<typename FunctionTraits<Function>::Return, void>,
			typename GetReturnStorage<typename FunctionTraits<Function>::Return>::type
		>::type
	invoke(Function ptr, Tuple& args)
	{
		((boost::get<0>(args))->*ptr)(BOOST_PP_ENUM(N, GET_ARGUMENT_THIS, args));
		return 0;
	}

	template<typename Function, typename Tuple>
	static typename
		boost::disable_if<
			boost::is_same<typename FunctionTraits<Function>::Return, void>,
			typename GetReturnStorage<typename FunctionTraits<Function>::Return>::type
		>::type
	invoke(Function ptr, Tuple& args)
	{
		return ((boost::get<0>(args))->*ptr)(BOOST_PP_ENUM(N, GET_ARGUMENT_THIS, args));
	}
};

#undef GET_ARGUMENT
#undef GET_ARGUMENT_THIS
#undef N
#endif