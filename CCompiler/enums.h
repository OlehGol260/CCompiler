#pragma once

#include <map>
#include <string>
enum class LexemType
{
	kReservedWord,
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
	kUnknown
};

static const std::map<LexemType, std::string> LexemTypeNames{
	{ LexemType::kType, "Type_specifier"},
	{ LexemType::kVar, "Variable" },
	{ LexemType::kReservedWord, "Reserved_word" },
	{ LexemType::kMathOp, "Mathematic_operator" },
	{ LexemType::kBoolOp, "Boolean_operator" },
	//{ LexemType::k, "Type specifier" },
	{ LexemType::kLiteral, "Literal" },
	{ LexemType::kOpenBrace, "Open Brace" },
	{ LexemType::kCloseBrace, "Close Brace" },
	{ LexemType::kOpenParenthesis, "Open parenthesis" },
	{ LexemType::kCloseParenthesis, "Close parenthesis" },
	{ LexemType::kPunctuator, "Semicolon" },
	{ LexemType::kAssignment, "Assignment_operator" },
	{ LexemType::kUnknown, "Unknown type" },
};