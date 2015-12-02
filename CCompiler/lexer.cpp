#include "lexer.h"

#include <sstream>

#include "grammar.h"
#include <iostream>

void Lexer::GenerateLexems(const std::string& code)
{
	std::stringstream ss(AddSpaces(code));
	std::string token = "";
	
	while(ss >> token) {
		lexems_.push_back(std::make_shared<Lexem>(Grammar::GetType(token), token));
	}
}

std::string Lexer::AddSpaces(const std::string& code)
{
	auto code_size = code.size();
	std::stringstream ss;
	for (size_t i = 0; i < code_size; i++)
	{
		auto str = std::string(1, code.at(i));
	//	if (isspace(str.at(0))) { continue; }

		if (Grammar::IsOperator(str))
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
