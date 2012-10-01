#include "overloaded.hpp"

namespace nodebind
{
	OverloadedFunction::~OverloadedFunction()
	{}

	v8::Handle<v8::Value> OverloadedFunctionResolver::invoke(const v8::Arguments& args)
	{
		//Compute the score, and find the highest rated one.

		//Then, invoke it.

		return v8::Undefined();
	}
}