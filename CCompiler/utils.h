#pragma once

#include <sstream>
#include "enums.h"
#include <algorithm>

namespace utils
{
	/*!
		Check if given string is valid integer OR floating point number
		\param my_string what to check
		\return true if is number, false otherwise
	*/
	inline bool IsNumber(const std::string& my_string) {
		std::istringstream iss(my_string);
		float f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
								   // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}
}
