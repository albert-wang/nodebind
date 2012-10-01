/* 
 * Generates nodebind::FunctionTraits
 */

#ifndef BOOST_PP_IS_ITERATING
#	ifndef FUNCTIONTRAITS_HPP
#	define FUNCTIONTRAITS_HPP
#		include <boost/preprocessor/repetition.hpp>
#		include <boost/preprocessor/punctuation/comma_if.hpp>
#		include <boost/preprocessor/iteration/iterate.hpp>
#		define BOOST_PP_ITERATION_LIMITS (0, 9)
#		define BOOST_PP_FILENAME_1 "detail/argpolicies_rep.hpp"
#		include BOOST_PP_ITERATE()
#	endif
#else

#define n BOOST_PP_ITERATION()
#define MAKE_PAIR(z, i, data) ArgumentPolicyPair< typename boost::mpl::at_c<Args, i>::type, typename PoliciesFor<Pols, i>::type, i>

template<>
struct ArgumentPoliciesImpl<n>
{
	template<typename Args, typename Pols>
	struct apply
	{
		typedef boost::mpl::vector<
			BOOST_PP_ENUM(n, MAKE_PAIR, ~)
		> type;
	};
};

template<BOOST_PP_ENUM_PARAMS(n, typename T)>
struct IsTuple< boost::tuple< BOOST_PP_ENUM_PARAMS(n, T) > > 
	: public boost::mpl::true_
{
	typedef boost::mpl::vector< BOOST_PP_ENUM_PARAMS(n, T) > types;
};

#undef n
#undef MAKE_PAIR
#endif
