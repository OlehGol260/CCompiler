#include "statement_parser.h"
#include <limits>
#include "grammar.h"
#include "lexeme.h"
#include "lexeme_func.h"

std::shared_ptr<Statement> StatementParser::Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	if (lexems.empty()) { return nullptr; }

	auto statement = std::make_shared<Statement>();
	auto is_var_decl = 0;	// do not include variable type, if appeares
	auto is_punctuater = 0; // do not include punctuator, if appeares
	if (Grammar::IsVarType(lexems.front()->type()))
	{
		//	this value will be subtracted from lexems.rend.
		//	rend - points to the next element after the last(more in the specification)
		//	so if we don't want to include the last element, we subtract 1, so it != end will trigger at the last element
		is_var_decl = 1; 

		auto var_init = std::make_shared<Lexeme>(LT::kVarDeclaration, "DEF");
		var_init->set_left(std::static_pointer_cast<Lexeme>(lexems.at(0)));
		var_init->set_right(std::static_pointer_cast<Lexeme>(lexems.at(1)));
		statement->set_var_decl(var_init);
	}
	if (Grammar::IsPunctuator(lexems.back()->type()))
	{
		is_punctuater = 1;
	}

	statement->set_root(InnerStatParse(nullptr, WhereAttachCh::kUnknown, lexems.rbegin() + is_punctuater, lexems.rend() - is_var_decl));

	return statement;
}

std::shared_ptr<LexemeInterface> StatementParser::InnerStatParse(std::shared_ptr<LexemeInterface> parent, const WhereAttachCh&& side, lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend)
{
	auto curr_level = GetLowestLevel(rbegin, rend);

	std::shared_ptr<LexemeInterface> current_token = nullptr; //if only literal passed, following loops won't be executed and current token will hold the literal as needed
	std::shared_ptr<LexemeInterface> result_token = nullptr;
	auto found_needed_op = false;
	int position = 1;
	for (auto it_bin = Grammar::binary_operators().cbegin(); !found_needed_op && it_bin != Grammar::binary_operators().cend(); ++it_bin)
	{
		for (auto it_tok = rbegin; !found_needed_op && it_tok != rend; ++it_tok)
		{
			current_token = *it_tok;
			if (Grammar::IsBracket(current_token->type())) { continue; }
			result_token = current_token;
			position = rend - it_tok;
			if (curr_level == result_token->level() && Grammar::IsBinaryOperator(result_token->type()) && result_token->value() == *it_bin)
			{
				InnerStatParse(result_token, WhereAttachCh::kLeft, it_tok + 1, rend); // +1 - not include current token,
				InnerStatParse(result_token, WhereAttachCh::kRight, rbegin, it_tok); // condition it_tok != rend in the inner loop, doesn't require +1 here
				found_needed_op = true;
 			}
		}
	}
	
	auto result_token_type = result_token->type();

	if (Grammar::IsSqrt(result_token_type) || Grammar::IsLogicalNot(result_token_type))
	{
		auto result_token_func = std::static_pointer_cast<LexemeFunc>(result_token);
		result_token_func->set_body(InnerStatParse(nullptr, WhereAttachCh::kUnknown, rbegin, rend - position));
	}
	if (!parent) { return result_token; }
	auto parent_ = std::static_pointer_cast<Lexeme>(parent);

	if (side == WhereAttachCh::kLeft)
	{
		parent_->set_left(result_token);
	}
	else
	{
		parent_->set_right(result_token);
	}
	return parent;
}

int StatementParser::GetLowestLevel(lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend)
{
	auto min_lvl = std::numeric_limits<int>::max();
	for (auto it = rbegin; it != rend ; ++it)
	{
		auto curr = *it;
		if (!Grammar::IsBracket(curr->type()) && curr->level() < min_lvl)
		{
			min_lvl = curr->level();
		}
	}
	return min_lvl;
}
