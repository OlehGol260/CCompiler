#include "grammar.h"
#include "utils.h"

const std::string Grammar::kOpenBrace_ = "{";
const std::string Grammar::kCloseBrace_ = "}";
const std::string Grammar::kOpenParenthesis_ = "(";
const std::string Grammar::kCloseParenthesis_ = ")";

const std::vector<std::string> Grammar::kReservedWords_ = {
	"for",
	"while",
	"if",
	"else",
	"print",
};

const std::vector<std::string> Grammar::kTypes_ = {
	"int",
	"float",
	"bool",
};

//ORDER MATTERS
const std::vector<std::string> Grammar::kBinaryOperators = {
	"=",
	"||",
	"&&",
	"==",
	"!=",
	">",
	"<",
	"+",
	"-",
	"*",
	"/",
	"%",
};

const std::string Grammar::kAssignment_ = "=";

const std::string Grammar::kPunctuator_ = ";";

const std::string Grammar::kLogicalNot_ = "!";

bool Grammar::IsReservedWord(const std::string& to_check)
{
	return IsInVector(to_check, kReservedWords_);
}

bool Grammar::IsAssignment(const std::string& to_check)
{
	return kAssignment_ == to_check;
}

bool Grammar::IsPunctuator(const std::string& to_check)
{
	return to_check == kPunctuator_;
}

bool Grammar::IsVarType(const std::string& to_check)
{
	return IsInVector(to_check, kTypes_);
}

bool Grammar::IsLogicalNot(const std::string& to_check)
{
	return to_check == kLogicalNot_;
}

bool Grammar::IsBinaryOperator(const std::string& to_check)
{
	return IsInVector(to_check, kBinaryOperators);
}

bool Grammar::IsBinaryOperator(const LexemType& t)
{
	//TODO: WORKAROUND, may be reconsidered
	return t == LexemType::kBinaryOperator || t == LexemType::kAssignment;
}

bool Grammar::IsReservedWord(const LexemType& t)
{
	return t == LexemType::kReservedWord;
}

bool Grammar::IsAssignment(const LexemType& t)
{
	return t == LexemType::kAssignment;
}

bool Grammar::IsPunctuator(const LexemType& t)
{
	return t == LexemType::kPunctuator;
}

bool Grammar::IsVarType(const LexemType& t)
{
	return t == LexemType::kVarType;
}

bool Grammar::IsLogicalNot(const LexemType& t)
{
	return t == LexemType::kLogicalNot;
}

bool Grammar::IsOpenBrace(const LexemType& t)
{
	return t == LexemType::kOpenBrace;
}

bool Grammar::IsCloseBrace(const LexemType& t)
{
	return t == LexemType::kCloseBrace;
}

bool Grammar::IsOpenParenthesis(const LexemType& t)
{
	return t == LexemType::kOpenParenthesis;
}

bool Grammar::IsCloseParenthesis(const LexemType& t)
{
	return t == LexemType::kCloseParenthesis;
}

bool Grammar::IsVariable(const LexemType& t)
{
	return t == LexemType::kVar;
}

bool Grammar::IsOpenBrace(const std::string& to_check)
{
	return to_check == kOpenBrace_;
}

bool Grammar::IsCloseBrace(const std::string& to_check)
{
	return to_check == kCloseBrace_;
}

bool Grammar::IsOpenParenthesis(const std::string& to_check)
{
	return to_check == kOpenParenthesis_;
}

bool Grammar::IsCloseParenthesis(const std::string& to_check)
{
	return to_check == kCloseParenthesis_;
}

LexemType Grammar::GetType(const std::string& to_check)
{
	if (IsVarType(to_check)) { return LexemType::kVarType; }
	if (IsAssignment(to_check)) { return LexemType::kAssignment; }
	if (IsBinaryOperator(to_check)) { return LexemType::kBinaryOperator; }
	if (IsOpenBrace(to_check)) { return LexemType::kOpenBrace; }
	if (IsCloseBrace(to_check)) { return LexemType::kCloseBrace; }
	if (IsOpenParenthesis(to_check)) { return LexemType::kOpenParenthesis; }
	if (IsCloseParenthesis(to_check)) { return LexemType::kCloseParenthesis; }
	if (IsPunctuator(to_check)) { return LexemType::kPunctuator; }
	if (utils::IsNumber(to_check)) { return LexemType::kLiteral; }
	return IsReservedWord(to_check) ? LexemType::kReservedWord : LexemType::kVar;
}
