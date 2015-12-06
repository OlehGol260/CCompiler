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

	template<typename T>
	static bool IsOperator(const T& t)
	{
		return IsBinaryOperator(t) || IsBracket(t) ||
			IsPunctuator(t) || IsVarType(t);
	}

	template<typename T>
	static bool IsBracket(const T& t)
	{
		return IsOpenParenthesis(t) || IsCloseParenthesis(t) ||
			IsOpenBrace(t) || IsCloseBrace(t);
	}

	template<typename T>
	static bool IsInVector(const T& t, const std::vector<T>& vec)
	{
		return std::find(vec.cbegin(), vec.cend(), t) != vec.cend();
	}


	static bool IsReservedWord(const std::string& to_check);
	static bool IsAssignment(const std::string& to_check);
	static bool IsPunctuator(const std::string& to_check);
	static bool IsVarType(const std::string& to_check);
	static bool IsLogicalNot(const std::string& to_check);
	static bool IsBinaryOperator(const std::string& to_check);
	static bool IsOpenBrace(const std::string& to_check);
	static bool IsCloseBrace(const std::string& to_check);
	static bool IsOpenParenthesis(const std::string& to_check);
	static bool IsCloseParenthesis(const std::string& to_check);

	static bool IsBinaryOperator(const LexemType& t);
	static bool IsReservedWord(const LexemType& t);
	static bool IsAssignment(const LexemType& t);
	static bool IsPunctuator(const LexemType& t);
	static bool IsVarType(const LexemType& t);
	static bool IsLogicalNot(const LexemType& t);
	static bool IsOpenBrace(const LexemType& t);
	static bool IsCloseBrace(const LexemType& t);
	static bool IsOpenParenthesis(const LexemType& t);
	static bool IsCloseParenthesis(const LexemType& t);
	static bool IsVariable(const LexemType& t);
	static bool IsLiteral(const LexemType& t);
	static LexemType GetType(const std::string& to_check);

	static auto& open_parenthesis() { return kOpenParenthesis_; }
	static auto& close_parenthesis() { return kCloseParenthesis_; }
	static auto& open_brace() { return kOpenBrace_; }
	static auto& close_brace() { return kCloseBrace_; }

	static auto& reserved_words()
	{
		return kReservedWords_;
	}
	static auto& binary_operators()
	{
		return kBinaryOperators;
	}
	static auto& assignment()
	{
		return kAssignment_;
	}
	static auto& punctutor()
	{
		return kPunctuator_;
	}
	static auto& types()
	{
		return kTypes_;
	}

private:
	static const std::string kOpenBrace_;
	static const std::string kCloseBrace_;
	static const std::string kOpenParenthesis_;
	static const std::string kCloseParenthesis_;
	static const std::vector<std::string> kReservedWords_;
	static const std::vector<std::string> kBinaryOperators;
	static const std::vector<std::string> kTypes_;
	static const std::string kAssignment_;
	static const std::string kPunctuator_;
	static const std::string kLogicalNot_;
};



