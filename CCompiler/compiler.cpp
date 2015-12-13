#include "compiler.h"

void Compiler::Compile(const std::string& code)
{
	m_lexer_.Parse(code);

	m_parser_.Parse(m_lexer_.lexems());
	m_evaluater_.Evaluate(m_parser_.main_context()->roots());
}
