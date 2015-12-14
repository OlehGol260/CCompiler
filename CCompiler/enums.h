#pragma once

#include <map>
enum class LexemeType
{
	kFor,
	kWhile,
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

static const std::map<VariableType, std::string> VariableTypeStr{
	{VariableType::kInt, "int"},
	{VariableType::kFloat, "float"},
	{VariableType::kBool, "bool"},
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
	{ LexemeType::kIf, "If statement"},
	{ LexemeType::kElse, "Else statement"},
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