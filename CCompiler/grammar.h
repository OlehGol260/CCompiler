#pragma once

#include <vector>
#include <algorithm>

#include "bracket.h"
#include "lexem.h"

class Grammar
{
public:
	Grammar() = delete;
	~Grammar() = default;
	Grammar(const Grammar&) = delete;
	Grammar& operator=(const Grammar&) = delete;

	static bool IsOperator(const std::string& to_check);
	static bool IsBoolOp(const std::string& to_check);
	static bool IsMathOp(const std::string& to_check);
	static bool IsReservedWord(const std::string& to_check);
	static bool IsAssignment(const std::string& to_check);
	static bool IsBracket(const std::string& to_check);
	static bool IsPunctuator(const std::string& to_check);
	static bool IsType(const std::string& to_check);

	static bool IsOpenBrace(const std::string& to_check);
	static bool IsCloseBrace(const std::string& to_check);
	static bool IsOpenParenthesis(const std::string& to_check);
	static bool IsCloseParenthesis(const std::string& to_check);

	template<typename T>
	static bool IsInVector(const T& t, const std::vector<T>& vec)
	{
		return std::find(vec.cbegin(), vec.cend(), t) != vec.cend();
	}

	static LexemType GetType(const std::string& to_check);

	static auto open_parenthesis() { return kOpenParenthesis_; }
	static auto close_parenthesis() { return kCloseParenthesis_; }
	static auto open_brace() { return kOpenBrace_; }
	static auto close_brace() { return kCloseBrace_; }

	static auto reserved_words()
	{
		return kReservedWords_;
	}
	static auto math_operators()
	{
		return kMathOperators_;
	}
	static auto bool_operators()
	{
		return kBoolOperators_;
	}
	static auto assignment()
	{
		return kAssignment_;
	}
	static auto punctutor()
	{
		return kPunctuator_;
	}
	static auto types()
	{
		return kTypes_;
	}
private:
	static const std::string kOpenBrace_;
	static const std::string kCloseBrace_;
	static const std::string kOpenParenthesis_;
	static const std::string kCloseParenthesis_;
	static const std::vector<std::string> kReservedWords_;
	static const std::vector<std::string> kMathOperators_;
	static const std::vector<std::string> kBoolOperators_;
	static const std::vector<std::string> kTypes_;
	static const std::string kAssignment_;
	static const std::string kPunctuator_;
};



