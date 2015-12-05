#pragma once

#include <map>
enum class LexemType
{
	kReservedWord,
	kMathBoolOperator,
	kMathOp,
	kBoolOp,
	kPunctuator,
	kAssignment,
	kType,
	kVar,
	kLiteral,
	kOpenBrace,
	kCloseBrace,
	kOpenParenthesis,
	kCloseParenthesis,
	kUnknown,
	kVarDeclaration 
};

enum class BracketType
{
	kParentheses,
	kBrackets,
};

enum class WhereAttachCh
{
	kLeft,
	kRight,
	kUnknown
};

static const std::map<LexemType, std::string> LexemTypeNames{
	{ LexemType::kType, "Type_specifier"},
	{ LexemType::kVar, "Variable" },
	{ LexemType::kReservedWord, "Reserved_word" },
	{ LexemType::kMathBoolOperator, "Math or bool operator"},
	{ LexemType::kMathOp, "Mathematic_operator" },
	{ LexemType::kBoolOp, "Boolean_operator" },
	{ LexemType::kLiteral, "Literal" },
	{ LexemType::kOpenBrace, "Open Brace" },
	{ LexemType::kCloseBrace, "Close Brace" },
	{ LexemType::kOpenParenthesis, "Open parenthesis" },
	{ LexemType::kCloseParenthesis, "Close parenthesis" },
	{ LexemType::kPunctuator, "Semicolon" },
	{ LexemType::kAssignment, "Assignment_operator" },
	{ LexemType::kUnknown, "Unknown type" },
};