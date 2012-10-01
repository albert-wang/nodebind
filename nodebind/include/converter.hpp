#ifndef NODEBIND_CONVERTER_HPP
#define NODEBIND_CONVERTER_HPP
#pragma once

#include "detail/meta.hpp"

namespace nodebind
{
	template<typename T>
	struct Converter
	{
		/* 
			Return -1 for unconvertible, 0 for convertible, and 1 for exact match.
			During function resolution, if any negative value is returned, that method
			is automatically removed from overload lookup. Otherwise, the highest scoring
			match is returned. If any two matches have the same score, an ambiguous
			function call is raised.
		*/

		static int score(const v8::Handle<v8::Value>& object)
		{
			return -1;
		}
	
		static T from(const v8::Handle<v8::Value>& object)
		{
			return T();
		}

		static v8::Handle<v8::Value> to(const T&)
		{
			return v8::Undefined();
		}

		//Used to clean up anything that was done in a 'from' call.
		static void after(const T& t)
		{}
	};

}

#endif