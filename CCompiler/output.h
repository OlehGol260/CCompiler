#pragma once
#include <string>
#include <sstream>
#include "type_name_traits.h"

#define VNAME(x) #x

namespace output
{
	template<typename T>
	inline std::string variable(const T& t)
	{
		std::stringstream ss;
		ss << "Variable " << VNAME(t) << "of type" << TypeName<T>::Get();
		return ss.str();
	}
}
