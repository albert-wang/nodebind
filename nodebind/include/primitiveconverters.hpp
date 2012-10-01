#ifndef NODEBIND_PRIMITIVE_CONVERTERS_HPP
#define NODEBIND_PRIMITIVE_CONVERTERS_HPP
#pragma once

#include "converter.hpp"

namespace nodebind
{
#define CONVERTER_FOR(type) \
	template<>						\
	struct Converter<type>			\
	{								\
		static int score(const v8::Handle<v8::Value>& object); \
		static type from(const v8::Handle<v8::Value>& object); \
		static v8::Handle<v8::Value> to(const type&); \
		static void after(const type&);	\
	};								\
	template<> struct Converter<const type> : public Converter<type> {}; \
	template<> struct Converter<const type&> : public Converter<type> {}
	
	CONVERTER_FOR(boost::int32_t);
	CONVERTER_FOR(boost::uint32_t);
	CONVERTER_FOR(boost::int64_t);
	CONVERTER_FOR(boost::uint64_t);
	CONVERTER_FOR(float);
	CONVERTER_FOR(double);
	CONVERTER_FOR(bool);

	CONVERTER_FOR(std::string);
	CONVERTER_FOR(char *);


#undef CONVERTER_FOR
}

#endif