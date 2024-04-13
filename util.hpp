#ifndef TOKOX_FRACTIONS_UTIL
#define TOKOX_FRACTIONS_UTIL

#include <string>
#include <stdexcept>
#include <cxxabi.h>
#include <typeinfo>

namespace tokox
{

template<typename T>
std::string get_typename()
{
	int status;
	char* real_name = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
	switch (status)
	{
		case 0:
			break;
		case -1:
			throw std::runtime_error("Failed allocating memory in convert_to_type_by_name -> abi::__cxa_demangle");
			break;
		case -2:
			throw std::runtime_error("Invalid typeid(T).name() in convert_to_type_by_name -> abi::__cxa_demangle");
			break;
		case -3:
			throw std::invalid_argument("Invalid argument in convert_to_type_by_name -> abi::__cxa_demangle");
			break;
		default:
			throw std::runtime_error("Unknown error in convert_to_type_by_name -> abi::__cxa_demangle");
	}
	std::string real_name_str = real_name;
	std::free(real_name);
	return real_name_str;
}

}

#endif
