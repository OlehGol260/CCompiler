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
const std::vector<std::string> Grammar::kMathBoolOperators_ = {
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
//	"!",   UNARY, need reconsideration
};

const std::vector<std::string> Grammar::kMathOperators_ = {
	"+",
	"-",
	"*",
	"/",
	"%"
};

const std::vector<std::string> Grammar::kBoolOperators_ = {
	"&&",
	"||",
	"==",
	"!=",
	">",
	"<",
//	"!",
};

const std::string Grammar::kAssignment_ = "=";

const std::string Grammar::kPunctuator_ = ";";

const std::string Grammar::kLogicalNot_ = "!";

bool Grammar::IsOperator(const std::string& to_check)
{
	return IsBoolOp(to_check) || IsAssignment(to_check) ||
		IsMathOp(to_check) || IsBracket(to_check) || 
		IsPunctuator(to_check) || IsType(to_check);
}

bool Grammar::IsBoolOp(const std::string& to_check)
{
	return IsInVector(to_check, kBoolOperators_);
}

bool Grammar::IsMathOp(const std::string& to_check)
{
	return IsInVector(to_check, kMathOperators_);
}

bool Grammar::IsReservedWord(const std::string& to_check)
{
	return IsInVector(to_check, kReservedWords_);
}

bool Grammar::IsAssignment(const std::string& to_check)
{
	return kAssignment_ == to_check;
}

bool Grammar::IsBracket(const std::string& to_check)
{
	return IsOpenParenthesis(to_check) ||IsCloseParenthesis(to_check) ||
		IsOpenBrace(to_check) || IsCloseBrace(to_check);
}

bool Grammar::IsPunctuator(const std::string& to_check)
{
	return to_check == kPunctuator_;
}

bool Grammar::IsType(const std::string& to_check)
{
	return IsInVector(to_check, kTypes_);
}

bool Grammar::IsLogicalNot(const std::string& to_check)
{
	return to_check == kLogicalNot_;
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
	if (IsType(to_check)) { return LexemType::kType; }
	if (IsAssignment(to_check)) { return LexemType::kAssignment; }
	if (IsMathOp(to_check) || IsBoolOp(to_check)) { return LexemType::kMathBoolOperator; }
	//if (IsMathOp(to_check)) { return LexemType::kMathOp; }
	//if (IsBoolOp(to_check)) { return LexemType::kBoolOp; }
	if (IsOpenBrace(to_check)) { return LexemType::kOpenBrace; }
	if (IsCloseBrace(to_check)) { return LexemType::kCloseBrace; }
	if (IsOpenParenthesis(to_check)) { return LexemType::kOpenParenthesis; }
	if (IsCloseParenthesis(to_check)) { return LexemType::kCloseParenthesis; }
	if (IsPunctuator(to_check)) { return LexemType::kPunctuator; }
	if (utils::IsNumber(to_check)) { return LexemType::kLiteral; }
	return IsReservedWord(to_check) ? LexemType::kReservedWord : LexemType::kVar;
}
