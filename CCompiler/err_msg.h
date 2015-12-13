#pragma once
#include <string>
#include <sstream>

#include "enums.h"

class ErrMessage
{
public:
	static void AbortMsg(const std::string& str)
	{
		fputs(str.c_str(), stderr);
		abort();
	}
	static void AbortInvalidToken(const std::string& value)
	{
		auto str = "Invalid token: " + value;
		AbortMsg(str);
	}
	static void AbortInvalidNumberBrackets(const BracketType& t)
	{
		std::stringstream ss;
		ss << "Number of open and closed " << (t == BracketType::kParentheses ? "parentheses" : "brackets") << " doesn't match";
		AbortMsg(ss.str());
	}

	static void AbortInvalidNextToken(const std::string& cur, const std::string& next)
	{
		AbortMsg("Invalid next token after: " + cur + "  Meet: " + next);
	}

	static void AbortInvalidVariableName(const std::string& var_name)
	{
		AbortMsg("Variable " + var_name + " containes prohibited symbols");
	}

	static void AbortUndeclaredVariable(const std::string& var_name)
	{
		AbortMsg("Variable " + var_name + " has not been declared");
	}
};
