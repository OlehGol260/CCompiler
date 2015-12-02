#include "grammar.h"
#include "utils.h"

const Bracket Grammar::kParentheses_ = { "(", ")" };
const Bracket Grammar::kBraces_ = { "{", "}" };

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
};

const std::vector<std::string> Grammar::kMathOperators_ = {
	"+",
	"-",
	"*",
	"/",
	"%"
};

const std::vector<std::string> Grammar::kBoolOperators_ = {
	"==",
	"!=",
	">",
	"<",
	"&&",
	"||",
	"!",
};

const std::string Grammar::kAssignment_ = "=";

const std::string Grammar::kPunctuator_ = ";";

bool Grammar::IsOperator(const std::string& to_check)
{
	return IsAssignment(to_check) || IsBoolOp(to_check) || 
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
	return kParentheses_.IsOpen(to_check) || kParentheses_.IsClose(to_check) ||
		kBraces_.IsOpen(to_check) || kBraces_.IsClose(to_check);
}

bool Grammar::IsPunctuator(const std::string& to_check)
{
	return to_check == kPunctuator_;
}

bool Grammar::IsType(const std::string& to_check)
{
	return IsInVector(to_check, kTypes_);
}

LexemType Grammar::GetType(const std::string& to_check)
{
	if (IsType(to_check)) { return LexemType::kType; }
	if (IsAssignment(to_check)) { return LexemType::kAssignment; }
	if (IsMathOp(to_check)) { return LexemType::kMathOp; }
	if (IsBoolOp(to_check)) { return LexemType::kBoolOp; }
	if (IsBracket(to_check)) { return LexemType::kBracket; }
	if (IsPunctuator(to_check)) { return LexemType::kPunctuator; }
	if (utils::IsNumber(to_check)) { return LexemType::kLiteral; }
	return IsReservedWord(to_check) ? LexemType::kReservedWord : LexemType::kVar;
}
