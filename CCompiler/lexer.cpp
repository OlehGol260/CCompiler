#include "lexer.h"

#include <sstream>

#include "grammar.h"
#include <iostream>
#include "lexeme.h"
#include "lexeme_loop.h"

void Lexer::GenerateLexems(const std::string& code)
{
	std::stringstream ss(AddSpaces(code));
	std::stringstream ss_str_literal;
	auto is_double_quoted_scope = false;
	std::string token = "";
	auto i = 0;
	while(ss >> token) {
		auto type = Grammar::GetType(token);
		if (Grammar::IsDoubleQuote(type))
		{
			is_double_quoted_scope = !is_double_quoted_scope;
			continue;
		}
		if (is_double_quoted_scope)
		{
			ss_str_literal << token;
			continue;
		}
		std::shared_ptr<LexemeInterface> lexeme = nullptr;
		if (ss_str_literal)
		{
			lexeme = std::make_shared<Lexeme>(LexemeType::kStringLiteral, ss_str_literal.str());
		}
		if (Grammar::IsReservedWord(type))
		{
			lexeme = std::make_shared<LexemeLoop>(type, token);
		}
		else
		{
			lexeme = std::make_shared<Lexeme>(type, token);
		}
		lexems_.push_back(lexeme);
	}
}

std::string Lexer::AddSpaces(const std::string& code)
{
	auto code_size = code.size();
	std::stringstream ss;
	auto is_double_quoted_scope = false;
	for (size_t i = 0; i < code_size; i++)
	{
		auto str = std::string(1, code.at(i));

		//check for operators which contatin 2 symbols : != and ==
		if (Grammar::IsAssignment(str) || Grammar::IsLogicalNot(str))
		{
			auto next_str = std::string(1, code.at(i + 1));
			if (Grammar::IsAssignment(next_str))
			{
				str += next_str;
				i++;
			}	
		}
		if (Grammar::IsDoubleQuote(str))
		{
			is_double_quoted_scope = !is_double_quoted_scope;
		}

		if (!is_double_quoted_scope && Grammar::IsOperator(str))
		{
			ss << " " << str << " ";
		}
		else
		{
			ss << str;
		}
	}
	auto str = ss.str();
	return ss.str();
}

void Lexer::print() const
{
	for (auto& lexem : lexems_)
	{
		std::cout << lexem->value() << "\t\t"  << LexemTypeNames.at(lexem->type()) << std::endl;
	}
}
