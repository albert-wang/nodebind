/* 
 * Generates nodebind::def
 */

#ifndef BOOST_PP_IS_ITERATING
#	ifndef FUNCTIONREP_HPP
#	define FUNCTIONREP_HPP
#		include <boost/preprocessor/repetition.hpp>
#		include <boost/preprocessor/punctuation/comma_if.hpp>
#		include <boost/preprocessor/iteration/iterate.hpp>
#		define BOOST_PP_ITERATION_LIMITS (0, 9)
#		define BOOST_PP_FILENAME_1 "detail/function_rep.hpp"
#		include BOOST_PP_ITERATE()
#	endif
#else
#define N BOOST_PP_ITERATION()

template<typename FunctionPointer BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename T)>
detail::Registration * def(const char * f, FunctionPointer func BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, const T))
{
	typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N, T)> PolicyList;
	return detail::defineFunction<FunctionPointer, PolicyList>(f, func);
}

#endif
