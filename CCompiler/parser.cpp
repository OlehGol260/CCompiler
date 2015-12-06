#include "parser.h"

#include <iostream>
#include <regex>

#include "err_msg.h"
#include "grammar.h"
#include "statement_parser.h"
#include "control_flow_parser.h"

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

	auto is_next_token_invalid = false;

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
		case LT::kReservedWord:
			if (Grammar::IsOpenParenthesis(next_token_type))
		case LT::kBinaryOperator:
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kLiteral:
			case LT::kOpenParenthesis:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
			///////////////////////////////////////////
		case LT::kPunctuator:
			state = (open_parenthesis_count || open_brace_count) ? next_token_type : LT::kUnknown;
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
				is_next_token_invalid = true;
			}
			break;
			///////////////////////////////////////////
		case LT::kVarType:
			if (Grammar::IsVariable(next_token_type))
			{
				state = next_token_type;
			}
			else
			{
				is_next_token_invalid = true;
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
			case LT::kBinaryOperator:
			case LT::kCloseParenthesis:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
				break;
			}
			break;
			///////////////////////////////////////////		
		case LT::kLiteral:
			switch (next_token_type)
			{
			case LT::kCloseParenthesis:
			case LT::kPunctuator:
			case LT::kBinaryOperator:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
			///////////////////////////////////////////
		case LT::kOpenBrace:
			open_brace_count++;
			switch (next_token_type)
			{
			case LT::kVarType:
			case LT::kVar:
			case LT::kOpenBrace:
			case LT::kReservedWord:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
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
				ErrMessage::AbortInvalidNumberBrackets(BracketType::kBraces);
			}
			break;
			///////////////////////////////////////////
		case LT::kOpenParenthesis:
			open_parenthesis_count++;
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kOpenParenthesis:
			case LT::kLiteral:
			case LT::kVarType:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
			///////////////////////////////////////////
		case LT::kCloseParenthesis:
		{
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
			case LT::kBinaryOperator:
				if (open_parenthesis_count < 0) { signalize_inv_br_numb = true; }
				break;
			default:
				is_next_token_invalid = true;
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
			case LT::kVarType:
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

		if (is_next_token_invalid)
		{
			ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
		}

		current_lexem->set_level(open_parenthesis_count);
		lexems_block.push_back(current_lexem);

		if (state == LT::kUnknown && !lexems_block.empty())
		{
			if (Grammar::IsReservedWord(lexems_block.front()->type()))
			{
				auto st = ControlFlowParser::Parse(lexems_block);
			}
			else
			{
				auto st = StatementParser::Parse(lexems_block);
				st->Print();
			}
			lexems_block.clear();
		}

	}
}

bool Parser::IsVariableValid(const std::string& var_name)
{
	std::regex validater("^[a-zA-Z_][a-zA-Z0-9_]*$");
	return std::regex_match(var_name, validater);
}
