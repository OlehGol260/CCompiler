#pragma once

#include <vector>

#include "lexeme.h"

/*!
	Holds all allowed terminals in parsed text and give API to check if 
	certain string or type exists as it has to 
*/
class Grammar
{
public:
	Grammar() = delete;
	~Grammar() = default;
	Grammar(const Grammar&) = delete;
	Grammar& operator=(const Grammar&) = delete;

	static VariableType LexemeTypeToVariableType(LexemeType);

	template<typename T>
	static bool IsOperator(const T& t)
	{
		return IsBinaryOperator(t) || IsBracket(t)||  IsLogicalNot(t) ||
			IsPunctuator(t) || IsVarType(t) || IsDoubleQuote(t);
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

	static bool IsLexemeFunc(const LexemeType& lt);

	static std::string GetTextByOperation(const std::string& op);
	static bool IsAssignment(const std::string& to_check);
	static bool IsPunctuator(const std::string& to_check);
	static bool IsVarType(const std::string& to_check);
	static bool IsLogicalNot(const std::string& to_check);
	static bool IsBinaryOperator(const std::string& to_check);
	static bool IsOpenBrace(const std::string& to_check);
	static bool IsCloseBrace(const std::string& to_check);
	static bool IsOpenParenthesis(const std::string& to_check);
	static bool IsCloseParenthesis(const std::string& to_check);
	static bool IsLoop(const std::string& str);
	static bool IsIf(const std::string& str);
	static bool IsElse(const std::string& str);
	static bool IsPrint(const std::string& str);
	static bool IsFalse(const std::string& str);
	static bool IsTrue(const std::string& str);
	static bool IsSqrt(const std::string& str);
	static bool IsFor(const std::string& str);
	static bool IsWhile(const std::string& str);


	static bool IsDoubleQuote(const LexemeType& t);
	static bool IsSqrt(const LexemeType& t);
	static bool IsDoubleQuote(const std::string& to_check);
	static bool IsBinaryOperator(const LexemeType& t);
	static bool IsReservedWord(const LexemeType& t);
	static bool IsAssignment(const LexemeType& t);
	static bool IsPunctuator(const LexemeType& t);
	static bool IsVarType(const LexemeType& t);
	static bool IsLogicalNot(const LexemeType& t);
	static bool IsOpenBrace(const LexemeType& t);
	static bool IsCloseBrace(const LexemeType& t);
	static bool IsOpenParenthesis(const LexemeType& t);
	static bool IsCloseParenthesis(const LexemeType& t);
	static bool IsVariable(const LexemeType& t);
	static bool IsLoop(const LexemeType& t);
	static bool IsIf(const LexemeType& t);

	static bool IsFor(const LexemeType& t);
	static bool IsWhile(const LexemeType& t);
	static bool IsElse(const LexemeType& t);
	static bool IsPrint(const LexemeType& t);
	static bool IsFalse(const LexemeType& t);
	static bool IsTrue(const LexemeType& t);
	static LexemeType GetType(const std::string& to_check);

	static auto& open_parenthesis() { return kOpenParenthesis_; }
	static auto& close_parenthesis() { return kCloseParenthesis_; }
	static auto& open_brace() { return kOpenBrace_; }
	static auto& close_brace() { return kCloseBrace_; }
	static auto& false_() { return kFalse_; }
	static auto& true_() { return kTrue_; }
	static auto& sqrt() { return kSqrt_; }

	static auto& double_qoute() { return kDoubleQuote_; }
	static auto& if_st() { return kIf_; }
	static auto& else_st() { return kElse_; }
	static auto& print() { return kPrint_; }
	static auto& for_() { return kFor_; }
	static auto& while_() { return kWhile_; }
	
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
	static const std::string kIf_;
	static const std::string kElse_;
	static const std::string kPrint_;
	static const std::string kFor_;
	static const std::string kWhile_;
	static const std::vector<std::string> kBinaryOperators;
	static const std::vector<std::string> kTypes_;
	static const std::string kAssignment_;
	static const std::string kPunctuator_;
	static const std::string kLogicalNot_;
	static const std::string kDoubleQuote_;
	static const std::string kFalse_;
	static const std::string kTrue_;
	static const std::string kSqrt_;
	
	static const std::map <std::string, std::string> kBinary_op_to_text_;
};



