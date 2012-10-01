#ifndef NODEBIND_POLICYCONS_HPP
#define NODEBIND_POLICYCONS_HPP
#pragma once

#include <boost/mpl/vector.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include "argumenttransform.hpp"

namespace nodebind
{
	namespace detail
	{
		//From a function signature, derive the return value and a mpl vector of its arguments.
		template<typename Sig>
		struct FunctionTraits
		{};

		template<typename Ret>
		struct FunctionTraits<Ret (*)(void)>
		{
			typedef boost::mpl::false_ MemberFunction;
			typedef Ret Return;
			typedef boost::mpl::vector<> Arguments;
			static const size_t Arity = 0;
		};

		template<typename Ret, typename T>
		struct FunctionTraits<Ret (T::*)(void)>
		{
			typedef boost::mpl::true_ MemberFunction;
			typedef T Class;
			typedef Ret Return;
			typedef boost::mpl::vector<T *> Arguments;
			static const size_t Arity = 0;
		};

		template<typename Ret, typename T>
		struct FunctionTraits<Ret (T::*)(void) const>
		{
			typedef boost::mpl::true_ MemberFunction;
			typedef T Class;
			typedef Ret Return;
			typedef boost::mpl::vector<T *> Arguments;
			static const size_t Arity = 0;
		};
	}
}

#include "functiontraits_rep.hpp"

#endif
