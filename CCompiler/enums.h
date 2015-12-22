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
	kUnaryOperator,
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

enum class PrintType
{
	kVariable,
	kString
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
