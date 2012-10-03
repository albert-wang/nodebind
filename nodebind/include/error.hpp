#ifndef NODEBIND_ERROR_HPP
#define NODEBIND_ERROR_HPP
#pragma once

#include <exception>
#include <string>

namespace nodebind
{
	class Error : public std::exception
	{
	public:
		Error(const std::string& what);

		virtual const char * what() const;
	private:
		std::string errorDescription;
	};
}
#endif