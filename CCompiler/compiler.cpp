#include "compiler.h"
#include <fstream>
#include <streambuf>
#include <iostream>

void Compiler::Compile(const std::string& code)
{
	m_lexer_.Parse(code);
	m_parser_.Parse(m_lexer_.lexems());
	m_evaluater_.Evaluate(m_parser_.main_context()->roots());
}

void Compiler::CompileFile(const std::string& filepath)
{
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	Compile(buffer.str());
}

void Compiler::Clear()
{
	m_lexer_.Clear();
	m_parser_.Clear();
	m_evaluater_.Clear();
}
