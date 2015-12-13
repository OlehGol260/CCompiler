#pragma once

#include <map>
enum class LexemeType
{
	kLoop,
	kElse,
	kIf,
	kPrint,
	kBinaryOperator,
	kPunctuator,
	kAssignment,
	kVarType,
	kVar,
	kImmediateInteger,
	kImmediateFloat,
	kOpenBrace,
	kCloseBrace,
	kOpenParenthesis,
	kCloseParenthesis,
	kUnknown,
	kLogicalNot,
	kVarDeclaration,
	kDoubleQoute,
	kStringLiteral,
	kFalse,
	kTrue,
	kSqrt,
};

enum class VariableType
{
	kInt,
	kBool,
	kFloat,
};
enum class BracketType
{
	kParentheses,
	kBraces,
};

enum class WhereAttachCh
{
	kLeft,
	kRight,
	kUnknown
};

static const std::map<LexemeType, std::string> LexemTypeNames{
	{ LexemeType::kDoubleQoute, "Double quote"},
	{ LexemeType::kLogicalNot, "Logical not"},
	{ LexemeType::kVarType, "Type_specifier"},
	{ LexemeType::kVar, "Variable" },
	{ LexemeType::kLoop, "Loop"},
	{ LexemeType::kIf, "If statement"},
	{ LexemeType::kElse, "Else statement"},
	{ LexemeType::kLoop, "Loop"},
	{ LexemeType::kBinaryOperator, "Binary operator"},
	{ LexemeType::kOpenBrace, "Open Brace" },
	{ LexemeType::kCloseBrace, "Close Brace" },
	{ LexemeType::kOpenParenthesis, "Open parenthesis" },
	{ LexemeType::kCloseParenthesis, "Close parenthesis" },
	{ LexemeType::kPunctuator, "Semicolon" },
	{ LexemeType::kAssignment, "Assignment_operator" },
	{ LexemeType::kUnknown, "Unknown type" },
	{ LexemeType::kStringLiteral, "String literal"},
	{ LexemeType::kPrint, "Print statement"}
};