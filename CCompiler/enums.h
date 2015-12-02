#pragma once

#include <map>
#include <string>
enum class LexemType
{
	kType,
	kVar,
	kReservedWord,
	kMathOp,
	kBoolOp,
	kExpression,
	kLiteral,
	kBracket,
	kPunctuator,
	kAssignment,
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
	{ LexemType::kBracket, "Bracket" },
	{ LexemType::kPunctuator, "Semicolon" },
	{ LexemType::kAssignment, "Assignment_operator" },
	{ LexemType::kUnknown, "Unknown type" },
};