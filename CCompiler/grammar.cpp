#include "grammar.h"
#include "utils.h"
#include "err_msg.h"
const std::string Grammar::kOpenBrace_ = "{";
const std::string Grammar::kCloseBrace_ = "}";
const std::string Grammar::kOpenParenthesis_ = "(";
const std::string Grammar::kCloseParenthesis_ = ")";

const std::string Grammar::kFor_ = "for";
const std::string Grammar::kWhile_ = "while";



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
const std::string Grammar::kDoubleQuote_ = "\"";
const std::string Grammar::kFalse_ = "false";
const std::string Grammar::kTrue_ = "true";
const std::string Grammar::kSqrt_ = "sqrt";

const std::string Grammar::kIf_ = "if";
const std::string Grammar::kElse_ = "else";
const std::string Grammar::kPrint_ = "print";

const std::string Grammar::kAssignment_ = "=";

const std::string Grammar::kPunctuator_ = ";";

const std::string Grammar::kLogicalNot_ = "!";


VariableType Grammar::LexemeTypeToVariableType(LexemeType lt)
{
	switch(lt)
	{
	case LexemeType::kImmediateInteger:
		return VariableType::kInt;
	case LexemeType::kImmediateFloat:
		return VariableType::kFloat;
	case LexemeType::kTrue:
	case LexemeType::kFalse:
		return VariableType::kBool;
	default:
		ErrMessage::AbortMsg("Try to infer unknown variable type out of lexeme type");
	}
	return VariableType::kInt; //Workaround to shut the warning. This statement never is executed as an AbortMsg terminates the program
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

bool Grammar::IsBinaryOperator(const LexemeType& t)
{
	//TODO: WORKAROUND, may be reconsidered
	return t == LexemeType::kBinaryOperator || t == LexemeType::kAssignment;
}

bool Grammar::IsReservedWord(const LexemeType& t)
{
	return IsFor(t) || IsWhile(t) || IsPrint(t) || IsIf(t) || IsElse(t);
}

bool Grammar::IsAssignment(const LexemeType& t)
{
	return t == LexemeType::kAssignment;
}

bool Grammar::IsPunctuator(const LexemeType& t)
{
	return t == LexemeType::kPunctuator;
}

bool Grammar::IsVarType(const LexemeType& t)
{
	return t == LexemeType::kVarType;
}

bool Grammar::IsLogicalNot(const LexemeType& t)
{
	return t == LexemeType::kLogicalNot;
}

bool Grammar::IsOpenBrace(const LexemeType& t)
{
	return t == LexemeType::kOpenBrace;
}

bool Grammar::IsCloseBrace(const LexemeType& t)
{
	return t == LexemeType::kCloseBrace;
}

bool Grammar::IsOpenParenthesis(const LexemeType& t)
{
	return t == LexemeType::kOpenParenthesis;
}

bool Grammar::IsCloseParenthesis(const LexemeType& t)
{
	return t == LexemeType::kCloseParenthesis;
}

bool Grammar::IsVariable(const LexemeType& t)
{
	return t == LexemeType::kVar;
}

bool Grammar::IsLoop(const LexemeType& t)
{
	return IsFor(t) || IsWhile(t);
}

bool Grammar::IsIf(const LexemeType& t)
{
	return t == LexemeType::kIf;
}

bool Grammar::IsFor(const LexemeType& t)
{
	return t == LexemeType::kFor;
}

bool Grammar::IsWhile(const LexemeType& t)
{
	return t == LexemeType::kWhile;
}

bool Grammar::IsElse(const LexemeType& t)
{
	return t == LexemeType::kElse;
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

bool Grammar::IsLoop(const std::string& str)
{
	return IsFor(str) || IsWhile(str);
}

bool Grammar::IsIf(const std::string& str)
{
	return str == kIf_;
}

bool Grammar::IsElse(const std::string& str)
{
	return str == kElse_;
}

bool Grammar::IsFalse(const std::string& str)
{
	return str == kFalse_;
}

bool Grammar::IsTrue(const std::string& str)
{
	return str == kTrue_;
}

bool Grammar::IsSqrt(const std::string& str)
{
	return str == kSqrt_;
}

bool Grammar::IsFor(const std::string& str)
{
	return str == kFor_;
}

bool Grammar::IsWhile(const std::string& str)
{
	return str == kWhile_;
}

bool Grammar::IsPrint(const std::string& str)
{
	return str == kPrint_;
}

bool Grammar::IsDoubleQuote(const LexemeType& t)
{
	return t == LexemeType::kDoubleQoute;
}

bool Grammar::IsSqrt(const LexemeType& t)
{
	return t == LexemeType::kSqrt;
}

bool Grammar::IsDoubleQuote(const std::string& to_check)
{
	return to_check == kDoubleQuote_;
}

bool Grammar::IsPrint(const LexemeType& t)
{
	return t == LexemeType::kPrint;
}

bool Grammar::IsFalse(const LexemeType& t)
{
	return t == LexemeType::kFalse;
}

bool Grammar::IsTrue(const LexemeType& t)
{
	return t == LexemeType::kTrue;
}

LexemeType Grammar::GetType(const std::string& str)
{
	if (IsSqrt(str)) { return LexemeType::kSqrt; }
	if (IsTrue(str)) { return LexemeType::kTrue; }
	if (IsFalse(str)) { return LexemeType::kFalse; }
	if (IsVarType(str)) { return LexemeType::kVarType; }
	if (IsAssignment(str)) { return LexemeType::kAssignment; }
	if (IsBinaryOperator(str)) { return LexemeType::kBinaryOperator; }
	if (IsOpenBrace(str)) { return LexemeType::kOpenBrace; }
	if (IsCloseBrace(str)) { return LexemeType::kCloseBrace; }
	if (IsOpenParenthesis(str)) { return LexemeType::kOpenParenthesis; }
	if (IsCloseParenthesis(str)) { return LexemeType::kCloseParenthesis; }
	if (IsPunctuator(str)) { return LexemeType::kPunctuator; }
	if (utils::IsNumber(str))
	{
		return str.find(".") != std::string::npos ? LexemeType::kImmediateFloat : LexemeType::kImmediateInteger;

	}
	if (IsDoubleQuote(str)) { return LexemeType::kDoubleQoute; }
	if (IsIf(str)) { return LexemeType::kIf; }
	if (IsElse(str)) { return LexemeType::kElse; }
	if (IsFor(str)) { return LexemeType::kFor; }
	if (IsWhile(str)) { return LexemeType::kWhile; }
	
	if (IsPrint(str)) { return LexemeType::kPrint; }
	return LexemeType::kVar;
}
