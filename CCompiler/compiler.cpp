#include "compiler.h"
#include <fstream>
#include <streambuf>


void Compiler::Compile(const std::string& code)
{
	InnerCompile(code);
	m_translator_.Print();
}

void Compiler::Compile(const std::string& filepath, const std::string& code)
{
	InnerCompile(code);
	m_translator_.SaveToFile(filepath);
}

void Compiler::InnerCompile(const std::string& code)
{
	m_lexer_.Parse(code);
	m_parser_.Parse(m_lexer_.lexems());
	m_evaluater_.Evaluate(m_parser_.main_context()->roots());
	m_translator_.Translate(m_parser_.main_context()->roots());
}
