#ifndef NODEBIND_FUNCTION_HPP
#define NODEBIND_FUNCTION_HPP
#pragma once

#include "detail/meta.hpp"
#include "scope.hpp"
#include "overloaded.hpp"
#include "detail/functiontraits.hpp"

namespace nodebind
{
	namespace detail
	{
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
				//OverloadedFunctionResolver * resolver = ctx.getOverloadedFunctionResolver(name);
				//resolver->add(name, func);
			}
		private:
			const char * name;
			FunctionPointer function;
		};

		struct ArgumentScore
		{
			ArgumentScore(const v8::Arguments& args, bool isThisCall);

			void addScore(int score);

			template<typename T>
			void operator()(size_t index, size_t, const detail::Holder<T>&)
			{
				if (isThisCall)
				{
					if (index == 0)
					{
						int s = Converter<typename T::Argument>::score(args.This());
						addScore(s);
						return;
					}

					//Since this was considered argument 0, all the other index values are off-by-one.
					index--;
				}

				int s = Converter<typename T::Argument>::score(args[index]);
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

			int score(const v8::Arguments& args) 
			{
				bool thiscall = detail::HasPolicy<PolicyList, ThiscallPolicyBase>::type::value;
				ArgumentScore score(args, thiscall);

				IndexedExecute<Transformed::TransformedArguments>::invoke(score);

				return score.score;
			}

			v8::Handle<v8::Object> invoke(const v8::Arguments& args)
			{

			}
		};

		template<typename FunctionPointer, typename PolicyList> 
		detail::Registration * defineFunction(const char * name, FunctionPointer ptr)
		{
			std::cout << "Registering function: " << name << " with signature " << typeid(FunctionPointer).name() << " and policy list: ";
			detail::printVector<PolicyList>(std::cout, "\n");

			return new FunctionRegistration<FunctionPointer, PolicyList>(name, ptr);
		}
	}

#include "detail/function_rep.hpp"
}

#endif