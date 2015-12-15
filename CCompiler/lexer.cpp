#include "lexer.h"

#include <sstream>
#include <iostream>

#include "grammar.h"
#include "lexeme.h"
#include "lexeme_loop.h"
#include "lexeme_condition.h"
#include "typedefs.h"
#include "lexeme_func.h"

void Lexer::Parse(const std::string& code)
{
	std::stringstream ss(AddSpaces(code));
	std::string token = "";

	while(ss >> token) {
		auto type = Grammar::GetType(token);
		std::shared_ptr<LexemeInterface> lexeme = nullptr;
		if (Grammar::IsDoubleQuote(type))
		{
			std::string str;
			std::getline(ss, str, Grammar::double_qoute().at(0));
			lexeme = std::make_shared<LexemeInterface>(LT::kStringLiteral, str);
		}
		else
		{
			switch (type)
			{
			case LT::kFor:
			case LT::kWhile:
				lexeme = std::make_shared<LexemeLoop>(type, token);
				break;
			case LT::kIf:
				lexeme = std::make_shared<LexemeCondition>(type, token);
				break;
			case LT::kPrint:
			case LT::kSqrt:
				lexeme = std::make_shared<LexemeFunc>(type, token);
				break;
			default:
				lexeme = std::make_shared<Lexeme>(type, token);
			}
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
	
		if (!is_double_quoted_scope)
		{
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
		}
		if (!is_double_quoted_scope && Grammar::IsOperator(str))
		{
			ss << " " << str << " ";
		}
		else
		{
			ss << str;
		}
		if (Grammar::IsDoubleQuote(str))
		{
			is_double_quoted_scope = !is_double_quoted_scope;
		}
	}
	auto a = ss.str();
	return ss.str();
}

void Lexer::print() const
{
	for (auto& lexem : lexems_)
	{
		std::cout << lexem->value() << "\t\t"  << LexemTypeNames.at(lexem->type()) << std::endl;
	}
}