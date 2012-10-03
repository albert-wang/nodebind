#include <iostream>
#include "main.h"
#include "detail/meta.hpp"
#include "policy.hpp"
#include "detail/policyutils.hpp"
#include "detail/argumenttransform.hpp"
#include "detail/invoke.hpp"
#include "nodebind.hpp"
#include "primitiveconverters.hpp"

#include <boost/lexical_cast.hpp>
#include <string>

std::string invoke(int a)
{
	std::cout << "Calling invoke with : " << a << "\n";
	return boost::lexical_cast<std::string>(a);
}

void init(v8::Handle<v8::Object> target)
{
	using namespace nodebind;

	boost::tuple<int> value = boost::make_tuple(42);
	detail::freeInvoke(invoke, value);


	nodebind::Context context = nodebind::open(target);

	nodebind::module(context) 
		[
			nodebind::def("itoa", invoke)
		];
}