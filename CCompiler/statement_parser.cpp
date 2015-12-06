#include "statement_parser.h"

#include "grammar.h"


std::shared_ptr<Statement> StatementParser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
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

		auto var_init = std::make_shared<Lexem>(LT::kVarDeclaration, "DEF");
		var_init->set_left(std::static_pointer_cast<Lexem>(lexems.at(0)));
		var_init->set_right(std::static_pointer_cast<Lexem>(lexems.at(1)));
		statement->set_var_init(var_init);
	}
	if (Grammar::IsPunctuator(lexems.back()->type()))
	{
		is_punctuater = 1;
		statement->set_punctuator(lexems.back());
	}

	statement->set_root(InnerStatParse(nullptr, WhereAttachCh::kUnknown, lexems.rbegin() + is_punctuater, lexems.rend() - is_var_decl));

	return statement;
}

std::shared_ptr<LexemInterface> StatementParser::InnerStatParse(std::shared_ptr<LexemInterface> parent, const WhereAttachCh&& side, lexem_interfaces_reverse_iter rbegin, lexem_interfaces_reverse_iter rend)
{
	auto curr_level = GetLowestLevel(rbegin, rend);

	std::shared_ptr<LexemInterface> current_token = nullptr; //if only literal passed, following loops won't be executed and current token will hold the literal as needed
	std::shared_ptr<LexemInterface> result_token = nullptr;
	auto found_needed_op = false;

	for (auto it_bin = Grammar::binary_operators().cbegin(); !found_needed_op && it_bin != Grammar::binary_operators().cend(); ++it_bin)
	{

		for (auto it_tok = rbegin; !found_needed_op && it_tok != rend; ++it_tok)
		{
			current_token = *it_tok;
			if (Grammar::IsBracket(current_token->type())) { continue; }
			result_token = current_token;
			if (curr_level == result_token->level() && Grammar::IsBinaryOperator(result_token->type()) && result_token->value() == *it_bin)
			{
				InnerStatParse(result_token, WhereAttachCh::kLeft, it_tok + 1, rend);
				auto a = it_tok - 1;
				InnerStatParse(result_token, WhereAttachCh::kRight, rbegin, it_tok);
				found_needed_op = true;
 			}
		}
	}
	if (!parent) { return result_token; }

	auto parent_ = std::static_pointer_cast<Lexem>(parent);
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

int StatementParser::GetLowestLevel(lexem_interfaces_reverse_iter rbegin, lexem_interfaces_reverse_iter rend)
{
	auto min_lvl = 10000;
	for (auto it = rbegin; it != rend ; ++it)
	{
		auto curr = (*it);
		if (!Grammar::IsBracket(curr->type()) && curr->level() < min_lvl)
		{
			min_lvl = curr->level();
		}
	}
	return min_lvl;
}
