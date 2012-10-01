#ifndef NODEBIND_OVERLOADED_HPP
#define NODEBIND_OVERLOADED_HPP
#pragma once

#include "detail/meta.hpp"

namespace nodebind
{
	class OverloadedFunction
	{
	public:
		virtual ~OverloadedFunction();
		virtual int score(const v8::Arguments& args) = 0;
		virtual v8::Handle<v8::Value> invoke(const v8::Arguments& args) = 0;
	};

	class OverloadedFunctionResolver
	{
	public:
		static v8::Handle<v8::Value> invoke(const v8::Arguments& args);
	private:
	};
}
#endif