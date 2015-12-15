#pragma once
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "translator.h"
class Compiler
{
public:
	void Compile(const std::string& code);
	void Compile(const std::string& filepath, const std::string& code);
	void CompileFile(const std::string& filepath);
	Compiler() = default;

	Compiler(const Compiler&) = delete;
	void operator=(const Compiler&) = delete;

private:
	Lexer m_lexer_;
	Parser m_parser_;
	Evaluator m_evaluater_;
	Translator m_translator_;
};
