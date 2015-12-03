#include "parser.h"

#include <iostream>
#include <regex>

#include "err_msg.h"
using LT = LexemType;

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

		//there is no kReservedWord, becase kUnknow case determine how to begin to parse
		switch (state)
		{
		case LT::kMathOp:
		case LT::kBoolOp:
			if (current_lexem_type != LT::kMathOp && current_lexem_type != LT::kBoolOp)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: bool operator");
			}
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kLiteral:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;		
			///////////////////////////////////////////
		case LT::kPunctuator:
			if (current_lexem_type != LT::kPunctuator)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: ';'");
			}
			next_token_type = lexems.at(i + 1)->type();
			switch(next_token_type)
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
			if (current_lexem_type != LT::kAssignment)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: '='");
			}
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kLiteral:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kType:
			if (current_lexem_type != LT::kType)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: type specifier");
			}
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
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
			if (current_lexem_type != LT::kVar)
			{
				ErrMessage::AbortMsg("Invalid token met: " + current_lexem->value() + "Variable expected");
			}
			if (!IsVariableValid(current_lexem->value()))
			{
				ErrMessage::AbortInvalidVariable(current_lexem->value());
			}
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch (next_token_type)
			{
			case LT::kPunctuator:
			case LT::kAssignment:
			case LT::kMathOp:
			case LT::kBoolOp:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
				break;
			}
			break;
			///////////////////////////////////////////		
		case LT::kLiteral: 
			if (current_lexem_type != LT::kLiteral)
			{
				ErrMessage::AbortMsg("Invalid token met: " + current_lexem->value() + "Literal expected");
			}
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch(next_token_type)
			{
			case LT::kCloseParenthesis:
			case LT::kPunctuator:
			case LT::kMathOp:
			case LT::kBoolOp:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kOpenBrace: 
			if (current_lexem_type != LT::kOpenBrace)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: '{'");
			}
			open_brace_count++;
			state = lexems.at(i + 1)->type();
			break;
			///////////////////////////////////////////
		case LT::kCloseBrace:
			if (current_lexem_type != LT::kCloseBrace)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: '}'");
			}
			open_brace_count--;
			if (open_brace_count == 0)
			{
				state = LT::kUnknown;
			} else if (open_brace_count > 0 && i < lexems_size - 1) //TODO: test here
			{
				state = lexems.at(i + 1)->type();
			} else
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
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kOpenParenthesis:
				state = next_token_type;
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			break;
			///////////////////////////////////////////
		case LT::kCloseParenthesis: 
			if (current_lexem_type != LT::kCloseParenthesis)
			{
				ErrMessage::AbortMsg("Meet: " + current_lexem->value() + " Required: ')'");
			}
			open_parenthesis_count--;
			next_token = lexems.at(i + 1);
			next_token_type = next_token->type();
			switch (next_token_type)
			{
			case LT::kOpenBrace:
				if (open_parenthesis_count != 0) {
					ErrMessage::AbortInvalidNumberBrackets(BracketType::kParentheses);
				}
				break;
			case LT::kMathOp:
			case LT::kBoolOp:
				if (open_parenthesis_count < 0) {
					ErrMessage::AbortInvalidNumberBrackets(BracketType::kParentheses);
				}
				break;
			default:
				ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
			}
			state = next_token_type;
			break;
		case LT::kUnknown:	// new chunk will be checked here
			
			switch(current_lexem_type)
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

		lexems_block.push_back(current_lexem);
		if (state == LT::kUnknown && !lexems_block.empty())
		{
			if (lexems_block.at(0)->type() == LT::kReservedWord)
			{
				ParseComplex(lexems_block);
			}
			else
			{
				ParseStatement(lexems_block);
			}
			lexems_block.clear();
		}
		
	}
}


void Parser::ParseStatement(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	for (auto& lexem : lexems)
	{
		std::cout << lexem->value() << "\t\t" << LexemTypeNames.at(lexem->type()) << std::endl;
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
