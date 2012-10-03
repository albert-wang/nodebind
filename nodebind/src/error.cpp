#include "error.hpp"

namespace nodebind
{
	Error::Error(const std::string& what)
		:errorDescription(what)
	{}

	const char * Error::what() const
	{
		return errorDescription.c_str();
	}
}