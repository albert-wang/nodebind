#ifndef NODEBIND_ARGTRANSFORM_HPP
#define NODEBIND_ARGTRANSFORM_HPP
#pragma once

#include "policy.hpp"
#include "detail/policyutils.hpp"

namespace nodebind
{
	namespace detail
	{
		struct HasOutPolicy
		{
			template<typename T>
			struct apply 
				: boost::mpl::or_<
					HasPolicy< typename T::Policies, PureOutPolicyBase >,
					HasPolicy< typename T::Policies, InOutPolicyBase >
				>
			{};
		};

		struct NotHasPureOutPolicy
		{
			template<typename T>
			struct apply : 
				boost::mpl::not_< HasPolicy<typename T::Policies, PureOutPolicyBase> >
			{};
		};

		struct TakeArgument
		{
			template<typename T>
			struct apply
			{
				typedef typename T::Argument type;
			};
		};

		struct TakeIndex
		{
			template<typename T>
			struct apply
			{
				typedef typename boost::mpl::int_< T::Index > type;
			};
		};

		struct IndirectOnce
		{
			template<typename T>
			struct apply
				: boost::remove_cv<
					typename boost::remove_reference< typename boost::remove_pointer<T>::type >::type
				>
			{};
		};

		template<typename ReturnType>
		struct GetReturnStorage 
			: boost::mpl::if_< 
				boost::is_same<ReturnType, void>, 
				int, 
				ReturnType
			>
		{};

		//The return transformation
		template<typename ReturnType, typename Arguments, typename PolicyList>
		struct ReturnTransformation
		{
			typedef typename ArgumentPolicies<Arguments, PolicyList>::type ArgumentPolicies;
			
			// map( map( copy_if(argumentpolifies, hasoutpolicy), takeargument) indirectonce)
			typedef typename boost::mpl::transform<
				typename boost::mpl::transform< 
					typename boost::mpl::copy_if<ArgumentPolicies, HasOutPolicy>::type, 
					TakeArgument
				>::type,
			IndirectOnce>::type OutArguments;

			typedef typename GetReturnStorage<ReturnType>::type ReturnStorage;

			//This implies that tuple return values cannot use output parameters.
			typedef typename boost::mpl::if_<
				IsTuple<ReturnStorage>, 
				typename IsTuple<ReturnStorage>::types,
				typename boost::mpl::copy<OutArguments, boost::mpl::back_inserter< boost::mpl::vector<ReturnStorage> > >::type
			>::type type;
		};

		//The argument transformation.
		template<typename Arguments, typename PolicyList>
		struct ArgumentTransformation
		{
			typedef typename ArgumentPolicies<Arguments, PolicyList>::type ArgumentPolicies;
			typedef typename boost::mpl::copy_if<ArgumentPolicies, NotHasPureOutPolicy>::type OutParameters;

			typedef typename boost::mpl::copy<
				typename boost::mpl::transform<OutParameters, TakeArgument>::type,
				boost::mpl::back_inserter< boost::mpl::vector<> >
			>::type type;

			typedef typename boost::mpl::copy<
				typename boost::mpl::transform<OutParameters, TakeIndex>::type, 
				boost::mpl::back_inserter< boost::mpl::vector<> >
			>::type IndexMappings;
		};

		//Combined Transformation
		template<typename ReturnType, typename Arguments, typename PolicyList>
		struct SignatureTransformation
		{
			//This is a mpl::vector of the return values. Values are indirected once - less one 
			//pointer, less one reference.
			typedef typename ReturnTransformation<ReturnType, Arguments, PolicyList>::type ReturnTypes;
			typedef typename ArgumentTransformation<Arguments, PolicyList> TransformedArguments;

			//This is the signature that is exposed to node.
			typedef typename TransformedArguments::type ArgumentTypes;

			//This is the storage for the C++ method. Same as the actual signature, except all 
			//References are turned into pointers, 

			typedef typename TransformedArguments::IndexMappings ArgumentIndexMappings; 
		};
	}
}

#endif
