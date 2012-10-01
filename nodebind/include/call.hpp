#ifndef NODEBIND_CALL_HPP
#define NODEBIND_CALL_HPP
#pragma once

#include "detail/meta.hpp"
#include "detail/functiontraits.hpp"

namespace nodebind
{
	namespace detail
	{
		/*
			This is the converter and invoker for all functions.
			It takes in a Function pointer type, and a policy list, and does everything 'right',
			where 'right' is defined as 

				- If it has a `ThisCall` policy, it unserializes the object in args.This(). 
				  It must be a registered class through nodebind. The class shall be referred to as T.
					If the function pointer is a free function with T* or T& or a CV qualified version
					of the above, then it is called with args.This() as the first argument, and the rest
					of the arguments following.

					If the function pointer is a member function, then the pointer is invoked with the 
					arguments as given.

				- Out parameters, both `PureOut` and `InOut` are created in the return tuple prior to the
				  call. `InOut` parameters are value types in the return tuple, and pointers in the argument
				  tuple. The value in the return tuple is initialized to the one passed in, and then a pointer
				  to the value in the return tuple is set in the argument tuple.
					As an example, given a (nonsense) function 
						bool transform_value(int amount, double * inout) { *inout += amount; return amount % 2; } 
					with `inout` declared as an `InOut` parameter, the argument tuple will be
						tuple<int, double *>
					and the return tuple will be
						tuple<bool, double>

					Let the function be called from javascript with the arguments (1, 2.0)
						The argument tuple will be first initialized: 
							<1, nullptr>
						and the return tuple will be initialized: 
							<unspecified-boolean-value, 2.0>
						Then, to setup the proper argument tuple, the following operation is performed:
							args[1] = &ret[1], resulting in the argument list being: 
								<1, pointer-to-ret[1]>

					PureOut parameters are done in the same way, except that that the initial initialization
					in the return tuple is skipped.

				- If the AsyncPolicy exists, then the function call is split over 3 distinct steps.
					First, the arguments are deserialized and setup into tuples.
					Then, the actual invocation is offloaded into the event loop. This function must not do any v8 manipulation.
					Lastly, the return tuple is serialized into javascript, and the callback method is invoked.

					The final argument passed into node is expected to be a callback in the form 
						function (err, ret0, ret1, ret2...) { ... }
					The 'this' pointer in the callback function is unspecified.

		*/

		//Deserializes arguments.


		template<typename Function, typename PolicyList>
		class GenericInvoke
		{
			typedef typename FunctionTraits<Function> Traits;
			typedef SignatureTransformation<typename Traits::Return, typename Traits::Arguments, PolicyList> Transformed;

			typedef v8::Handle<v8::Value> HValue;
		public:
			struct Baton
			{
				Function method;
				typename Transformed::ReturnTypes returnTuple;
				typename MakeTuple<Traits::Arguments>::type argumentTuple;
			};


			static typename boost::enable_if<Traits::MemberFunction, HValue>::type invoke(const v8::Arguments& args)
			{
				//If its a member function, it must have a this-call policy attached.
				BOOST_STATIC_ASSERT(HasPolicy<PolicyList, ThiscallPolicyBase>::type::value);
			}

			static typename boost::disable_if<Traits::MemberFunction, HValue>::type invoke(const v8::Arguments& args)
			{
				if (HasPolicy<PolicyList, ThiscallPolicyBase>::type::value)
				{

				}
				else 
				{

				}
			}

		private:
		};
	}
}
#endif