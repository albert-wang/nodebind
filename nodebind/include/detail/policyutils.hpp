#ifndef NODEBIND_POLICYUTILS_HPP
#define NODEBIND_POLICYUTILS_HPP
#pragma once

namespace nodebind
{
	namespace detail
	{
		template<size_t TargetIndex>
		struct FilterForArgumentIndex
		{
			template<typename T>
			struct apply 
				: boost::mpl::bool_<T::Index == TargetIndex>
			{};
		};

		template<typename Policies, typename Base>
		struct HasPolicy 
			: boost::mpl::not_< 
				typename boost::is_same<
					typename boost::mpl::find_if< Policies, boost::is_base_of< Base, boost::mpl::_1 > >::type, 
					typename boost::mpl::end< Policies >::type
				>::type
			>
		{};

		//Given a list of policies and a target index, returns all the policies for that index.
		template<typename Policies, size_t Target>
		struct PoliciesFor
		{
			typedef typename boost::mpl::copy_if< Policies, FilterForArgumentIndex<Target> >::type type;
		};

		//Transforms mpl::vector to a tuple
		template<typename TypeVector>
		struct MakeTuple
		{
			typedef typename boost::mpl::reverse_fold<TypeVector, boost::tuples::null_type, boost::tuples::cons<boost::mpl::_2, boost::mpl::_1> >::type type;
		};

		template<typename ArgType, typename Pols, size_t N>
		struct ArgumentPolicyPair
		{
			typedef ArgType Argument;
			typedef Pols Policies;
			static const size_t Index = N;
		};

		template<size_t N>
		struct ArgumentPoliciesImpl
		{};

		template<typename T>
		struct IsTuple
			: public boost::mpl::false_
		{
			typedef boost::mpl::vector<> types;
		};

		template<typename T>
		struct Holder
		{
			typedef T type;
		};

		//This takes in a mpl::vector and a Visitor, and calls Visitor(index, size, detail::Holder<T>) for every type in the vector.
		template<typename T>
		struct IndexedExecute
		{
			//Indexed Execute should only be invoked on an mpl::vector type.
			BOOST_STATIC_ASSERT(boost::mpl::is_sequence<T>::value);

			template<size_t I, size_t N>
			struct VisitorHelper
			{
				template<typename Visitor>
				static void invoke(Visitor& visit)
				{
					typedef typename boost::mpl::at_c<T, I>::type CurrentType;
					visit.operator()<I, N, CurrentType>();

					VisitorHelper<I + 1, N>::invoke(visit);
				}
			};

			template<size_t N>
			struct VisitorHelper<N, N>
			{
				template<typename Visitor>
				static void invoke(Visitor&)
				{}
			};
			
			template<typename Visitor>
			static void invoke(Visitor& visit)
			{
				VisitorHelper<0, boost::mpl::size<T>::value>::invoke(visit);
			}
		};

		//MPL Pretty printer. Useful for debugging things.
		struct VectorPrint
		{
			VectorPrint(std::ostream& stream)
				:stream(stream)
			{
				stream << "mpl::vector<"; 
			}

			template<size_t Index, size_t Size, typename T>
			void operator()()
			{
				if (Index) 
				{
					stream <<", ";
				}
				
				stream << typeid(T).name();

				if (boost::is_const<boost::remove_reference<T>::type>::value)
				{
					stream <<" const ";
				}

				if (boost::is_reference<T>::value)
				{
					stream << "&";
				}

				if (Index == Size - 1) 
				{
					stream << ">";
				}
			}

			std::ostream& stream;
		};

		template<typename T>
		void printVector(std::ostream& stream, const char * append)
		{
			VectorPrint printer(stream);
			IndexedExecute<T>::invoke(printer);
			if (append) 
			{
				stream << append; 
			}
		}

		//Generate specializations for ArgumentPoliciesImpl, IsTuple and MakeTuple.
#include "detail/argpolicies_rep.hpp"

		//Returns a list of ArgumentPolicyPair
		template<typename Args, typename Policies>
		struct ArgumentPolicies
		{
			typedef typename ArgumentPoliciesImpl<
				boost::mpl::size<Args>::type::value
			>::template apply<Args, Policies>::type type;
		};
	}
}


#endif