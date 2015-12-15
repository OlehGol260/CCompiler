#pragma once
#include <string>
#include <sstream>

#include "enums.h"

/*!
	Holds static members which can terminate the programwith writing to stderr some message
*/
class ErrMessage
{
public:
	/*!
		Terminates the program with the given str
	*/
	static void AbortMsg(const std::string& str)
	{
		fputs(str.c_str(), stderr);
		abort();
	}

	/*!
		Terminates the program with the given str about invallid token
	*/
	static void AbortInvalidToken(const std::string& value)
	{
		auto str = "Invalid token: " + value;
		AbortMsg(str);
	}

	/*!
		Terminates the program with the given str about open and close bracket mismatch
	*/
	static void AbortInvalidNumberBrackets(const BracketType& t)
	{
		std::stringstream ss;
		ss << "Number of open and closed " << (t == BracketType::kParentheses ? "parentheses" : "brackets") << " doesn't match";
		AbortMsg(ss.str());
	}

	/*!
		Terminates the program signalyzing an invalid token order.
	*/
	static void AbortInvalidNextToken(const std::string& cur, const std::string& next)
	{
		AbortMsg("Invalid next token after: " + cur + "  Meet: " + next);
	}

	/*!
		Terminates the program signalyzing that var_name has invalid name in terms of c++
	*/
	static void AbortInvalidVariableName(const std::string& var_name)
	{
		AbortMsg("Variable " + var_name + " containes prohibited symbols");
	}

	/*!
		Terminates the program signalyzing that the variable var_name hasn't been declared in the current scope
	*/
	static void AbortUndeclaredVariable(const std::string& var_name)
	{
		AbortMsg("Variable " + var_name + " has not been declared in the current scope");
	}

	/*!
		Terminates the program signalyzing that the variable var_name hasn't been declared in the current scope
	*/
	static void AbortUnhandledOperation(const std::string& op, const std::vector<VariableType> types)
	{
		std::string types_ = "";
		for (auto& type : types)
		{
			types_ += VariableTypeStr.at(type) + ", ";
		}
		AbortMsg("Try to perform operation '" + op + "' for types: " + types_);
	}
};
