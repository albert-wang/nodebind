#include "overloaded.hpp"
#include "error.hpp"

namespace nodebind
{
	OverloadedFunction::~OverloadedFunction()
	{}

	v8::Handle<v8::Value> OverloadedFunctionResolver::invoke(const v8::Arguments& args)
	{
		v8::HandleScope scope;
		v8::Local<v8::Value> external = args.Data();

		OverloadedFunctionResolver * self = static_cast<OverloadedFunctionResolver *>(v8::External::Unwrap(external));


		//This is a pretty common case.
		if (self->functions.size() == 1)
		{
			OverloadedFunction * f = self->functions[0];

			if (f->score(args) < 0)
			{
				//This is an invalid function.
				throw Error("There was no matching function for this call");
			}

			return f->invoke(args);
		}

		//Compute the score, and find the highest rated one.
		OverloadedFunction * currentBestOverload = nullptr;
		int currentBestScore = -100000;

		for (size_t i = 0; i < self->functions.size(); ++i)
		{
			if (self->functions[i]->score(args) > currentBestScore)
			{
				currentBestOverload = self->functions[i];
			}
		}

		for (size_t i = 0; i < self->functions.size(); ++i)
		{
			if (self->functions[i]->score(args) == currentBestScore && self->functions[i] != currentBestOverload)
			{
				//This is an error - ambiguous overload
				throw Error("Ambiguous overloads detected");
			}
		}

		//Then, invoke it.
		return currentBestOverload->invoke(args);
	}

	void OverloadedFunctionResolver::add(OverloadedFunction * f)
	{
		functions.push_back(f);
	}
}