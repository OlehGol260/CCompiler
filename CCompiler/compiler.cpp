#include "compiler.h"
#include <fstream>
#include <streambuf>


void Compiler::Compile(const std::string& code)
{
	m_lexer_.Parse(code);
	m_parser_.Parse(m_lexer_.lexems());
	m_evaluater_.Evaluate(m_parser_.main_context()->roots());
}

void Compiler::Compile(const std::string& filepath, const std::string& code)
{
	Compile(code);
	m_translator_.Translate(m_parser_.main_context()->roots());
	m_translator_.SaveToFile(filepath);
}

void Compiler::CompileFile(const std::string& filepath)
{
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	Compile(buffer.str());
}