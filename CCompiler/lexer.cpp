#include "lexer.h"

#include <sstream>

#include "grammar.h"
#include <iostream>
#include "lexem.h"
#include "lexem_reserved.h"

void Lexer::GenerateLexems(const std::string& code)
{
	std::stringstream ss(AddSpaces(code));
	std::string token = "";
	auto i = 0;
	while(ss >> token) {
		auto type = Grammar::GetType(token);
		std::shared_ptr<LexemInterface> lexem = nullptr;
		if (type == LexemType::kReservedWord)
		{
			lexem = std::make_shared<LexemReserved>(type, token, i++);
		}
		else
		{
			lexem = std::make_shared<Lexem>(type, token, i++);
		}
		lexems_.push_back(lexem);
	}
}

std::string Lexer::AddSpaces(const std::string& code)
{
	auto code_size = code.size();
	std::stringstream ss;
	for (size_t i = 0; i < code_size; i++)
	{
		auto str = std::string(1, code.at(i));

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
