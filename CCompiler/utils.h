#pragma once

#include <sstream>
#include "enums.h"

namespace utils
{
	inline bool IsNumber(const std::string& myString) {
		std::istringstream iss(myString);
		float f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
								   // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}
}
