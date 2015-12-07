#pragma once

#include <map>
enum class LexemType
{
	kReservedWord,
	kBinaryOperator,
	kPunctuator,
	kAssignment,
	kVarType,
	kVar,
	kLiteral,
	kOpenBrace,
	kCloseBrace,
	kOpenParenthesis,
	kCloseParenthesis,
	kUnknown,
	kLogicalNot,
	kVarDeclaration,
	kDoubleQoute,
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

static const std::map<LexemType, std::string> LexemTypeNames{
	{ LexemType::kDoubleQoute, "Double quote"},
	{ LexemType::kLogicalNot, "Logical not"},
	{ LexemType::kVarType, "Type_specifier"},
	{ LexemType::kVar, "Variable" },
	{ LexemType::kReservedWord, "Reserved_word" },
	{ LexemType::kBinaryOperator, "Binary operator"},
	{ LexemType::kLiteral, "Literal" },
	{ LexemType::kOpenBrace, "Open Brace" },
	{ LexemType::kCloseBrace, "Close Brace" },
	{ LexemType::kOpenParenthesis, "Open parenthesis" },
	{ LexemType::kCloseParenthesis, "Close parenthesis" },
	{ LexemType::kPunctuator, "Semicolon" },
	{ LexemType::kAssignment, "Assignment_operator" },
	{ LexemType::kUnknown, "Unknown type" },
};