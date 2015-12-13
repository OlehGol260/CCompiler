#pragma once
#include "lexer.h"
#include "parser.h"
#include "evaluater.h"

class Compiler
{
public:
	void Compile(const std::string& code);
	Compiler() = default;

	Compiler(const Compiler&) = delete;
	void operator=(const Compiler&) = delete;
	
private:
	Lexer m_lexer_;
	Parser m_parser_;
	Evaluater m_evaluater_;
};
