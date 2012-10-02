/* 
 * Generates nodebind::FunctionTraits
 */

#ifndef BOOST_PP_IS_ITERATING
#	ifndef NODEBIND_FUNCTIONTRAITSREP_HPP
#	define NODEBIND_FUNCTIONTRAITSREP_HPP
#		include <boost/preprocessor/repetition.hpp>
#		include <boost/preprocessor/punctuation/comma_if.hpp>
#		include <boost/preprocessor/iteration/iterate.hpp>
#		define BOOST_PP_ITERATION_LIMITS (1, 9)
#		define BOOST_PP_FILENAME_1 "detail/functiontraits_rep.hpp"
#		include BOOST_PP_ITERATE()
#	endif
#else

#define n BOOST_PP_ITERATION()
#define TRANSFORM_ARGUMENT(z, n, data) typename ArgumentTransform<T##n>::Result

namespace nodebind
{
	namespace detail
	{
		//Free function traits
		template<typename Ret, BOOST_PP_ENUM_PARAMS(n, typename T)>
		struct FunctionTraits<Ret (*)( BOOST_PP_ENUM_PARAMS(n, T) )>
		{
			typedef boost::mpl::false_ MemberFunction;
			typedef Ret Return;
			typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(n, T)> Arguments;
			static const size_t Arity = n;
		};
		
		//Member function traits.
		template<typename Ret, typename cls, BOOST_PP_ENUM_PARAMS(n, typename T)>
		struct FunctionTraits<Ret (cls::*)( BOOST_PP_ENUM_PARAMS(n, T) )>
		{
			typedef boost::mpl::true_ MemberFunction;
			typedef cls Class;
			typedef Ret Return;
			typedef boost::mpl::vector<cls * BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, T)> Arguments;
			static const size_t Arity = n;
		};

		template<typename Ret, typename cls, BOOST_PP_ENUM_PARAMS(n, typename T)>
		struct FunctionTraits<Ret (cls::*)( BOOST_PP_ENUM_PARAMS(n, T) ) const>
		{
			typedef boost::mpl::true_ MemberFunction;
			typedef cls Class;
			typedef Ret Return;
			typedef boost::mpl::vector<cls * BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, T)> Arguments;
			static const size_t Arity = n;
		};
	}
}

#undef n
#undef TRANSFORM_ARGUMENT
#endif
