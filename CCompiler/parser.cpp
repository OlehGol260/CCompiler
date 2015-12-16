#include "parser.h"

#include <regex>

#include "err_msg.h"
#include "grammar.h"
#include "statement_parser.h"
#include "reserved_word_parser.h"

void Parser::Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems) const
{
	auto state = LT::kUnknown;
	std::shared_ptr<LexemeInterface> next_token = nullptr;
	auto next_token_type = LT::kUnknown; //type of next lexem determine the next state, as it may be ambiguouse
	lexem_interface_vec lexems_block;

	auto open_parenthesis_count = 0;
	auto open_brace_count = 0;

	lexems_block.clear();
	auto lexems_size = lexems.size();
	auto is_print = false;
	auto is_if_block = false; // to detect else
	auto is_next_token_invalid = false;

	for (size_t i = 0; i < lexems_size; i++)
	{
		auto current_lexem = lexems.at(i);
		auto a = current_lexem->value();
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

		if (state == LT::kUnknown) {
			// new chunk will be checked here
			switch (current_lexem_type)
			{
			case LT::kPrint:
			case LT::kFor:
			case LT::kWhile:
			case LT::kIf:
			case LT::kVarType:
			case LT::kVar:
				state = current_lexem_type;
				break;
			default:
				ErrMessage::AbortInvalidToken(current_lexem->value());
			}
		}

		switch (state)
		{
		
		case LT::kSqrt:
			if (next_token_type != LT::kOpenParenthesis)
			{
				is_next_token_invalid = true;
			}
			state = next_token_type;
			break;
		case LT::kFalse:
		case LT::kTrue:
			switch(next_token_type)
			{
			case LexemeType::kPunctuator:
			case LexemeType::kBinaryOperator:
			case LexemeType::kCloseParenthesis:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
		case LT::kStringLiteral:
			if (!Grammar::IsCloseParenthesis(next_token_type))
			{
				is_next_token_invalid = true;
			}
			state = next_token_type;
			break;
		case LT::kElse:
			if (!Grammar::IsOpenBrace(next_token_type))
			{
				is_next_token_invalid = true;
			}
			state = next_token_type;
			break;
		case LT::kPrint:
			is_print = true;
			if (!Grammar::IsOpenParenthesis(next_token_type))
			{
				is_next_token_invalid = true;
			}
			state = next_token_type;
			break;
		case LT::kLogicalNot:
			switch (next_token_type)
			{
			case LexemeType::kOpenParenthesis:
			case LexemeType::kFalse:
			case LexemeType::kTrue:
			case LexemeType::kVar:
			case LexemeType::kImmediateFloat:
			case LexemeType::kImmediateInteger:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
		case LT::kIf:
			is_if_block = true; //break here is ommitted on purpose
		case LT::kFor:
		case LT::kWhile:
			if (!Grammar::IsOpenParenthesis(next_token_type))
			{
				is_next_token_invalid = true;
			}
			state = next_token_type;

			break;
		case LT::kDoubleQoute:
			state = LT::kCloseParenthesis;
			break;
		case LT::kBinaryOperator:
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kSqrt:
			case LT::kImmediateFloat:
			case LT::kImmediateInteger:
			case LT::kOpenParenthesis:
			case LT::kLogicalNot:
			case LT::kTrue:
			case LT::kFalse:
				state = next_token_type;
				break;
			default:
				is_next_token_invalid = true;
			}
			break;
			///////////////////////////////////////////
		case LT::kPunctuator:
			state = open_parenthesis_count || open_brace_count ? next_token_type : LT::kUnknown;
			break;
			///////////////////////////////////////////
		case LT::kAssignment:
			switch (next_token_type)
			{
			case LT::kVar:
			case LT::kSqrt:
			case LT::kImmediateFloat:
			case LT::kImmediateInteger:
			case LT::kOpenParenthesis:
			case LT::kTrue:
			case LT::kFalse:
			case LT::kLogicalNot:
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
				ErrMessage::AbortInvalidVariableName(current_lexem->value());
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
		case LT::kImmediateFloat:
		case LT::kImmediateInteger:
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
			case LT::kIf:
			case LT::kPrint:
			case LT::kVarType:
			case LT::kVar:
			case LT::kOpenBrace:
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
				if (is_if_block && Grammar::IsElse(next_token_type))
				{
					state = next_token_type;
					is_if_block = !is_if_block;
				}
				else
				{
					state = LT::kUnknown;
				}
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
			case LT::kStringLiteral:
				if (!is_print)
				{
					is_next_token_invalid = true;
					is_print = false;
				}
			case LT::kVar:
			case LT::kSqrt:
			case LT::kTrue:
			case LT::kFalse:
			case LT::kOpenParenthesis:
			case LT::kLogicalNot:
			case LT::kImmediateFloat:
			case LT::kImmediateInteger:
			case LT::kVarType:
			case LT::kDoubleQoute:
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
		}

		if (is_next_token_invalid)
		{
			ErrMessage::AbortInvalidNextToken(current_lexem->value(), next_token->value());
		}

		current_lexem->set_level(open_parenthesis_count);
		lexems_block.push_back(current_lexem);

		if (state == LT::kUnknown && !lexems_block.empty())
		{
			if (open_brace_count)
			{
				ErrMessage::AbortInvalidNumberBrackets(BracketType::kBraces);
			}
			auto st = Grammar::IsReservedWord(lexems_block.front()->type()) ?
				ReservedWordParser::Parse(lexems_block) :
				StatementParser::Parse(lexems_block);

			main_context_->AddStatement(st);
			lexems_block.clear();
		}

	}
}

bool Parser::IsVariableValid(const std::string& var_name)
{
	std::regex validater("^[a-zA-Z_][a-zA-Z0-9_]*$");
	return std::regex_match(var_name, validater);
}
