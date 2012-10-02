#include <iostream>
#include "main.h"
#include "detail/meta.hpp"
#include "policy.hpp"
#include "detail/policyutils.hpp"
#include "detail/argumenttransform.hpp"

#include "nodebind.hpp"

#include <boost/lexical_cast.hpp>
#include <string>

std::string invoke(int a)
{
	return boost::lexical_cast<std::string>(a);
}

void init(v8::Handle<v8::Object> target)
{
	using namespace nodebind;

	typedef boost::mpl::vector< InOutPolicy<1>, PureOutPolicy<2> > pols;
	typedef boost::mpl::vector< float, const char&, const bool *, double > args;
	typedef detail::ReturnTransformation<void, args, pols > res;
	typedef detail::ArgumentTransformation< args, pols > argtrans;

	nodebind::detail::printVector<args>(std::cout, "\n");
	nodebind::detail::printVector<argtrans::ArgumentStorage>(std::cout, "\n");

	nodebind::Context context = nodebind::open(target);

	nodebind::module(context) 
		[
			nodebind::def("itoa", invoke)
		];

	target->Set(v8::String::NewSymbol("Hello"), v8::String::New("Hello!"));
}