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
		// this value will be subtracted from lexems.rend. rend - points to the next element after the last(more in the specification)
		// so if we don't want to include the last element, we need to subtact 2: rend - 1 -> point to the last el,
		// rend - 2 -> point to the element before the last in the vector
		is_var_decl = 2; 

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
	auto lowest_lvl = std::min_element(rbegin, rend, [](std::shared_ptr<LexemInterface> l1, std::shared_ptr<LexemInterface> l2)
	{
		return l1->level() < l2->level();
	});
	auto curr_level = (*lowest_lvl)->level();
	auto current_token = *rbegin; //if only literal passed, following loops won't be executed and current token will hold the literal as needed
	auto found_needed_op = false;

	for (auto it_bin = Grammar::binary_operators().cbegin(); !found_needed_op && it_bin != Grammar::binary_operators().cend(); ++it_bin)
	{
		for (auto it_tok = rbegin; !found_needed_op && it_tok != rend; ++it_tok)
		{
			current_token = *it_tok;
			if (curr_level == current_token->level() && Grammar::IsBinaryOperator(current_token->type()) && current_token->value() == *it_bin)
			{
				InnerStatParse(current_token, WhereAttachCh::kLeft, it_tok + 1, rend);
				InnerStatParse(current_token, WhereAttachCh::kRight, rbegin, it_tok - 1);
				found_needed_op = true;
			}
		}
	}
	if (!parent) { return current_token; }

	auto parent_ = std::static_pointer_cast<Lexem>(parent);
	if (side == WhereAttachCh::kLeft)
	{
		parent_->set_left(current_token);
	}
	else
	{
		parent_->set_right(current_token);
	}
	return parent;
}