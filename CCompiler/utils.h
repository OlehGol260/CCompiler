#pragma once

#include <sstream>
#include "enums.h"
#include <algorithm>

namespace utils
{
	inline bool IsNumber(const std::string& myString) {
		std::istringstream iss(myString);
		float f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
								   // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}

	inline bool StringToBool(const std::string& value)
	{
		auto copy = value;
		std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
		std::istringstream is(copy);
		bool b;
		is >> std::boolalpha >> b;
		return b;
	}
}
