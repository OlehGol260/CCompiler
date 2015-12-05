﻿#include "parser.h"

#include <iostream>
#include <regex>

#include "err_msg.h"
#include "grammar.h"
#include "statement_parser.h"


//outer_state = current_lexem_type == LexemType::kReservedWord ? State::kReservedWordPart : State::kSimplePart;
void Parser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	main_context_ = std::shared_ptr<Context>(0);

	auto state = LT::kUnknown;
	std::shared_ptr<LexemInterface> next_token = nullptr;
	auto next_token_type = LT::kUnknown; //type of next lexem determine the next state, as it may be ambiguouse
	lexems_interface_vec lexems_block;

	auto open_parenthesis_count = 0;
	auto open_brace_count = 0;

	lexems_block.clear();
	auto lexems_size = lexems.size();


	for (size_t i = 0; i < lexems_size; i++)
	{
		auto current_lexem = lexems.at(i);
		auto current_lexem_type = current_lexem->type();

		if (i < lexems_size - 1)
		{
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
		}
		else
		{
			next_token_type = LT::kUnknown;
		}
		//there is no kReservedWord, becase kUnknow case determine how to begin to parse
		switch (state)
		{
		case LT::kMathBoolOperator:
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kLiteral:
			case LT::kOpenParenthesis:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kPunctuator:
			switch (next_token_type)
			{
			case LT::kCloseParenthesis:
			case LT::kCloseBrace:
				state = next_token_type;
				break;
			default:
				state = LT::kUnknown;
			}

			break;
			///////////////////////////////////////////
		case LT::kAssignment:
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kLiteral:
			case LT::kOpenParenthesis:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kType:
			if (next_token_type == LT::kVar)
			{
				state = next_token_type;
			}
			else
			{
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kVar:
			if (!IsVariableValid(current_lexem->value()))
			{
				ErrMessage::AbortInvalidVariable(current_lexem->value());
			}
			switch (next_token_type)
			{
			case LT::kPunctuator:
			case LT::kAssignment:
			case LT::kMathBoolOperator:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
				break;
			}
			break;
			///////////////////////////////////////////		
		case LT::kLiteral:
			switch (next_token_type)
			{
			case LT::kCloseParenthesis:
			case LT::kPunctuator:
			case LT::kMathBoolOperator:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kOpenBrace:
			open_brace_count++;
			switch (next_token_type)
			{
			case LT::kType:
			case LT::kVar:
			case LT::kOpenBrace:
			case LT::kReservedWord:
				state = next_token_type;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}	
			break;
			///////////////////////////////////////////
		case LT::kCloseBrace:
			open_brace_count--;
			if (open_brace_count == 0)
			{
				state = LT::kUnknown;
			}
			else if (open_brace_count > 0 && i < lexems_size - 1) //TODO: test here
			{
				state = next_token_type;
			}
			else
			{
				ErrMessage::AbortMsg("Number of open and closed braces doesn't match");
			}
			break;
			///////////////////////////////////////////
		case LT::kOpenParenthesis:
			if (current_lexem_type != LT::kOpenParenthesis)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: '('");
			}
			open_parenthesis_count++;
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kOpenParenthesis:
			case LT::kLiteral:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kCloseParenthesis:
		{
			if (current_lexem_type != LT::kCloseParenthesis)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: ')'");
			}
			open_parenthesis_count--;
			auto signalize_inv_br_numb = false;

			switch (next_token_type)
			{
			case LT::kCloseParenthesis:
				break;
			case LT::kOpenBrace:
			case LT::kPunctuator:
				if (open_parenthesis_count != 0) { signalize_inv_br_numb = true; }
				break;
			case LT::kMathBoolOperator:
				if (open_parenthesis_count < 0) { signalize_inv_br_numb = true; }
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			if (signalize_inv_br_numb)
			{
				ErrMessage::AbortInvalidNumberBrackets(BracketType::kParentheses);
			}
			state = next_token_type;
			break;
		}

		case LT::kUnknown:	// new chunk will be checked here
			switch (current_lexem_type)
			{
			case LT::kReservedWord:
				state = LT::kOpenParenthesis;
				break;
			case LT::kType:
				state = LT::kVar;
				break;
			case LT::kVar:
				state = LT::kAssignment;
				break;
			default:
				ErrMessage::AbortInvalidToken(current_lexem->value());
			}
			break;
		}
		current_lexem->set_level(open_parenthesis_count);
		//TODO: WILD WORKAROUND
		if (current_lexem_type != LT::kOpenParenthesis && current_lexem_type != LT::kCloseParenthesis)
		{
			lexems_block.push_back(current_lexem);
		}
		
		if (state == LT::kUnknown && !lexems_block.empty())
		{
			if (lexems_block.at(0)->type() == LT::kReservedWord)
			{
				ParseComplex(lexems_block);
			}
			else
			{
				for (auto &a : lexems_block)
				{
					std::cout << a->value() << "\t" << a->level() << std::endl;
				}
				auto st = StatementParser::Parse(lexems_block);
				st->Print();
				auto a = 0;
				//ParseStatement(lexems_block);
			}
			lexems_block.clear();
		}

	}
}

void Parser::ParseComplex(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	for (auto& lexem : lexems)
	{
		std::cout << lexem->value() << "\t\t" << LexemTypeNames.at(lexem->type()) << std::endl;
	}
}

bool Parser::IsVariableValid(const std::string& var_name)
{
	std::regex validater("^[a-zA-Z_][a-zA-Z0-9_]*$");
	return std::regex_match(var_name, validater);
}
