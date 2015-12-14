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
	m_lexer_.Parse(code);
	m_parser_.Parse(m_lexer_.lexems());
	m_translator_.Translate(m_parser_.main_context()->roots());
	m_translator_.SaveToFile(filepath);
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
