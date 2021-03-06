#ifndef NODEBIND_FUNCTION_HPP
#define NODEBIND_FUNCTION_HPP
#pragma once

#include "detail/meta.hpp"
#include "scope.hpp"
#include "overloaded.hpp"
#include "detail/argumenttransform.hpp"
#include "detail/functiontraits.hpp"
#include "converter.hpp"
#include "call.hpp"

namespace nodebind
{
	namespace detail
	{
		struct ArgumentScore
		{
			ArgumentScore(const v8::Arguments& args, bool isThisCall);

			void addScore(int score);

			template<size_t I, size_t N, typename T>
			void operator()()
			{
				size_t index = I;
				if (isThisCall)
				{
					if (I == 0)
					{
						int s = Converter<T>::score(args.This());
						addScore(s);
						return;
					}

					//Since this was considered argument 0, all the other index values are off-by-one.
					index--;
				}

				int s = Converter<T>::score(args[index]);
				addScore(s);
			}

			int score;
			bool isThisCall;
			const v8::Arguments& args;
		};

		template<typename FunctionPointer, typename PolicyList>
		class FreeFunction : public OverloadedFunction
		{
		public:
			typedef FunctionTraits<FunctionPointer> Traits;
			typedef SignatureTransformation<typename Traits::Return, typename Traits::Arguments, PolicyList> Transformed;

			FreeFunction(FunctionPointer function)
				:function(function)
			{}

			int score(const v8::Arguments& args) 
			{
				bool thiscall = detail::HasPolicy<PolicyList, ThiscallPolicyBase>::type::value;
				ArgumentScore score(args, thiscall);

				IndexedExecute<Transformed::ArgumentTypes>::invoke(score);

				return score.score;
			}

			v8::Handle<v8::Value> invoke(const v8::Arguments& args)
			{
				return GenericInvoke<FunctionPointer, PolicyList>::invoke(function, args);
			}
		private:
			FunctionPointer function;
		};

		template<typename FunctionPointer, typename PolicyList>
		class FunctionRegistration : public Registration
		{
		public:
			FunctionRegistration(const char * name, FunctionPointer func)
				:name(name)
				,function(func)
			{}

			void invoke(Context& ctx)
			{
				OverloadedFunctionResolver * resolver = ctx.findOverloadedFunction(name);
				resolver->add(new FreeFunction<FunctionPointer, PolicyList>(function));
			}
		private:
			const char * name;
			FunctionPointer function;
		};


		template<typename FunctionPointer, typename PolicyList> 
		detail::Registration * defineFunction(const char * name, FunctionPointer ptr)
		{
			return new FunctionRegistration<FunctionPointer, PolicyList>(name, ptr);
		}
	}

#include "detail/function_rep.hpp"
}

#endif